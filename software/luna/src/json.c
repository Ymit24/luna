#include "json.h"

#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const size_t JSON_MAX_DEPTH = 256;

static void skip_whitespace(char **cursor, char *end) {
    while (*cursor < end && isspace((unsigned char)**cursor)) {
        (*cursor)++;
    }
}

static bool parse_string(struct ArenaAllocator *allocator,
                         char **cursor, char *end,
                         struct JsonValue *out_value,
                         struct JsonError *out_error) {
    (void)allocator;

    assert(**cursor == '"');
    (*cursor)++;

    char *str_start = *cursor;
    size_t length = 0;

    while (*cursor < end) {
        char c = **cursor;

        if (c == '"') {
            break;
        } else if (c == '\\') {
            (*cursor)++;
            if (*cursor >= end) {
                out_error->message = "Unterminated string escape sequence";
                out_error->at_pointer = str_start;
                return false;
            }
            char escaped = **cursor;
            switch (escaped) {
                case '"':
                case '\\':
                case '/':
                case 'b':
                case 'f':
                case 'n':
                case 'r':
                case 't':
                    break;
                case 'u':
                    (*cursor)++;
                    for (int i = 0; i < 4; i++) {
                        if (*cursor >= end || !isxdigit((unsigned char)**cursor)) {
                            out_error->message = "Invalid unicode escape sequence";
                            out_error->at_pointer = *cursor;
                            return false;
                        }
                        (*cursor)++;
                    }
                    continue;
                default:
                    out_error->message = "Invalid escape character";
                    out_error->at_pointer = *cursor;
                    return false;
            }
        }

        (*cursor)++;
        length++;
    }

    if (*cursor >= end) {
        out_error->message = "Unterminated string";
        out_error->at_pointer = str_start;
        return false;
    }

    (*cursor)++;

    out_value->type = JSON_STRING;
    out_value->value.string_value.ptr = str_start;
    out_value->value.string_value.length = length;

    return true;
}

static bool parse_number(char **cursor, char *end,
                         struct JsonValue *out_value,
                         struct JsonError *out_error) {
    char *start = *cursor;

    if (**cursor == '-') {
        (*cursor)++;
    }

    if (*cursor >= end || !isdigit((unsigned char)**cursor)) {
        out_error->message = "Invalid number format";
        out_error->at_pointer = *cursor;
        return false;
    }

    if (**cursor == '0') {
        (*cursor)++;
    } else {
        while (*cursor < end && isdigit((unsigned char)**cursor)) {
            (*cursor)++;
        }
    }

    if (*cursor < end && **cursor == '.') {
        (*cursor)++;
        if (*cursor >= end || !isdigit((unsigned char)**cursor)) {
            out_error->message = "Invalid fractional number";
            out_error->at_pointer = *cursor;
            return false;
        }
        while (*cursor < end && isdigit((unsigned char)**cursor)) {
            (*cursor)++;
        }
    }

    if (*cursor < end && (**cursor == 'e' || **cursor == 'E')) {
        (*cursor)++;
        if (*cursor < end && (**cursor == '+' || **cursor == '-')) {
            (*cursor)++;
        }
        if (*cursor >= end || !isdigit((unsigned char)**cursor)) {
            out_error->message = "Invalid exponent";
            out_error->at_pointer = *cursor;
            return false;
        }
        while (*cursor < end && isdigit((unsigned char)**cursor)) {
            (*cursor)++;
        }
    }

    char *endptr;
    double value = strtod(start, &endptr);
    assert(endptr == *cursor);

    (void)value;

    out_value->type = JSON_NUMBER;
    out_value->value.number_value = value;

    return true;
}

static bool parse_value(struct ArenaAllocator *allocator,
                        char **cursor, char *end, size_t depth,
                        struct JsonValue *out_value,
                        struct JsonError *out_error);

