#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arena_allocator.h"
#include "json.h"

static void test_null(void) {
    uint8_t arena_buffer[4096];
    struct ArenaAllocator allocator = arena_make(arena_buffer, sizeof(arena_buffer));

    char *source = "null";
    struct JsonValue value;
    struct JsonError error;

    bool result = json_parse(&allocator, source, strlen(source), &value, &error);

    assert(result == true);
    assert(value.type == JSON_NULL);

    char output[16];
    size_t written = json_encode(&allocator, &value, output, sizeof(output));
    assert(written == 4);
    assert(memcmp(output, "null", 4) == 0);

    printf("test_null: PASSED\n");
}

static void test_true(void) {
    uint8_t arena_buffer[4096];
    struct ArenaAllocator allocator = arena_make(arena_buffer, sizeof(arena_buffer));

    char *source = "true";
    struct JsonValue value;
    struct JsonError error;

    bool result = json_parse(&allocator, source, strlen(source), &value, &error);

    assert(result == true);
    assert(value.type == JSON_BOOL);
    assert(value.value.bool_value == true);

    char output[16];
    size_t written = json_encode(&allocator, &value, output, sizeof(output));
    assert(written == 4);
    assert(memcmp(output, "true", 4) == 0);

    printf("test_true: PASSED\n");
}

static void test_false(void) {
    uint8_t arena_buffer[4096];
    struct ArenaAllocator allocator = arena_make(arena_buffer, sizeof(arena_buffer));

    char *source = "false";
    struct JsonValue value;
    struct JsonError error;

    bool result = json_parse(&allocator, source, strlen(source), &value, &error);

    assert(result == true);
    assert(value.type == JSON_BOOL);
    assert(value.value.bool_value == false);

    char output[16];
    size_t written = json_encode(&allocator, &value, output, sizeof(output));
    assert(written == 5);
    assert(memcmp(output, "false", 5) == 0);

    printf("test_false: PASSED\n");
}

static void test_number_integer(void) {
    uint8_t arena_buffer[4096];
    struct ArenaAllocator allocator = arena_make(arena_buffer, sizeof(arena_buffer));

    char *source = "42";
    struct JsonValue value;
    struct JsonError error;

    bool result = json_parse(&allocator, source, strlen(source), &value, &error);

    assert(result == true);
    assert(value.type == JSON_NUMBER);
    assert(value.value.number_value == 42.0);

    char output[32];
    size_t written = json_encode(&allocator, &value, output, sizeof(output));
    assert(written > 0);

    printf("test_number_integer: PASSED\n");
}

static void test_number_negative(void) {
    uint8_t arena_buffer[4096];
    struct ArenaAllocator allocator = arena_make(arena_buffer, sizeof(arena_buffer));

    char *source = "-123.456";
    struct JsonValue value;
    struct JsonError error;

    bool result = json_parse(&allocator, source, strlen(source), &value, &error);

    assert(result == true);
    assert(value.type == JSON_NUMBER);
    assert(value.value.number_value == -123.456);

    printf("test_number_negative: PASSED\n");
}

static void test_number_scientific(void) {
    uint8_t arena_buffer[4096];
    struct ArenaAllocator allocator = arena_make(arena_buffer, sizeof(arena_buffer));

    char *source = "1.5e10";
    struct JsonValue value;
    struct JsonError error;

    bool result = json_parse(&allocator, source, strlen(source), &value, &error);

    assert(result == true);
    assert(value.type == JSON_NUMBER);
    assert(value.value.number_value == 1.5e10);

    char *source2 = "2.5E-5";
    struct JsonValue value2;
    bool result2 = json_parse(&allocator, source2, strlen(source2), &value2, &error);

    assert(result2 == true);
    assert(value2.type == JSON_NUMBER);
    assert(value2.value.number_value == 2.5e-5);

    printf("test_number_scientific: PASSED\n");
}

