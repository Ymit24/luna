#ifndef ARENA_ALLOCATOR_H
#define ARENA_ALLOCATOR_H

#include <stddef.h>
#include <stdint.h>

struct ArenaAllocator {
  void *arena;
  uint16_t length;
  uint16_t capacity;
};

struct ArenaAllocator arena_make(void *arena, uint16_t capacity);
void *arena_alloc(struct ArenaAllocator *allocator, size_t size);

#endif
