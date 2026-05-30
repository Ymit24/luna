#include "arena_allocator.h"
#include "compiler.h"
#include "test.h"
#include <stdio.h>

struct ModuleStatementNode *parse_source_file(struct ArenaAllocator *allocator,
                                              char *source_file);

void run_integration_test(void) {
  char slot[UINT16_MAX * 16] = {0};
  struct ArenaAllocator allocator = arena_make(slot, sizeof(slot));

  char *source_file_paths[] = {"examples/test.luna"};
  compile(&allocator, source_file_paths, 1, "out/tmp/int_test");
}

int main(void) {
  run_test("basic", &run_integration_test);
  print_results();
  return 0;
}