static void test_string(void) {
    uint8_t arena_buffer[4096];
    struct ArenaAllocator allocator = arena_make(arena_buffer, sizeof(arena_buffer));

    char *source = "\"hello world\"";
    struct JsonValue value;
    struct JsonError error;

    bool result = json_parse(&allocator, source, strlen(source), &value, &error);

    assert(result == true);
    assert(value.type == JSON_STRING);
    assert(value.value.string_value.length == 11);
    assert(memcmp(value.value.string_value.ptr, "hello world", 11) == 0);

    char output[32];
    size_t written = json_encode(&allocator, &value, output, sizeof(output));
    assert(written == 13);
    assert(memcmp(output, source, strlen(source)) == 0);

    printf("test_string: PASSED\n");
}

static void test_string_escapes(void) {
    uint8_t arena_buffer[4096];
    struct ArenaAllocator allocator = arena_make(arena_buffer, sizeof(arena_buffer));

    char *source = "\"hello\\nworld\\t\\\\\\\"test\\b\\f\\r\\/\"";
    struct JsonValue value;
    struct JsonError error;

    bool result = json_parse(&allocator, source, strlen(source), &value, &error);

    assert(result == true);
    assert(value.type == JSON_STRING);

    char output[64];
    size_t written = json_encode(&allocator, &value, output, sizeof(output));
    assert(written > 0);

    printf("test_string_escapes: PASSED\n");
}

static void test_empty_array(void) {
    uint8_t arena_buffer[4096];
    struct ArenaAllocator allocator = arena_make(arena_buffer, sizeof(arena_buffer));

    char *source = "[]";
    struct JsonValue value;
    struct JsonError error;

    bool result = json_parse(&allocator, source, strlen(source), &value, &error);

    assert(result == true);
    assert(value.type == JSON_ARRAY);
    assert(value.value.array_value->count == 0);

    char output[16];
    size_t written = json_encode(&allocator, &value, output, sizeof(output));
    assert(written == 2);
    assert(memcmp(output, "[]", 2) == 0);

    printf("test_empty_array: PASSED\n");
}

static void test_array_with_values(void) {
    uint8_t arena_buffer[4096];
    struct ArenaAllocator allocator = arena_make(arena_buffer, sizeof(arena_buffer));

    char *source = "[1, \"two\", true, null]";
    struct JsonValue value;
    struct JsonError error;

    bool result = json_parse(&allocator, source, strlen(source), &value, &error);

    assert(result == true);
    assert(value.type == JSON_ARRAY);
    assert(value.value.array_value->count == 4);
    assert(value.value.array_value->values[0].type == JSON_NUMBER);
    assert(value.value.array_value->values[0].value.number_value == 1.0);
    assert(value.value.array_value->values[1].type == JSON_STRING);
    assert(value.value.array_value->values[1].value.string_value.length == 3);
    assert(memcmp(value.value.array_value->values[1].value.string_value.ptr, "two", 3) == 0);
    assert(value.value.array_value->values[2].type == JSON_BOOL);
    assert(value.value.array_value->values[2].value.bool_value == true);
    assert(value.value.array_value->values[3].type == JSON_NULL);

    char output[64];
    size_t written = json_encode(&allocator, &value, output, sizeof(output));
    assert(written > 0);

    printf("test_array_with_values: PASSED\n");
}

static void test_nested_array(void) {
    uint8_t arena_buffer[4096];
    struct ArenaAllocator allocator = arena_make(arena_buffer, sizeof(arena_buffer));

    char *source = "[[1, 2], [3, 4], [5, 6]]";
    struct JsonValue value;
    struct JsonError error;

    bool result = json_parse(&allocator, source, strlen(source), &value, &error);

    assert(result == true);
    assert(value.type == JSON_ARRAY);
    assert(value.value.array_value->count == 3);

    for (size_t i = 0; i < 3; i++) {
        assert(value.value.array_value->values[i].type == JSON_ARRAY);
        assert(value.value.array_value->values[i].value.array_value->count == 2);
    }

    assert(value.value.array_value->values[0].value.array_value->values[0].value.number_value == 1.0);
    assert(value.value.array_value->values[2].value.array_value->values[1].value.number_value == 6.0);

    printf("test_nested_array: PASSED\n");
}

