#include <assert.h>
#include <stdlib.h>

#include "arena_allocator.h"
#include "lexer.h"
#include "luna_string.h"
#include "test.h"

struct Lexer setup_lexer(char *source) {
  char *arena = malloc(sizeof(char) * 10024);
  struct ArenaAllocator alloc = arena_make(arena, 10024);
  struct Lexer lexer = lexer_make(&alloc, string_make(source));
  return lexer;
}

void teardown_lexer(struct Lexer lexer) { free(lexer.allocator->arena); }

void test_lexer_make(void) {
  struct Lexer lexer = setup_lexer("foo");

  assert(lexer.position == 0);
  assert(lexer.source.length == 3);

  teardown_lexer(lexer);
}

void test_lexer_peek(void) {
  struct Lexer lexer = setup_lexer("foo");

  assert(lexer.position == 0);
  assert(lexer.source.length == 3);

  assert(lexer_peek(&lexer) == 'f');
  assert(lexer.position == 0);

  lexer.position++;
  assert(lexer_peek(&lexer) == 'o');
  assert(lexer.position == 1);

  teardown_lexer(lexer);
}

void test_lexer_next(void) {
  struct Lexer lexer = setup_lexer("2+3");

  assert(lexer.position == 0);
  assert(lexer.source.length == 3);

  struct Token out;
  assert(lexer_next(&lexer, &out));

  assert(out.type == T_INTEGER);
  assert(out.value.integer == 2);
  assert(lexer.position == 1);

  assert(lexer_next(&lexer, &out));

  assert(out.type == T_PLUS);
  assert(lexer.position == 2);

  assert(lexer_next(&lexer, &out));

  assert(out.type == T_INTEGER);
  assert(out.value.integer == 3);
  assert(lexer.position == 3);

  teardown_lexer(lexer);
}

void test_lexer(void) {
  run_test("lexer, it makes", &test_lexer_make);
  run_test("lexer, it peeks", &test_lexer_peek);
  run_test("lexer, it nexts", &test_lexer_next);
}
