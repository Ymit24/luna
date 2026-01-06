#include "lsp.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const size_t LSP_MESSAGE_BUFFER_SIZE = 65536;
static const size_t LSP_HEADER_BUFFER_SIZE = 256;

enum LSPMethod lsp_method_from_string(const char *method_name) {
    if (method_name == NULL) {
        return METHOD_UNDEFINED;
    }

    if (strcmp(method_name, "initialize") == 0) {
        return METHOD_INITIALIZE;
    } else if (strcmp(method_name, "shutdown") == 0) {
        return METHOD_SHUTDOWN;
    } else if (strcmp(method_name, "exit") == 0) {
        return METHOD_EXIT;
    } else if (strcmp(method_name, "textDocument/hover") == 0) {
        return METHOD_TEXT_DOCUMENT_HOVER;
    } else if (strcmp(method_name, "textDocument/didOpen") == 0) {
        return METHOD_TEXT_DOCUMENT_DID_OPEN;
    } else if (strcmp(method_name, "textDocument/didChange") == 0) {
        return METHOD_TEXT_DOCUMENT_DID_CHANGE;
    } else if (strcmp(method_name, "textDocument/didClose") == 0) {
        return METHOD_TEXT_DOCUMENT_DID_CLOSE;
    }

    return METHOD_UNDEFINED;
}

struct LSPMessage lsp_read_message(void) {
    struct LSPMessage message = {.length = 0, .content = NULL};

    char header_buffer[LSP_HEADER_BUFFER_SIZE];
    size_t header_length = 0;

    while (header_length < LSP_HEADER_BUFFER_SIZE - 1) {
        int c = fgetc(stdin);
        if (c == EOF) {
            return message;
        }

        if (c == '\r') {
            int next = fgetc(stdin);
            if (next == '\n') {
                break;
            } else {
                ungetc(next, stdin);
            }
        } else {
            header_buffer[header_length++] = (char)c;
        }
    }

    header_buffer[header_length] = '\0';

    size_t content_length = 0;
    char *header_end = strstr(header_buffer, "Content-Length:");
    if (header_end != NULL) {
        sscanf(header_end, "Content-Length: %zu", &content_length);
    }

    if (content_length == 0) {
        return message;
    }

    int c = fgetc(stdin);
    if (c == '\r') {
        int next = fgetc(stdin);
        if (next != '\n') {
            ungetc(next, stdin);
        }
    } else {
        ungetc(c, stdin);
    }

    char *content = malloc(content_length + 1);
    assert(content != NULL);

    size_t bytes_read = fread(content, 1, content_length, stdin);
    (void)bytes_read;

    content[content_length] = '\0';

    message.length = content_length;
    message.content = content;

    return message;
}

size_t lsp_write_message(char *buffer, size_t buffer_capacity,
                         const char *content, size_t content_length) {
    if (content_length >= buffer_capacity) {
        return 0;
    }

    int written = snprintf(buffer, buffer_capacity,
                           "Content-Length: %zu\r\n\r\n%.*s",
                           content_length, (int)content_length, content);

    return (size_t)written;
}