static void test_empty_object(void) {
    uint8_t arena_buffer[4096];
    struct ArenaAllocator allocator = arena_make(arena_buffer, sizeof(arena_buffer));

    char *source = "{}";
    struct JsonValue value;
    struct JsonError error;

    bool result = json_parse(&allocator, source, strlen(source), &value, &error);

    assert(result == true);
    assert(value.type == JSON_OBJECT);
    assert(value.value.object_value->count == 0);

    char output[16];
    size_t written = json_encode(&allocator, &value, output, sizeof(output));
    assert(written == 2);
    assert(memcmp(output, "{}", 2) == 0);

    printf("test_empty_object: PASSED\n");
}

static void test_object_with_values(void) {
    uint8_t arena_buffer[4096];
    struct ArenaAllocator allocator = arena_make(arena_buffer, sizeof(arena_buffer));

    char *source = "{\"name\": \"John\", \"age\": 30, \"active\": true}";
    struct JsonValue value;
    struct JsonError error;

    bool result = json_parse(&allocator, source, strlen(source), &value, &error);

    assert(result == true);
    assert(value.type == JSON_OBJECT);
    assert(value.value.object_value->count == 3);

    struct JsonObjectEntry *name_entry = &value.value.object_value->entries[0];
    assert(name_entry->key_length == 4);
    assert(memcmp(name_entry->key_ptr, "name", 4) == 0);
    assert(name_entry->value.type == JSON_STRING);
    assert(name_entry->value.value.string_value.length == 4);
    assert(memcmp(name_entry->value.value.string_value.ptr, "John", 4) == 0);

    struct JsonObjectEntry *age_entry = &value.value.object_value->entries[1];
    assert(age_entry->key_length == 3);
    assert(memcmp(age_entry->key_ptr, "age", 3) == 0);
    assert(age_entry->value.type == JSON_NUMBER);
    assert(age_entry->value.value.number_value == 30.0);

    struct JsonObjectEntry *active_entry = &value.value.object_value->entries[2];
    assert(active_entry->key_length == 6);
    assert(memcmp(active_entry->key_ptr, "active", 6) == 0);
    assert(active_entry->value.type == JSON_BOOL);
    assert(active_entry->value.value.bool_value == true);

    char output[128];
    size_t written = json_encode(&allocator, &value, output, sizeof(output));
    assert(written > 0);

    printf("test_object_with_values: PASSED\n");
}

static void test_nested_object(void) {
    uint8_t arena_buffer[4096];
    struct ArenaAllocator allocator = arena_make(arena_buffer, sizeof(arena_buffer));

    char *source = "{\"outer\": {\"inner\": {\"value\": 42}}}";
    struct JsonValue value;
    struct JsonError error;

    bool result = json_parse(&allocator, source, strlen(source), &value, &error);

    assert(result == true);
    assert(value.type == JSON_OBJECT);
    assert(value.value.object_value->count == 1);

    struct JsonValue *outer = &value.value.object_value->entries[0].value;
    assert(outer->type == JSON_OBJECT);
    assert(outer->value.object_value->count == 1);

    struct JsonValue *inner = &outer->value.object_value->entries[0].value;
    assert(inner->type == JSON_OBJECT);
    assert(inner->value.object_value->count == 1);

    struct JsonValue *value_val = &inner->value.object_value->entries[0].value;
    assert(value_val->type == JSON_NUMBER);
    assert(value_val->value.number_value == 42.0);

    printf("test_nested_object: PASSED\n");
}

