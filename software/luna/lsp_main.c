#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include "arena_allocator.h"
#include "json.h"
#include "lsp.h"

static const size_t LSP_ARENA_SIZE = UINT16_MAX * 64;

static char *temp_dir = NULL;

static void cleanup_temp_dir(void) {
    if (temp_dir) {
        char command[512];
        snprintf(command, sizeof(command), "rm -rf '%s'/*", temp_dir);
        system(command);
        free(temp_dir);
        temp_dir = NULL;
    }
}

static char *get_temp_dir(void) {
    if (temp_dir) {
        return temp_dir;
    }

    temp_dir = malloc(256);
    assert(temp_dir != NULL);
    snprintf(temp_dir, 256, "out/tmp");
    mkdir(temp_dir, 0755);
    atexit(cleanup_temp_dir);
    return temp_dir;
}

const char *lsp_extract_uri_from_params(struct ArenaAllocator *allocator,
                                                struct JsonValue *params) {
    if (params->type != JSON_OBJECT) {
        return NULL;
    }

    struct JsonObjectEntry *text_doc_entry = NULL;
    for (size_t i = 0; i < params->value.object_value->count; i++) {
        struct JsonObjectEntry *entry = &params->value.object_value->entries[i];
        if (strncmp(entry->key_ptr, "textDocument", entry->key_length) == 0) {
            text_doc_entry = entry;
            break;
        }
    }

    if (text_doc_entry == NULL || text_doc_entry->value.type != JSON_OBJECT) {
        return NULL;
    }

    struct JsonValue *uri_value = NULL;
    for (size_t i = 0; i < text_doc_entry->value.value.object_value->count; i++) {
        struct JsonObjectEntry *entry = &text_doc_entry->value.value.object_value->entries[i];
        if (strncmp(entry->key_ptr, "uri", entry->key_length) == 0) {
            uri_value = &entry->value;
            break;
        }
    }

    if (uri_value == NULL || uri_value->type != JSON_STRING) {
        return NULL;
    }

    const char *result = arena_alloc(allocator, uri_value->value.string_value.length + 1);
    assert(result != NULL);
    memcpy((char *)result, uri_value->value.string_value.ptr, uri_value->value.string_value.length);
    ((char *)result)[uri_value->value.string_value.length] = '\0';

    return result;
}

