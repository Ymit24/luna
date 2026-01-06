#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#include "arena_allocator.h"
#include "ast.h"
#include "lexer.h"
#include "parser.h"
#include "luna_string.h"
#include "lsp_query.h"

#define BUFFER_SIZE (1024 * 1024)
#define ARENA_SIZE UINT16_MAX * 16
#define MAX_METHOD_LEN 128
#define MAX_RESPONSE_LEN (16 * 1024)
#define MAX_OPEN_FILES 32
#define MAX_URI_LEN 512
#define MAX_FILE_CONTENT_LEN (128 * 1024)

static char buffer[BUFFER_SIZE];
static uint8_t arena_data[ARENA_SIZE];
static struct ArenaAllocator global_arena;

struct Document {
    char uri[MAX_URI_LEN];
    char content[MAX_FILE_CONTENT_LEN];
    size_t content_len;
    bool in_use;
};

static struct Document open_documents[MAX_OPEN_FILES];

static int find_content_length(const char *header) {
    const char *prefix = "Content-Length:";
    const char *ptr = strstr(header, prefix);
    if (!ptr) {
        return -1;
    }
    ptr += strlen(prefix);
    while (*ptr && isspace(*ptr)) {
        ptr++;
    }
    return atoi(ptr);
}

static ssize_t read_message(char *out_buf, size_t max_len) {
    char header[1024];
    size_t header_pos = 0;
    int c;

    while ((c = getchar()) != EOF) {
        if (c == '\r') {
            continue;
        }
        if (c == '\n') {
            header[header_pos] = '\0';
            break;
        }
        if (header_pos < sizeof(header) - 1) {
            header[header_pos++] = c;
        }
    }

    if (c == EOF || header_pos == 0) {
        return -1;
    }

    int content_length = find_content_length(header);
    if (content_length < 0) {
        return -1;
    }

    if ((size_t)content_length > max_len - 1) {
        return -1;
    }

    c = getchar();
    if (c != '\r' && c != '\n') {
        return -1;
    }

    size_t read_pos = 0;
    while (read_pos < (size_t)content_length && (c = getchar()) != EOF) {
        out_buf[read_pos++] = c;
    }
    out_buf[read_pos] = '\0';

    return read_pos;
}

static void extract_string(const char *json, const char *key, char *out, size_t max_len) {
    char search_key[256];
    snprintf(search_key, sizeof(search_key), "\"%s\"", key);
    const char *ptr = strstr(json, search_key);
    if (!ptr) {
        out[0] = '\0';
        return;
    }
    ptr = strchr(ptr, ':');
    if (!ptr) {
        out[0] = '\0';
        return;
    }
    ptr++;
    while (*ptr && isspace(*ptr)) {
        ptr++;
    }
    if (*ptr == '"') {
        ptr++;
        size_t i = 0;
        while (*ptr && *ptr != '"' && i < max_len - 1) {
            out[i++] = *ptr++;
        }
        out[i] = '\0';
    } else {
        out[0] = '\0';
    }
}

static char *extract_value(const char *json, const char *key) {
    char search_key[256];
    snprintf(search_key, sizeof(search_key), "\"%s\"", key);
    const char *ptr = strstr(json, search_key);
    if (!ptr) {
        return NULL;
    }
    ptr = strchr(ptr, ':');
    if (!ptr) {
        return NULL;
    }
    ptr++;
    while (*ptr && isspace(*ptr)) {
        ptr++;
    }
    return (char *)ptr;
}

static int extract_number(const char *json, const char *key) {
    char search_key[256];
    snprintf(search_key, sizeof(search_key), "\"%s\"", key);
    const char *ptr = strstr(json, search_key);
    if (!ptr) {
        return -1;
    }
    ptr = strchr(ptr, ':');
    if (!ptr) {
        return -1;
    }
    ptr++;
    while (*ptr && isspace(*ptr)) {
        ptr++;
    }
    return atoi(ptr);
}

static void send_response(const char *json_body) {
    size_t len = strlen(json_body);
    printf("Content-Length: %zu\r\n\r\n%s", len, json_body);
    fflush(stdout);
}

static struct Document *find_document(const char *uri) {
    for (int i = 0; i < MAX_OPEN_FILES; i++) {
        if (open_documents[i].in_use && strcmp(open_documents[i].uri, uri) == 0) {
            return &open_documents[i];
        }
    }
    return NULL;
}

static struct Document *add_document(const char *uri, const char *content) {
    for (int i = 0; i < MAX_OPEN_FILES; i++) {
        if (!open_documents[i].in_use) {
            strncpy(open_documents[i].uri, uri, MAX_URI_LEN - 1);
            open_documents[i].uri[MAX_URI_LEN - 1] = '\0';
            strncpy(open_documents[i].content, content, MAX_FILE_CONTENT_LEN - 1);
            open_documents[i].content_len = strlen(content);
            if (open_documents[i].content_len > MAX_FILE_CONTENT_LEN - 1) {
                open_documents[i].content_len = MAX_FILE_CONTENT_LEN - 1;
            }
            open_documents[i].content[open_documents[i].content_len] = '\0';
            open_documents[i].in_use = true;
            return &open_documents[i];
        }
    }
    return NULL;
}