static void test_complex_structure(void) {
    uint8_t arena_buffer[16384];
    struct ArenaAllocator allocator = arena_make(arena_buffer, sizeof(arena_buffer));

    char *source =
        "{"
        "\"users\": ["
        "  {\"name\": \"Alice\", \"scores\": [95, 87, 92]}, "
        "  {\"name\": \"Bob\", \"scores\": [78, 85, 80]}"
        "], "
        "\"metadata\": {\"version\": 1, \"timestamp\": \"2024-01-01\"}"
        "}";

    struct JsonValue value;
    struct JsonError error;

    bool result = json_parse(&allocator, source, strlen(source), &value, &error);

    assert(result == true);
    assert(value.type == JSON_OBJECT);
    assert(value.value.object_value->count == 2);

    char output[1024];
    size_t written = json_encode(&allocator, &value, output, sizeof(output));
    assert(written > 0);
    assert(written < sizeof(output));

    printf("test_complex_structure: PASSED\n");
}

static void test_roundtrip(void) {
    uint8_t arena_buffer[16384];
    struct ArenaAllocator allocator = arena_make(arena_buffer, sizeof(arena_buffer));

    char *source = "{\"array\": [1, 2, 3], \"nested\": {\"a\": true, \"b\": null}}";

    struct JsonValue value1;
    struct JsonError error;

    bool result1 = json_parse(&allocator, source, strlen(source), &value1, &error);
    assert(result1 == true);

    char encoded[1024];
    size_t encoded_len = json_encode(&allocator, &value1, encoded, sizeof(encoded));
    assert(encoded_len > 0);
    assert(encoded_len < sizeof(encoded));

    struct JsonValue value2;
    bool result2 = json_parse(&allocator, encoded, encoded_len, &value2, &error);
    assert(result2 == true);

    assert(value1.type == value2.type);

    if (value1.type == JSON_OBJECT) {
        assert(value1.value.object_value->count == value2.value.object_value->count);
    }

    char encoded2[1024];
    size_t encoded2_len = json_encode(&allocator, &value2, encoded2, sizeof(encoded2));
    assert(encoded_len == encoded2_len);
    assert(memcmp(encoded, encoded2, encoded_len) == 0);

    printf("test_roundtrip: PASSED\n");
}

static void test_minified_encoding(void) {
    uint8_t arena_buffer[4096];
    struct ArenaAllocator allocator = arena_make(arena_buffer, sizeof(arena_buffer));

    char *source = "{\n  \"name\": \"test\",\n  \"value\": 42\n}";
    struct JsonValue value;
    struct JsonError error;

    bool result = json_parse(&allocator, source, strlen(source), &value, &error);
    assert(result == true);

    char pretty[128];
    size_t pretty_len = json_encode(&allocator, &value, pretty, sizeof(pretty));
    assert(pretty_len > 0);

    char minified[128];
    size_t minified_len = json_encode_minified(&allocator, &value, minified, sizeof(minified));
    assert(minified_len > 0);
    assert(minified_len < pretty_len);

    printf("test_minified_encoding: PASSED\n");
}

static void test_error_unterminated_string(void) {
    uint8_t arena_buffer[4096];
    struct ArenaAllocator allocator = arena_make(arena_buffer, sizeof(arena_buffer));

    char *source = "\"hello";
    struct JsonValue value;
    struct JsonError error;

    bool result = json_parse(&allocator, source, strlen(source), &value, &error);

    assert(result == false);
    assert(error.message != NULL);

    printf("test_error_unterminated_string: PASSED\n");
}

static void test_error_invalid_json(void) {
    uint8_t arena_buffer[4096];
    struct ArenaAllocator allocator = arena_make(arena_buffer, sizeof(arena_buffer));

    char *source = "not valid json";
    struct JsonValue value;
    struct JsonError error;

    bool result = json_parse(&allocator, source, strlen(source), &value, &error);

    assert(result == false);
    assert(error.message != NULL);

    printf("test_error_invalid_json: PASSED\n");
}

