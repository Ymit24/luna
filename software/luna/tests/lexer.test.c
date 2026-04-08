#include <assert.h>
#include <stdlib.h>

#include "lexer.h"
#include "arena_allocator.h"
#include "luna_string.h"
#include "test.h"

struct Lexer setup_lexer(char* source) {
  char *arena = malloc(sizeof(char) * 1024);
  struct ArenaAllocator alloc = arena_make(arena, 1024);
  struct Lexer lexer = lexer_make(&alloc, string_make(source));
  return lexer;
}

void teardown_lexer(struct Lexer lexer) {
  free(lexer.allocator->arena);
}

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

  teardown_lexer(lexer);
}

void test_lexer(void) {
  run_test("lexer, it makes", &test_lexer_make);
  run_test("lexer, it peeks", &test_lexer_peek);
}