static bool parse_array(struct ArenaAllocator *allocator,
                        char **cursor, char *end, size_t depth,
                        struct JsonValue *out_value,
                        struct JsonError *out_error) {
    assert(**cursor == '[');
    (*cursor)++;

    skip_whitespace(cursor, end);

    struct JsonArray *array = arena_alloc(allocator, sizeof(struct JsonArray));
    array->count = 0;
    array->values = NULL;

    if (*cursor >= end) {
        out_error->message = "Unterminated array";
        out_error->at_pointer = *cursor;
        return false;
    }

    if (**cursor == ']') {
        (*cursor)++;
        out_value->type = JSON_ARRAY;
        out_value->value.array_value = array;
        return true;
    }

    size_t capacity = 4;
    struct JsonValue *values = arena_alloc(allocator, sizeof(struct JsonValue) * capacity);
    size_t count = 0;

    while (*cursor < end) {
        if (count >= capacity) {
            capacity *= 2;
            struct JsonValue *new_values = arena_alloc(allocator, sizeof(struct JsonValue) * capacity);
            for (size_t i = 0; i < count; i++) {
                new_values[i] = values[i];
            }
            values = new_values;
        }

        skip_whitespace(cursor, end);

        if (!parse_value(allocator, cursor, end, depth + 1, &values[count], out_error)) {
            return false;
        }
        count++;

        skip_whitespace(cursor, end);

        if (*cursor >= end) {
            out_error->message = "Unterminated array";
            out_error->at_pointer = *cursor;
            return false;
        }

        if (**cursor == ',') {
            (*cursor)++;
            continue;
        } else if (**cursor == ']') {
            (*cursor)++;
            break;
        } else {
            out_error->message = "Expected ',' or ']' in array";
            out_error->at_pointer = *cursor;
            return false;
        }
    }

    array->values = values;
    array->count = count;

    out_value->type = JSON_ARRAY;
    out_value->value.array_value = array;

    return true;
}

static bool parse_object(struct ArenaAllocator *allocator,
                         char **cursor, char *end, size_t depth,
                         struct JsonValue *out_value,
                         struct JsonError *out_error) {
    assert(**cursor == '{');
    (*cursor)++;

    skip_whitespace(cursor, end);

    struct JsonObject *object = arena_alloc(allocator, sizeof(struct JsonObject));
    object->count = 0;
    object->entries = NULL;

    if (*cursor >= end) {
        out_error->message = "Unterminated object";
        out_error->at_pointer = *cursor;
        return false;
    }

    if (**cursor == '}') {
        (*cursor)++;
        out_value->type = JSON_OBJECT;
        out_value->value.object_value = object;
        return true;
    }

    size_t capacity = 4;
    struct JsonObjectEntry *entries = arena_alloc(allocator, sizeof(struct JsonObjectEntry) * capacity);
    size_t count = 0;

    while (*cursor < end) {
        skip_whitespace(cursor, end);

        if (*cursor >= end || **cursor != '"') {
            out_error->message = "Expected object key";
            out_error->at_pointer = *cursor;
            return false;
        }

        if (count >= capacity) {
            capacity *= 2;
            struct JsonObjectEntry *new_entries = arena_alloc(allocator, sizeof(struct JsonObjectEntry) * capacity);
            for (size_t i = 0; i < count; i++) {
                new_entries[i] = entries[i];
            }
            entries = new_entries;
        }

        struct JsonObjectEntry *entry = &entries[count];

        (*cursor)++;
        char *key_ptr = *cursor;
        size_t key_length = 0;

        while (*cursor < end) {
            char c = **cursor;
            if (c == '"') {
                break;
            } else if (c == '\\') {
                (*cursor)++;
                if (*cursor >= end) {
                    out_error->message = "Unterminated key escape sequence";
                    out_error->at_pointer = key_ptr;
                    return false;
                }
                (*cursor)++;
            } else {
                (*cursor)++;
                key_length++;
            }
        }

        if (*cursor >= end) {
            out_error->message = "Unterminated object key";
            out_error->at_pointer = key_ptr;
            return false;
        }

        (*cursor)++;

        entry->key_ptr = key_ptr;
        entry->key_length = key_length;

        skip_whitespace(cursor, end);

        if (*cursor >= end || **cursor != ':') {
            out_error->message = "Expected ':' after object key";
            out_error->at_pointer = *cursor;
            return false;
        }
        (*cursor)++;

        skip_whitespace(cursor, end);

        if (!parse_value(allocator, cursor, end, depth + 1, &entry->value, out_error)) {
            return false;
        }
        count++;

        skip_whitespace(cursor, end);

        if (*cursor >= end) {
            out_error->message = "Unterminated object";
            out_error->at_pointer = *cursor;
            return false;
        }

        if (**cursor == ',') {
            (*cursor)++;
            continue;
        } else if (**cursor == '}') {
            (*cursor)++;
            break;
        } else {
            out_error->message = "Expected ',' or '}' in object";
            out_error->at_pointer = *cursor;
            return false;
        }
    }

    object->entries = entries;
    object->count = count;

    out_value->type = JSON_OBJECT;
    out_value->value.object_value = object;

    return true;
}