int main(void) {
    uint8_t arena_buffer[LSP_ARENA_SIZE];
    struct ArenaAllocator allocator = arena_make(arena_buffer, sizeof(arena_buffer));

    bool running = true;
    bool initialized = false;

    while (running) {
        struct LSPMessage message = lsp_read_message();

        if (message.content == NULL || message.length == 0) {
            break;
        }

        struct JsonError parse_error;
        struct JsonValue json;

        bool parsed = json_parse(&allocator, message.content, message.length,
                                 &json, &parse_error);

        free(message.content);

        if (!parsed) {
            struct LSPResponse error_response = (struct LSPResponse){
                .id = 0,
                .is_error = true,
                .result = (struct JsonValue){.type = JSON_NULL},
                .error = (struct JsonValue){
                    .type = JSON_OBJECT,
                    .value.object_value = NULL
                }
            };

            struct JsonObjectEntry *error_entries = arena_alloc(&allocator, sizeof(struct JsonObjectEntry) * 2);
            assert(error_entries != NULL);

            error_entries[0].key_ptr = "code";
            error_entries[0].key_length = 4;
            error_entries[0].value.type = JSON_NUMBER;
            error_entries[0].value.value.number_value = (double)LSP_PARSE_ERROR;

            error_entries[1].key_ptr = "message";
            error_entries[1].key_length = 7;
            error_entries[1].value.type = JSON_STRING;
            error_entries[1].value.value.string_value.ptr = parse_error.message;
            error_entries[1].value.value.string_value.length = strlen(parse_error.message);

            struct JsonObject *error_obj = arena_alloc(&allocator, sizeof(struct JsonObject));
            assert(error_obj != NULL);
            error_obj->entries = error_entries;
            error_obj->count = 2;

            error_response.error.type = JSON_OBJECT;
            error_response.error.value.object_value = error_obj;

            lsp_send_response(&allocator, &error_response);

            allocator.length = 0;
            continue;
        }

        struct LSPRequest request = lsp_parse_request(&allocator, &json);

        if (request.is_notification) {
            if (request.method == METHOD_TEXT_DOCUMENT_DID_OPEN ||
                request.method == METHOD_TEXT_DOCUMENT_DID_CHANGE) {
                const char *uri = lsp_extract_uri_from_params(&allocator, &request.params);
                if (uri == NULL) {
                    allocator.length = 0;
                    continue;
                }

                char *content = lsp_extract_text_from_params(&allocator, &request.params);
                if (content == NULL) {
                    allocator.length = 0;
                    continue;
                }

                const char *temp_dir_path = get_temp_dir();
                const char *file_name = strrchr(uri, '/');
                if (file_name) {
                    file_name++;
                } else {
                    file_name = uri;
                }

                char *temp_file_path = malloc(strlen(temp_dir_path) + 1 + strlen(file_name) + 1);
                assert(temp_file_path != NULL);
                sprintf(temp_file_path, "%s/%s", temp_dir_path, file_name);

                FILE *f = fopen(temp_file_path, "w");
                if (f) {
                    fwrite(content, 1, strlen(content), f);
                    fclose(f);

                    int exit_code = 0;
                    lsp_compile_and_check(temp_file_path, &exit_code);

                    if (exit_code != 0) {
                        struct LSPDiagnostic diag = {
                            .range = {
                                .start = {0, 0},
                                .end = {0, 0}
                            },
                            .severity = DIAGNOSTIC_ERROR,
                            .message = "source code contains errors",
                            .source = "luna-lsp"
                        };
                        lsp_send_publish_diagnostics(&allocator, uri, &diag, 1);
                    } else {
                        lsp_send_publish_diagnostics(&allocator, uri, NULL, 0);
                    }
                }

                free(temp_file_path);
                allocator.length = 0;
                continue;
            }

            allocator.length = 0;
            continue;
        }

        struct LSPResponse response;

        switch (request.method) {
            case METHOD_INITIALIZE:
                response = lsp_handle_initialize(&allocator, &request);
                initialized = true;
                break;

            case METHOD_SHUTDOWN:
                response = lsp_handle_shutdown(&allocator, &request);
                initialized = false;
                break;

            case METHOD_EXIT:
                running = false;
                response = (struct LSPResponse){
                    .id = request.id,
                    .is_error = false,
                    .result = (struct JsonValue){.type = JSON_NULL},
                    .error = (struct JsonValue){.type = JSON_NULL}
                };
                break;

            case METHOD_TEXT_DOCUMENT_HOVER:
                if (!initialized) {
                    response = (struct LSPResponse){
                        .id = request.id,
                        .is_error = true,
                        .result = (struct JsonValue){.type = JSON_NULL},
                        .error = (struct JsonValue){
                            .type = JSON_OBJECT,
                            .value.object_value = NULL
                        }
                    };

                    struct JsonObjectEntry *error_entries = arena_alloc(&allocator, sizeof(struct JsonObjectEntry) * 2);
                    assert(error_entries != NULL);

                    error_entries[0].key_ptr = "code";
                    error_entries[0].key_length = 4;
                    error_entries[0].value.type = JSON_NUMBER;
                    error_entries[0].value.value.number_value = (double)LSP_SERVER_NOT_INITIALIZED;

                    error_entries[1].key_ptr = "message";
                    error_entries[1].key_length = 7;
                    error_entries[1].value.type = JSON_STRING;
                    error_entries[1].value.value.string_value.ptr = (char *)"Server not initialized";
                    error_entries[1].value.value.string_value.length = 21;

                    struct JsonObject *error_obj = arena_alloc(&allocator, sizeof(struct JsonObject));
                    assert(error_obj != NULL);
                    error_obj->entries = error_entries;
                    error_obj->count = 2;

                    response.error.type = JSON_OBJECT;
                    response.error.value.object_value = error_obj;
                } else {
                    response = lsp_handle_hover(&allocator, &request);
                }
                break;

            default:
                response = (struct LSPResponse){
                    .id = request.id,
                    .is_error = true,
                    .result = (struct JsonValue){.type = JSON_NULL},
                    .error = (struct JsonValue){
                        .type = JSON_OBJECT,
                        .value.object_value = NULL
                    }
                };

                struct JsonObjectEntry *error_entries = arena_alloc(&allocator, sizeof(struct JsonObjectEntry) * 2);
                assert(error_entries != NULL);

                error_entries[0].key_ptr = "code";
                error_entries[0].key_length = 4;
                error_entries[0].value.type = JSON_NUMBER;
                error_entries[0].value.value.number_value = (double)LSP_METHOD_NOT_FOUND;

                error_entries[1].key_ptr = "message";
                error_entries[1].key_length = 7;
                error_entries[1].value.type = JSON_STRING;
                error_entries[1].value.value.string_value.ptr = (char *)"Method not found";
                error_entries[1].value.value.string_value.length = 16;

                struct JsonObject *error_obj = arena_alloc(&allocator, sizeof(struct JsonObject));
                assert(error_obj != NULL);
                error_obj->entries = error_entries;
                error_obj->count = 2;

                response.error.type = JSON_OBJECT;
                response.error.value.object_value = error_obj;
                break;
        }

        lsp_send_response(&allocator, &response);

        allocator.length = 0;
    }

    return 0;
}
