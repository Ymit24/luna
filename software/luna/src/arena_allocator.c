#include "arena_allocator.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>

struct ArenaAllocator arena_make(void *arena, size_t capacity) {
  return (struct ArenaAllocator){
      .arena = arena,
      .capacity = capacity,
      .length = 0,
  };
}

void *arena_alloc(struct ArenaAllocator *allocator, size_t size) {
  printf("[arena_alloc] allocating %zu bytes. Current len/cap = %zu/%zu. %lu\n",
         size, allocator->length, allocator->capacity,
         sizeof(struct ArenaAllocator));
  assert(allocator->length + size < allocator->capacity);
  allocator->length += size;
  return (void *)((uint8_t *)allocator->arena + allocator->length - size);
}

void arena_verify(struct ArenaAllocator *arena) {
  printf("[arena_verify] arean: %p %zu %zu\n", arena->arena, arena->length,
         arena->capacity);
}