static bool parse_value(struct ArenaAllocator *allocator,
                        char **cursor, char *end, size_t depth,
                        struct JsonValue *out_value,
                        struct JsonError *out_error) {
    if (depth > JSON_MAX_DEPTH) {
        out_error->message = "Maximum nesting depth exceeded";
        out_error->at_pointer = *cursor;
        return false;
    }

    skip_whitespace(cursor, end);

    if (*cursor >= end) {
        out_error->message = "Unexpected end of input";
        out_error->at_pointer = *cursor;
        return false;
    }

    switch (**cursor) {
        case 'n':
            if (end - *cursor >= 4 && memcmp(*cursor, "null", 4) == 0) {
                *cursor += 4;
                out_value->type = JSON_NULL;
                return true;
            }
            out_error->message = "Invalid token";
            out_error->at_pointer = *cursor;
            return false;

        case 't':
            if (end - *cursor >= 4 && memcmp(*cursor, "true", 4) == 0) {
                *cursor += 4;
                out_value->type = JSON_BOOL;
                out_value->value.bool_value = true;
                return true;
            }
            out_error->message = "Invalid token";
            out_error->at_pointer = *cursor;
            return false;

        case 'f':
            if (end - *cursor >= 5 && memcmp(*cursor, "false", 5) == 0) {
                *cursor += 5;
                out_value->type = JSON_BOOL;
                out_value->value.bool_value = false;
                return true;
            }
            out_error->message = "Invalid token";
            out_error->at_pointer = *cursor;
            return false;

        case '"':
            return parse_string(allocator, cursor, end, out_value, out_error);

        case '[':
            return parse_array(allocator, cursor, end, depth, out_value, out_error);

        case '{':
            return parse_object(allocator, cursor, end, depth, out_value, out_error);

        default:
            if (**cursor == '-' || isdigit((unsigned char)**cursor)) {
                return parse_number(cursor, end, out_value, out_error);
            }
            out_error->message = "Invalid JSON value";
            out_error->at_pointer = *cursor;
            return false;
    }
}

bool json_parse(struct ArenaAllocator *allocator,
                char *source,
                size_t source_length,
                struct JsonValue *out_value,
                struct JsonError *out_error) {
    char *cursor = source;
    char *end = source + source_length;

    struct JsonError error = {0};
    if (out_error) {
        *out_error = error;
    }

    bool result = parse_value(allocator, &cursor, end, 0, out_value, out_error);

    if (result) {
        skip_whitespace(&cursor, end);
        if (cursor < end) {
            if (out_error) {
                out_error->message = "Unexpected trailing characters";
                out_error->at_pointer = cursor;
            }
            return false;
        }
    }

    return result;
}

static size_t encode_string(char *buffer, size_t capacity,
                            const char *ptr, size_t length) {
    size_t written = 0;

    if (buffer && capacity > 0) {
        buffer[written] = '"';
        written++;
    } else {
        written++;
    }

    for (size_t i = 0; i < length; i++) {
        char c = ptr[i];
        const char *escape = NULL;

        switch (c) {
            case '"':
                escape = "\\\"";
                break;
            case '\\':
                escape = "\\\\";
                break;
            case '\b':
                escape = "\\b";
                break;
            case '\f':
                escape = "\\f";
                break;
            case '\n':
                escape = "\\n";
                break;
            case '\r':
                escape = "\\r";
                break;
            case '\t':
                escape = "\\t";
                break;
            default:
                if (c < 0x20) {
                    char esc_buf[7];
                    int esc_len = snprintf(esc_buf, sizeof(esc_buf), "\\u%04x", (unsigned char)c);
                    escape = esc_buf;
                    (void)esc_len;
                }
                break;
        }

        if (escape) {
            size_t esc_len = strlen(escape);
            if (buffer && written + esc_len < capacity) {
                memcpy(buffer + written, escape, esc_len);
            }
            written += esc_len;
        } else {
            if (buffer && written < capacity) {
                buffer[written] = c;
            }
            written++;
        }
    }

    if (buffer && capacity > 0 && written < capacity) {
        buffer[written] = '"';
        written++;
    } else {
        written++;
    }

    return written;
}

static size_t encode_value(struct ArenaAllocator *allocator,
                           struct JsonValue *value,
                           char *buffer, size_t capacity,
                           bool minified);

static size_t encode_array(struct ArenaAllocator *allocator,
                           struct JsonArray *array,
                           char *buffer, size_t capacity,
                           bool minified) {
    size_t written = 0;

    if (buffer && capacity > 0) {
        buffer[written] = '[';
        written++;
    } else {
        written++;
    }

    for (size_t i = 0; i < array->count; i++) {
        if ((!minified && i > 0) || (minified && i > 0)) {
            if (buffer && written < capacity) {
                buffer[written] = ',';
                written++;
            } else {
                written++;
            }
        }

        if (!minified) {
            if (buffer && written < capacity) {
                buffer[written] = ' ';
                written++;
            } else {
                written++;
            }
        }

        size_t elem_written = encode_value(allocator, &array->values[i], NULL, 0, minified);
        if (buffer && written + elem_written < capacity) {
            encode_value(allocator, &array->values[i], buffer + written, capacity - written, minified);
        }
        written += elem_written;
    }

    if (!minified) {
        if (array->count > 0) {
            if (buffer && written < capacity) {
                buffer[written] = ' ';
                written++;
            } else {
                written++;
            }
        }
    }

    if (buffer && capacity > 0 && written < capacity) {
        buffer[written] = ']';
        written++;
    } else {
        written++;
    }

    return written;
}