static void test_error_trailing_chars(void) {
    uint8_t arena_buffer[4096];
    struct ArenaAllocator allocator = arena_make(arena_buffer, sizeof(arena_buffer));

    char *source = "42 extra stuff";
    struct JsonValue value;
    struct JsonError error;

    bool result = json_parse(&allocator, source, strlen(source), &value, &error);

    assert(result == false);
    assert(error.message != NULL);

    printf("test_error_trailing_chars: PASSED\n");
}

static void test_error_missing_colon(void) {
    uint8_t arena_buffer[4096];
    struct ArenaAllocator allocator = arena_make(arena_buffer, sizeof(arena_buffer));

    char *source = "{\"key\" value}";
    struct JsonValue value;
    struct JsonError error;

    bool result = json_parse(&allocator, source, strlen(source), &value, &error);

    assert(result == false);

    printf("test_error_missing_colon: PASSED\n");
}

static void test_whitespace_handling(void) {
    uint8_t arena_buffer[4096];
    struct ArenaAllocator allocator = arena_make(arena_buffer, sizeof(arena_buffer));

    char *source = "   true   ";
    struct JsonValue value;
    struct JsonError error;

    bool result = json_parse(&allocator, source, strlen(source), &value, &error);

    assert(result == true);
    assert(value.type == JSON_BOOL);
    assert(value.value.bool_value == true);

    printf("test_whitespace_handling: PASSED\n");
}

static void test_string_into_source(void) {
    uint8_t arena_buffer[4096];
    struct ArenaAllocator allocator = arena_make(arena_buffer, sizeof(arena_buffer));

    char source[] = "{\"key\": \"value\"}";
    size_t source_len = strlen(source);

    struct JsonValue value;
    struct JsonError error;

    bool result = json_parse(&allocator, source, source_len, &value, &error);
    assert(result == true);
    assert(value.type == JSON_OBJECT);
    assert(value.value.object_value->count == 1);

    struct JsonObjectEntry *entry = &value.value.object_value->entries[0];
    assert(entry->value.type == JSON_STRING);

    assert(entry->value.value.string_value.ptr >= source);
    assert(entry->value.value.string_value.ptr < source + source_len);
    assert(entry->value.value.string_value.length == 5);
    assert(memcmp(entry->value.value.string_value.ptr, "value", 5) == 0);

    printf("test_string_into_source: PASSED\n");
}

static void test_all_types_together(void) {
    uint8_t arena_buffer[16384];
    struct ArenaAllocator allocator = arena_make(arena_buffer, sizeof(arena_buffer));

    char *source =
        "{"
        "\"null_val\": null, "
        "\"bool_true\": true, "
        "\"bool_false\": false, "
        "\"number_int\": 42, "
        "\"number_float\": -3.14, "
        "\"string\": \"hello\", "
        "\"array\": [1, 2, 3], "
        "\"object\": {\"nested\": \"value\"}"
        "}";

    struct JsonValue value;
    struct JsonError error;

    bool result = json_parse(&allocator, source, strlen(source), &value, &error);

    assert(result == true);
    assert(value.type == JSON_OBJECT);
    assert(value.value.object_value->count == 8);

    char output[1024];
    size_t written = json_encode(&allocator, &value, output, sizeof(output));
    assert(written > 0);

    printf("test_all_types_together: PASSED\n");
}

int main(void) {
    printf("Running JSON library tests...\n\n");

    test_null();
    test_true();
    test_false();
    test_number_integer();
    test_number_negative();
    test_number_scientific();
    test_string();
    test_string_escapes();
    test_empty_array();
    test_array_with_values();
    test_nested_array();
    test_empty_object();
    test_object_with_values();
    test_nested_object();
    test_complex_structure();
    test_roundtrip();
    test_minified_encoding();
    test_error_unterminated_string();
    test_error_invalid_json();
    test_error_trailing_chars();
    test_error_missing_colon();
    test_whitespace_handling();
    test_string_into_source();
    test_all_types_together();

    printf("\nAll tests passed!\n");
    return 0;
}
