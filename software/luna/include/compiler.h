#ifndef COMPILER_H
#define COMPILER_H

#include <stddef.h>

#include "arena_allocator.h"
#include "luna_string.h"

void compile(
  struct ArenaAllocator *allocator,
  char **source_file_paths,
             size_t source_file_count

);

#endif
