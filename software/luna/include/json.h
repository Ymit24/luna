#ifndef JSON_H
#define JSON_H

#include <stdbool.h>
#include <stddef.h>

#include "arena_allocator.h"

enum JsonType {
    JSON_NULL,
    JSON_BOOL,
    JSON_NUMBER,
    JSON_STRING,
    JSON_ARRAY,
    JSON_OBJECT
};

struct JsonValue {
    enum JsonType type;
    union {
        bool bool_value;
        double number_value;
        struct {
            char *ptr;
            size_t length;
        } string_value;
        struct JsonArray *array_value;
        struct JsonObject *object_value;
    } value;
};

struct JsonArray {
    struct JsonValue *values;
    size_t count;
};

struct JsonObjectEntry {
    char *key_ptr;
    size_t key_length;
    struct JsonValue value;
};

struct JsonObject {
    struct JsonObjectEntry *entries;
    size_t count;
};

struct JsonError {
    char *message;
    size_t line;
    size_t column;
    char *at_pointer;
};

bool json_parse(struct ArenaAllocator *allocator,
                char *source,
                size_t source_length,
                struct JsonValue *out_value,
                struct JsonError *out_error);

size_t json_encode(struct ArenaAllocator *allocator,
                   struct JsonValue *value,
                   char *buffer,
                   size_t buffer_capacity);

size_t json_encode_minified(struct ArenaAllocator *allocator,
                            struct JsonValue *value,
                            char *buffer,
                            size_t buffer_capacity);

void json_value_free(struct JsonValue *value, struct ArenaAllocator *allocator);

#endif