static size_t encode_object(struct ArenaAllocator *allocator,
                            struct JsonObject *object,
                            char *buffer, size_t capacity,
                            bool minified) {
    size_t written = 0;

    if (buffer && capacity > 0) {
        buffer[written] = '{';
        written++;
    } else {
        written++;
    }

    for (size_t i = 0; i < object->count; i++) {
        if (i > 0) {
            if (buffer && written < capacity) {
                buffer[written] = ',';
                written++;
            } else {
                written++;
            }
        }

        if (!minified) {
            if (buffer && written < capacity) {
                buffer[written] = ' ';
                written++;
            } else {
                written++;
            }
        }

        size_t key_written = encode_string(NULL, 0, object->entries[i].key_ptr, object->entries[i].key_length);
        if (buffer && written + key_written + 1 <= capacity) {
            encode_string(buffer + written, capacity - written, object->entries[i].key_ptr, object->entries[i].key_length);
        }
        written += key_written;

        if (buffer && written < capacity) {
            buffer[written] = ':';
            written++;
        } else {
            written++;
        }

        if (!minified) {
            if (buffer && written < capacity) {
                buffer[written] = ' ';
                written++;
            } else {
                written++;
            }
        }

        size_t val_written = encode_value(allocator, &object->entries[i].value, NULL, 0, minified);
        if (buffer && written + val_written <= capacity) {
            encode_value(allocator, &object->entries[i].value, buffer + written, capacity - written, minified);
        }
        written += val_written;
    }

    if (!minified) {
        if (object->count > 0) {
            if (buffer && written < capacity) {
                buffer[written] = ' ';
                written++;
            } else {
                written++;
            }
        }
    }

    if (buffer && capacity > 0 && written < capacity) {
        buffer[written] = '}';
        written++;
    } else {
        written++;
    }

    return written;
}

static size_t encode_value(struct ArenaAllocator *allocator,
                           struct JsonValue *value,
                           char *buffer, size_t capacity,
                           bool minified) {
    (void)allocator;

    switch (value->type) {
        case JSON_NULL:
            if (buffer && capacity >= 4) {
                memcpy(buffer, "null", 4);
            }
            return 4;

        case JSON_BOOL:
            if (value->value.bool_value) {
                if (buffer && capacity >= 4) {
                    memcpy(buffer, "true", 4);
                }
                return 4;
            } else {
                if (buffer && capacity >= 5) {
                    memcpy(buffer, "false", 5);
                }
                return 5;
            }

        case JSON_NUMBER:
            if (buffer && capacity > 0) {
                int written = snprintf(buffer, capacity, "%.17g", value->value.number_value);
                if (written < 0) {
                    return 0;
                }
                return (size_t)written;
            } else {
                char temp[32];
                int written = snprintf(temp, sizeof(temp), "%.17g", value->value.number_value);
                return (size_t)(written > 0 ? written : 0);
            }

        case JSON_STRING:
            return encode_string(buffer, capacity,
                                 value->value.string_value.ptr,
                                 value->value.string_value.length);

        case JSON_ARRAY:
            return encode_array(allocator, value->value.array_value, buffer, capacity, minified);

        case JSON_OBJECT:
            return encode_object(allocator, value->value.object_value, buffer, capacity, minified);
    }

    return 0;
}

size_t json_encode(struct ArenaAllocator *allocator,
                   struct JsonValue *value,
                   char *buffer,
                   size_t buffer_capacity) {
    return encode_value(allocator, value, buffer, buffer_capacity, false);
}

size_t json_encode_minified(struct ArenaAllocator *allocator,
                            struct JsonValue *value,
                            char *buffer,
                            size_t buffer_capacity) {
    return encode_value(allocator, value, buffer, buffer_capacity, true);
}

void json_value_free(struct JsonValue *value, struct ArenaAllocator *allocator) {
    (void)allocator;

    if (!value) {
        return;
    }

    switch (value->type) {
        case JSON_ARRAY:
            if (value->value.array_value) {
                for (size_t i = 0; i < value->value.array_value->count; i++) {
                    json_value_free(&value->value.array_value->values[i], allocator);
                }
            }
            break;

        case JSON_OBJECT:
            if (value->value.object_value) {
                for (size_t i = 0; i < value->value.object_value->count; i++) {
                    json_value_free(&value->value.object_value->entries[i].value, allocator);
                }
            }
            break;

        default:
            break;
    }
}
