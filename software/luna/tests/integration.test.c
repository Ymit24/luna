#include "arena_allocator.h"
#include "test.h"
#include <stdio.h>

struct ModuleStatementNode *parse_source_file(struct ArenaAllocator *allocator,
                                              char *source_file);

void run_int_test(void) {
  char *source = "const printf = fn@extern[\" printf\"]@variadic(fmt: *i8): i32"
                 "const main = fn() {"
                 "  printf(\"test\")"
                 "};";

  char slot[10024] = {0};

  struct ArenaAllocator allocator = arena_make(slot, sizeof(slot));

  struct ModuleStatementNode *node = parse_source_file(&allocator, source);
}

int main(void) {
  print_results();
  return 0;
}