static void handle_initialize(const char *id_str) {
    char response[MAX_RESPONSE_LEN];
    snprintf(response, sizeof(response),
             "{\"jsonrpc\":\"2.0\",\"id\":%s,\"result\":{"
             "\"capabilities\":{"
             "\"hoverProvider\":true,"
             "\"textDocumentSync\":{\"openClose\":true,\"change\":1}"
             "},"
             "\"serverInfo\":{\"name\":\"luna-lsp\",\"version\":\"0.1.0\"}"
             "}}",
             id_str);
    send_response(response);
}

static void handle_textDocument_didOpen(void) {
    char uri[MAX_URI_LEN];
    extract_string(buffer, "uri", uri, sizeof(uri));

    char text[MAX_FILE_CONTENT_LEN];
    extract_string(buffer, "text", text, sizeof(text));

    add_document(uri, text);
}

static void handle_hover(const char *id_str) {
    char response[MAX_RESPONSE_LEN];

    char uri[MAX_URI_LEN];
    extract_string(buffer, "uri", uri, sizeof(uri));

    struct Document *doc = find_document(uri);
    if (!doc) {
        snprintf(response, sizeof(response),
                 "{\"jsonrpc\":\"2.0\",\"id\":%s,\"result\":{"
                 "\"contents\":\"Document not open in editor\""
                 "}}",
                 id_str);
        send_response(response);
        return;
    }

    int line = extract_number(buffer, "line");
    int character = extract_number(buffer, "character");

    if (line < 0 || character < 0) {
        snprintf(response, sizeof(response),
                 "{\"jsonrpc\":\"2.0\",\"id\":%s,\"result\":null}",
                 id_str);
        send_response(response);
        return;
    }

    global_arena = arena_make(arena_data, sizeof(arena_data));

    struct LunaString src_str = string_make(doc->content);
    struct LunaString filename = string_make("buffer.luna");
    struct Lexer lexer = lexer_make(&global_arena, src_str, filename);

    struct Token toks[4096];
    uint16_t tok_index = 0;

    while (lexer_next(&lexer, &toks[tok_index++])) {
    }

    if (tok_index == 0) {
        snprintf(response, sizeof(response),
                 "{\"jsonrpc\":\"2.0\",\"id\":%s,\"result\":{"
                 "\"contents\":\"File is empty or contains no code\""
                 "}}",
                 id_str);
        send_response(response);
        return;
    }

    if (tok_index == 1 && toks[0].type == T_EOF) {
        snprintf(response, sizeof(response),
                 "{\"jsonrpc\":\"2.0\",\"id\":%s,\"result\":{"
                 "\"contents\":\"File contains no valid Luna code\""
                 "}}",
                 id_str);
        send_response(response);
        return;
    }

    struct Parser parser = parser_make(&global_arena, toks, tok_index);

    struct ModuleStatementNode *ast = parse_module_statements(&parser);

    if (!ast) {
        snprintf(response, sizeof(response),
                 "{\"jsonrpc\":\"2.0\",\"id\":%s,\"result\":{"
                 "\"contents\":\"Parse error: Could not parse file\""
                 "}}",
                 id_str);
        send_response(response);
        return;
    }

    struct LineMap map = build_line_map(src_str, &global_arena);
    uint16_t byte_offset = line_col_to_byte_offset(map, (uint16_t)line, (uint16_t)character, src_str);

    struct ContainingNode result = find_smallest_containing_node(ast, byte_offset);

    char hover_text[1024];
    query_hover_info(result, hover_text, sizeof(hover_text));

    snprintf(response, sizeof(response),
             "{\"jsonrpc\":\"2.0\",\"id\":%s,\"result\":{"
             "\"contents\":{\"kind\":\"markdown\",\"value\":\"%s\"}"
             "}}",
             id_str, hover_text);
    send_response(response);
}

int main(void) {
    while (1) {
        ssize_t msg_len = read_message(buffer, sizeof(buffer));
        if (msg_len < 0) {
            break;
        }

        char method[MAX_METHOD_LEN];
        extract_string(buffer, "method", method, sizeof(method));

        char *id_value = extract_value(buffer, "id");
        char id_str[256];
        if (id_value) {
            if (*id_value == '"') {
                const char *start = id_value + 1;
                const char *end = strchr(start, '"');
                if (end) {
                    size_t len = end - start;
                    strncpy(id_str, start, len);
                    id_str[len] = '\0';
                } else {
                    id_str[0] = '\0';
                }
            } else if (isdigit(*id_value) || *id_value == '-') {
                const char *ptr = id_value;
                size_t i = 0;
                while (i < sizeof(id_str) - 1 && (isdigit(*ptr) || *ptr == '-')) {
                    id_str[i++] = *ptr++;
                }
                id_str[i] = '\0';
            } else {
                id_str[0] = '\0';
            }
        } else {
            id_str[0] = '\0';
        }

        if (strcmp(method, "initialize") == 0) {
            handle_initialize(id_str);
        } else if (strcmp(method, "initialized") == 0) {
            continue;
        } else if (strcmp(method, "textDocument/didOpen") == 0) {
            handle_textDocument_didOpen();
        } else if (strcmp(method, "textDocument/hover") == 0) {
            handle_hover(id_str);
        } else if (strcmp(method, "shutdown") == 0) {
            if (id_str[0]) {
                send_response("{\"jsonrpc\":\"2.0\",\"id\":null,\"result\":null}");
            }
            break;
        } else if (strcmp(method, "exit") == 0) {
            break;
        }
    }

    return 0;
}