struct LSPRequest lsp_parse_request(struct ArenaAllocator *allocator,
                                    struct JsonValue *json) {
    assert(allocator != NULL);
    assert(json != NULL);

    struct LSPRequest request = (struct LSPRequest){
        .id = -1,
        .is_notification = true,
        .method = METHOD_UNDEFINED,
        .params = (struct JsonValue){.type = JSON_NULL}
    };

    if (json->type != JSON_OBJECT) {
        return request;
    }

    struct JsonValue id_value = (struct JsonValue){.type = JSON_NULL};
    struct JsonValue method_value = (struct JsonValue){.type = JSON_NULL};
    struct JsonValue params_value = (struct JsonValue){.type = JSON_NULL};

    for (size_t i = 0; i < json->value.object_value->count; i++) {
        struct JsonObjectEntry *entry = &json->value.object_value->entries[i];

        if (strncmp(entry->key_ptr, "id", entry->key_length) == 0) {
            id_value = entry->value;
        } else if (strncmp(entry->key_ptr, "method", entry->key_length) == 0) {
            method_value = entry->value;
        } else if (strncmp(entry->key_ptr, "params", entry->key_length) == 0) {
            params_value = entry->value;
        }
    }

    if (id_value.type == JSON_NUMBER) {
        request.id = (int64_t)id_value.value.number_value;
        request.is_notification = false;
    } else if (id_value.type == JSON_STRING) {
        char id_str[64];
        size_t copy_len = id_value.value.string_value.length;
        if (copy_len >= sizeof(id_str)) {
            copy_len = sizeof(id_str) - 1;
        }
        memcpy(id_str, id_value.value.string_value.ptr, copy_len);
        id_str[copy_len] = '\0';
        request.id = (int64_t)atol(id_str);
        request.is_notification = false;
    }

    if (method_value.type == JSON_STRING) {
        char method_name[256];
        size_t copy_len = method_value.value.string_value.length;
        if (copy_len >= sizeof(method_name)) {
            copy_len = sizeof(method_name) - 1;
        }
        memcpy(method_name, method_value.value.string_value.ptr, copy_len);
        method_name[copy_len] = '\0';

        request.method = lsp_method_from_string(method_name);
    }

    if (params_value.type == JSON_OBJECT || params_value.type == JSON_ARRAY) {
        request.params = params_value;
    }

    return request;
}

void lsp_send_response(struct ArenaAllocator *allocator,
                       struct LSPResponse *response) {
    assert(response != NULL);
    assert(allocator != NULL);

    struct JsonObjectEntry *entries = arena_alloc(allocator, sizeof(struct JsonObjectEntry) * 4);
    assert(entries != NULL);

    entries[0].key_ptr = "jsonrpc";
    entries[0].key_length = 7;
    entries[0].value.type = JSON_STRING;
    entries[0].value.value.string_value.ptr = (char *)"2.0";
    entries[0].value.value.string_value.length = 3;

    entries[1].key_ptr = "id";
    entries[1].key_length = 2;
    entries[1].value.type = JSON_NUMBER;
    entries[1].value.value.number_value = (double)response->id;

    struct JsonObject *response_obj = arena_alloc(allocator, sizeof(struct JsonObject));
    assert(response_obj != NULL);
    response_obj->entries = entries;

    if (response->is_error) {
        entries[2].key_ptr = "error";
        entries[2].key_length = 5;
        entries[2].value = response->error;
        response_obj->count = 3;
    } else {
        entries[2].key_ptr = "result";
        entries[2].key_length = 6;
        entries[2].value = response->result;
        response_obj->count = 3;
    }

    struct JsonValue response_json;
    response_json.type = JSON_OBJECT;
    response_json.value.object_value = response_obj;

    char buffer[LSP_MESSAGE_BUFFER_SIZE];
    size_t encoded = json_encode_minified(allocator, &response_json, buffer,
                                          sizeof(buffer) - 1);

    if (encoded > 0) {
        char output[LSP_MESSAGE_BUFFER_SIZE];
        size_t written = lsp_write_message(output, sizeof(output), buffer, encoded);

        if (written > 0) {
            fwrite(output, 1, written, stdout);
            fflush(stdout);
        }
    }
}

