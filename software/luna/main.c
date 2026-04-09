#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "arena_allocator.h"
#include "compiler.h"

const char *VERSION = "0.1.4";

int main(int argc, char **argv) {
  puts("Luna Compiler");

  uint8_t arena[UINT16_MAX * 16];

  struct ArenaAllocator allocator = arena_make(&arena, sizeof(arena));

  if (argc < 2) {
    fprintf(stderr, "usage: %s <source.luna> ...\n", argv[0]);
    return 1;
  }

  if (strcmp(argv[1], "-v") == 0) {
    printf("version: %s\n", VERSION);
    return 0;
  }

  compile(&allocator, argv, argc, "out/compiled.ll");

  printf("Arena Allocator used %ld/%ld (%0.2f%%) memory\n", allocator.length,
         allocator.capacity,
         (double)allocator.length / (double)allocator.capacity * 100.0);
  return 0;
}
