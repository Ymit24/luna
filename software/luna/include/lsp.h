#ifndef LSP_H
#define LSP_H

#include <stdbool.h>
#include <stdint.h>

#include "arena_allocator.h"
#include "json.h"

enum LSPMethod {
    METHOD_UNDEFINED,
    METHOD_INITIALIZE,
    METHOD_SHUTDOWN,
    METHOD_EXIT,
    METHOD_TEXT_DOCUMENT_HOVER,
    METHOD_TEXT_DOCUMENT_DID_OPEN,
    METHOD_TEXT_DOCUMENT_DID_CHANGE,
    METHOD_TEXT_DOCUMENT_DID_CLOSE,
    METHOD_WINDOW_SHOW_MESSAGE,
    METHOD_WINDOW_LOG_MESSAGE
};

struct LSPRequest {
    int64_t id;
    bool is_notification;
    enum LSPMethod method;
    struct JsonValue params;
};

struct LSPResponse {
    int64_t id;
    bool is_error;
    struct JsonValue result;
    struct JsonValue error;
};

enum LSPDiagnosticSeverity {
    DIAGNOSTIC_ERROR = 1,
    DIAGNOSTIC_WARNING = 2,
    DIAGNOSTIC_INFO = 3
};

struct LSPPosition {
    int64_t line;
    int64_t character;
};

struct LSPRange {
    struct LSPPosition start;
    struct LSPPosition end;
};

struct LSPDiagnostic {
    struct LSPRange range;
    enum LSPDiagnosticSeverity severity;
    char *message;
    char *source;
};

enum LSPErrorCode {
    LSP_PARSE_ERROR = -32700,
    LSP_INVALID_REQUEST = -32600,
    LSP_METHOD_NOT_FOUND = -32601,
    LSP_INVALID_PARAMS = -32602,
    LSP_INTERNAL_ERROR = -32603,
    LSP_SERVER_NOT_INITIALIZED = -32002
};

enum LSPInitializeResultErrorCode {
    LSP_REQUEST_CANCELLED = -32800
};

struct LSPMessage {
    size_t length;
    char *content;
};

struct LSPRequest lsp_parse_request(struct ArenaAllocator *allocator,
                                     struct JsonValue *json);

enum LSPMethod lsp_method_from_string(const char *method_name);

void lsp_send_response(struct ArenaAllocator *allocator,
                       struct LSPResponse *response);

struct LSPResponse lsp_handle_initialize(struct ArenaAllocator *allocator,
                                         struct LSPRequest *request);

struct LSPResponse lsp_handle_shutdown(struct ArenaAllocator *allocator,
                                       struct LSPRequest *request);

struct LSPResponse lsp_handle_hover(struct ArenaAllocator *allocator,
                                    struct LSPRequest *request);

struct LSPMessage lsp_read_message(void);

size_t lsp_write_message(char *buffer, size_t buffer_capacity,
                         const char *content, size_t content_length);

char *lsp_uri_to_path(const char *uri);

bool lsp_compile_and_check(const char *file_path, int *out_exit_code);

void lsp_send_publish_diagnostics(struct ArenaAllocator *allocator,
                                   const char *uri,
                                   struct LSPDiagnostic *diagnostics,
                                   size_t count);

char *lsp_extract_text_from_params(struct ArenaAllocator *allocator,
                                    struct JsonValue *params);

const char *lsp_extract_uri_from_params(struct ArenaAllocator *allocator,
                                         struct JsonValue *params);

#endif