struct LSPResponse lsp_handle_initialize(struct ArenaAllocator *allocator,
                                         struct LSPRequest *request) {
    struct LSPResponse response = (struct LSPResponse){
        .id = request->id,
        .is_error = false,
        .result = (struct JsonValue){.type = JSON_NULL},
        .error = (struct JsonValue){.type = JSON_NULL}
    };

    struct JsonObjectEntry *server_info_entries = arena_alloc(allocator, sizeof(struct JsonObjectEntry) * 2);
    assert(server_info_entries != NULL);

    server_info_entries[0].key_ptr = "name";
    server_info_entries[0].key_length = 4;
    server_info_entries[0].value.type = JSON_STRING;
    server_info_entries[0].value.value.string_value.ptr = (char *)"Luna Language Server";
    server_info_entries[0].value.value.string_value.length = 20;

    server_info_entries[1].key_ptr = "version";
    server_info_entries[1].key_length = 7;
    server_info_entries[1].value.type = JSON_STRING;
    server_info_entries[1].value.value.string_value.ptr = (char *)"0.1.0";
    server_info_entries[1].value.value.string_value.length = 5;

    struct JsonObject *server_info_obj = arena_alloc(allocator, sizeof(struct JsonObject));
    assert(server_info_obj != NULL);
    server_info_obj->entries = server_info_entries;
    server_info_obj->count = 2;

    struct JsonObjectEntry *capabilities_entries = arena_alloc(allocator, sizeof(struct JsonObjectEntry) * 2);
    assert(capabilities_entries != NULL);

    capabilities_entries[0].key_ptr = "textDocumentSync";
    capabilities_entries[0].key_length = 16;
    capabilities_entries[0].value.type = JSON_NUMBER;
    capabilities_entries[0].value.value.number_value = 1;

    capabilities_entries[1].key_ptr = "hoverProvider";
    capabilities_entries[1].key_length = 13;
    capabilities_entries[1].value.type = JSON_BOOL;
    capabilities_entries[1].value.value.bool_value = true;

    struct JsonObject *capabilities_obj = arena_alloc(allocator, sizeof(struct JsonObject));
    assert(capabilities_obj != NULL);
    capabilities_obj->entries = capabilities_entries;
    capabilities_obj->count = 2;

    struct JsonObjectEntry *result_entries = arena_alloc(allocator, sizeof(struct JsonObjectEntry) * 2);
    assert(result_entries != NULL);

    result_entries[0].key_ptr = "serverInfo";
    result_entries[0].key_length = 10;
    result_entries[0].value.type = JSON_OBJECT;
    result_entries[0].value.value.object_value = server_info_obj;

    result_entries[1].key_ptr = "capabilities";
    result_entries[1].key_length = 12;
    result_entries[1].value.type = JSON_OBJECT;
    result_entries[1].value.value.object_value = capabilities_obj;

    struct JsonObject *result_obj = arena_alloc(allocator, sizeof(struct JsonObject));
    assert(result_obj != NULL);
    result_obj->entries = result_entries;
    result_obj->count = 2;

    response.result.type = JSON_OBJECT;
    response.result.value.object_value = result_obj;

    return response;
}

struct LSPResponse lsp_handle_shutdown(struct ArenaAllocator *allocator,
                                       struct LSPRequest *request) {
    (void)allocator;

    struct LSPResponse response = (struct LSPResponse){
        .id = request->id,
        .is_error = false,
        .result = (struct JsonValue){.type = JSON_NULL},
        .error = (struct JsonValue){.type = JSON_NULL}
    };

    return response;
}

struct LSPResponse lsp_handle_hover(struct ArenaAllocator *allocator,
                                    struct LSPRequest *request) {
    (void)allocator;

    struct LSPResponse response = (struct LSPResponse){
        .id = request->id,
        .is_error = false,
        .result = (struct JsonValue){.type = JSON_NULL},
        .error = (struct JsonValue){.type = JSON_NULL}
    };

    const char *hover_content = "Hello world from luna lsp";

    struct JsonObjectEntry *result_entries = arena_alloc(allocator, sizeof(struct JsonObjectEntry) * 1);
    assert(result_entries != NULL);

    result_entries[0].key_ptr = "contents";
    result_entries[0].key_length = 8;
    result_entries[0].value.type = JSON_STRING;
    result_entries[0].value.value.string_value.ptr = (char *)hover_content;
    result_entries[0].value.value.string_value.length = strlen(hover_content);

    struct JsonObject *result_obj = arena_alloc(allocator, sizeof(struct JsonObject));
    assert(result_obj != NULL);
    result_obj->entries = result_entries;
    result_obj->count = 1;

    response.result.type = JSON_OBJECT;
    response.result.value.object_value = result_obj;

    return response;
}

char *lsp_uri_to_path(const char *uri) {
    if (uri == NULL) {
        return NULL;
    }
    if (strncmp(uri, "file://", 7) != 0) {
        return NULL;
    }
    const char *path = uri + 7;
    char *result = strdup(path);
    return result;
}

bool lsp_compile_and_check(const char *file_path, int *out_exit_code) {
    char command[512];
    snprintf(command, sizeof(command), "./bin/lunac '%s' >/dev/null 2>&1", file_path);

    int status = system(command);
    *out_exit_code = WEXITSTATUS(status);
    return true;
}

void lsp_send_publish_diagnostics(struct ArenaAllocator *allocator,
                                    const char *uri,
                                    struct LSPDiagnostic *diagnostics,
                                    size_t count) {
    struct JsonObjectEntry *params_entries = arena_alloc(allocator, sizeof(struct JsonObjectEntry) * 2);
    assert(params_entries != NULL);

    params_entries[0].key_ptr = "uri";
    params_entries[0].key_length = 3;
    params_entries[0].value.type = JSON_STRING;
    params_entries[0].value.value.string_value.ptr = (char *)uri;
    params_entries[0].value.value.string_value.length = strlen(uri);

    if (count == 0) {
        params_entries[1].key_ptr = "diagnostics";
        params_entries[1].key_length = 11;
        params_entries[1].value.type = JSON_ARRAY;
        params_entries[1].value.value.array_value = NULL;

        struct JsonObject *params_obj = arena_alloc(allocator, sizeof(struct JsonObject));
        assert(params_obj != NULL);
        params_obj->entries = params_entries;
        params_obj->count = 2;

        struct JsonValue params_value;
        params_value.type = JSON_OBJECT;
        params_value.value.object_value = params_obj;

        struct JsonObjectEntry *root_entries = arena_alloc(allocator, sizeof(struct JsonObjectEntry) * 3);
        assert(root_entries != NULL);

        root_entries[0].key_ptr = "jsonrpc";
        root_entries[0].key_length = 7;
        root_entries[0].value.type = JSON_STRING;
        root_entries[0].value.value.string_value.ptr = (char *)"2.0";
        root_entries[0].value.value.string_value.length = 3;

        root_entries[1].key_ptr = "method";
        root_entries[1].key_length = 6;
        root_entries[1].value.type = JSON_STRING;
        root_entries[1].value.value.string_value.ptr = (char *)"textDocument/publishDiagnostics";
        root_entries[1].value.value.string_value.length = 31;

        root_entries[2].key_ptr = "params";
        root_entries[2].key_length = 6;
        root_entries[2].value = params_value;

        struct JsonObject *root_obj = arena_alloc(allocator, sizeof(struct JsonObject));
        assert(root_obj != NULL);
        root_obj->entries = root_entries;
        root_obj->count = 3;

        struct JsonValue root_value;
        root_value.type = JSON_OBJECT;
        root_value.value.object_value = root_obj;

        char buffer[LSP_MESSAGE_BUFFER_SIZE];
        size_t encoded = json_encode_minified(allocator, &root_value, buffer, sizeof(buffer) - 1);

        if (encoded > 0) {
            char output[LSP_MESSAGE_BUFFER_SIZE];
            size_t written = lsp_write_message(output, sizeof(output), buffer, encoded);
            if (written > 0) {
                fwrite(output, 1, written, stdout);
                fflush(stdout);
            }
        }
        return;
    }

    struct JsonObjectEntry *diag_entries = arena_alloc(allocator, sizeof(struct JsonObjectEntry) * 4);
    assert(diag_entries != NULL);

    for (size_t i = 0; i < count; i++) {
        struct JsonObjectEntry *range_entries = arena_alloc(allocator, sizeof(struct JsonObjectEntry) * 2);
        assert(range_entries != NULL);

        struct JsonObjectEntry *start_entries = arena_alloc(allocator, sizeof(struct JsonObjectEntry) * 2);
        assert(start_entries != NULL);
        start_entries[0].key_ptr = "line";
        start_entries[0].key_length = 4;
        start_entries[0].value.type = JSON_NUMBER;
        start_entries[0].value.value.number_value = (double)diagnostics[i].range.start.line;
        start_entries[1].key_ptr = "character";
        start_entries[1].key_length = 9;
        start_entries[1].value.type = JSON_NUMBER;
        start_entries[1].value.value.number_value = (double)diagnostics[i].range.start.character;

        struct JsonObject *start_obj = arena_alloc(allocator, sizeof(struct JsonObject));
        assert(start_obj != NULL);
        start_obj->entries = start_entries;
        start_obj->count = 2;

        range_entries[0].key_ptr = "start";
        range_entries[0].key_length = 5;
        range_entries[0].value.type = JSON_OBJECT;
        range_entries[0].value.value.object_value = start_obj;

        struct JsonObjectEntry *end_entries = arena_alloc(allocator, sizeof(struct JsonObjectEntry) * 2);
        assert(end_entries != NULL);
        end_entries[0].key_ptr = "line";
        end_entries[0].key_length = 4;
        end_entries[0].value.type = JSON_NUMBER;
        end_entries[0].value.value.number_value = (double)diagnostics[i].range.end.line;
        end_entries[1].key_ptr = "character";
        end_entries[1].key_length = 9;
        end_entries[1].value.type = JSON_NUMBER;
        end_entries[1].value.value.number_value = (double)diagnostics[i].range.end.character;

        struct JsonObject *end_obj = arena_alloc(allocator, sizeof(struct JsonObject));
        assert(end_obj != NULL);
        end_obj->entries = end_entries;
        end_obj->count = 2;

        range_entries[1].key_ptr = "end";
        range_entries[1].key_length = 3;
        range_entries[1].value.type = JSON_OBJECT;
        range_entries[1].value.value.object_value = end_obj;

        struct JsonObject *range_obj = arena_alloc(allocator, sizeof(struct JsonObject));
        assert(range_obj != NULL);
        range_obj->entries = range_entries;
        range_obj->count = 2;

        diag_entries[i * 4 + 0].key_ptr = "range";
        diag_entries[i * 4 + 0].key_length = 5;
        diag_entries[i * 4 + 0].value.type = JSON_OBJECT;
        diag_entries[i * 4 + 0].value.value.object_value = range_obj;

        diag_entries[i * 4 + 1].key_ptr = "severity";
        diag_entries[i * 4 + 1].key_length = 8;
        diag_entries[i * 4 + 1].value.type = JSON_NUMBER;
        diag_entries[i * 4 + 1].value.value.number_value = (double)diagnostics[i].severity;

        diag_entries[i * 4 + 2].key_ptr = "message";
        diag_entries[i * 4 + 2].key_length = 7;
        diag_entries[i * 4 + 2].value.type = JSON_STRING;
        diag_entries[i * 4 + 2].value.value.string_value.ptr = diagnostics[i].message;
        diag_entries[i * 4 + 2].value.value.string_value.length = strlen(diagnostics[i].message);

        diag_entries[i * 4 + 3].key_ptr = "source";
        diag_entries[i * 4 + 3].key_length = 6;
        diag_entries[i * 4 + 3].value.type = JSON_STRING;
        diag_entries[i * 4 + 3].value.value.string_value.ptr = diagnostics[i].source;
        diag_entries[i * 4 + 3].value.value.string_value.length = strlen(diagnostics[i].source);
    }

    struct JsonObject *diags_obj = arena_alloc(allocator, sizeof(struct JsonObject));
    assert(diags_obj != NULL);
    diags_obj->entries = diag_entries;
    diags_obj->count = count * 4;

    struct JsonArray *diags_array = arena_alloc(allocator, sizeof(struct JsonArray));
    assert(diags_array != NULL);
    diags_array->count = count;

    struct JsonValue *diag_values = arena_alloc(allocator, sizeof(struct JsonValue) * count);
    assert(diag_values != NULL);
    for (size_t i = 0; i < count; i++) {
        struct JsonObject *diag_obj = arena_alloc(allocator, sizeof(struct JsonObject));
        assert(diag_obj != NULL);
        diag_obj->entries = &diag_entries[i * 4];
        diag_obj->count = 4;
        diag_values[i].type = JSON_OBJECT;
        diag_values[i].value.object_value = diag_obj;
    }
    diags_array->values = diag_values;

    params_entries[1].key_ptr = "diagnostics";
    params_entries[1].key_length = 11;
    params_entries[1].value.type = JSON_ARRAY;
    params_entries[1].value.value.array_value = diags_array;

    struct JsonObject *params_obj = arena_alloc(allocator, sizeof(struct JsonObject));
    assert(params_obj != NULL);
    params_obj->entries = params_entries;
    params_obj->count = 2;

    struct JsonValue params_value;
    params_value.type = JSON_OBJECT;
    params_value.value.object_value = params_obj;

    struct JsonObjectEntry *root_entries = arena_alloc(allocator, sizeof(struct JsonObjectEntry) * 3);
    assert(root_entries != NULL);

    root_entries[0].key_ptr = "jsonrpc";
    root_entries[0].key_length = 7;
    root_entries[0].value.type = JSON_STRING;
    root_entries[0].value.value.string_value.ptr = (char *)"2.0";
    root_entries[0].value.value.string_value.length = 3;

    root_entries[1].key_ptr = "method";
    root_entries[1].key_length = 6;
    root_entries[1].value.type = JSON_STRING;
    root_entries[1].value.value.string_value.ptr = (char *)"textDocument/publishDiagnostics";
    root_entries[1].value.value.string_value.length = 31;

    root_entries[2].key_ptr = "params";
    root_entries[2].key_length = 6;
    root_entries[2].value = params_value;

    struct JsonObject *root_obj = arena_alloc(allocator, sizeof(struct JsonObject));
    assert(root_obj != NULL);
    root_obj->entries = root_entries;
    root_obj->count = 3;

    struct JsonValue root_value;
    root_value.type = JSON_OBJECT;
    root_value.value.object_value = root_obj;

    char buffer[LSP_MESSAGE_BUFFER_SIZE];
    size_t encoded = json_encode_minified(allocator, &root_value, buffer, sizeof(buffer) - 1);

    if (encoded > 0) {
        char output[LSP_MESSAGE_BUFFER_SIZE];
        size_t written = lsp_write_message(output, sizeof(output), buffer, encoded);
        if (written > 0) {
            fwrite(output, 1, written, stdout);
            fflush(stdout);
        }
    }
}

char *lsp_extract_text_from_params(struct ArenaAllocator *allocator,
                                    struct JsonValue *params) {
    if (params->type != JSON_OBJECT) {
        return NULL;
    }

    struct JsonObjectEntry *content_changes_entry = NULL;

    for (size_t i = 0; i < params->value.object_value->count; i++) {
        struct JsonObjectEntry *entry = &params->value.object_value->entries[i];
        if (strncmp(entry->key_ptr, "contentChanges", entry->key_length) == 0) {
            content_changes_entry = entry;
            break;
        }
    }

    if (content_changes_entry == NULL || content_changes_entry->value.type != JSON_ARRAY) {
        return NULL;
    }

    struct JsonArray *changes = content_changes_entry->value.value.array_value;
    if (changes == NULL || changes->count == 0) {
        return NULL;
    }

    struct JsonValue *first_change = &changes->values[0];
    if (first_change->type != JSON_OBJECT) {
        return NULL;
    }

    struct JsonValue *text_value = NULL;
    for (size_t i = 0; i < first_change->value.object_value->count; i++) {
        struct JsonObjectEntry *entry = &first_change->value.object_value->entries[i];
        if (strncmp(entry->key_ptr, "text", entry->key_length) == 0) {
            text_value = &entry->value;
            break;
        }
    }

    if (text_value == NULL || text_value->type != JSON_STRING) {
        return NULL;
    }

    char *result = arena_alloc(allocator, text_value->value.string_value.length + 1);
    assert(result != NULL);
    memcpy(result, text_value->value.string_value.ptr, text_value->value.string_value.length);
    result[text_value->value.string_value.length] = '\0';

    return result;
}
