#include "arena_allocator.h"
#include <assert.h>
#include <stddef.h>

struct ArenaAllocator arena_make(void *arena, uint16_t capacity) {
  return (struct ArenaAllocator){
      .arena = arena,
      .capacity = capacity,
      .length = 0,
  };
}

void *arena_alloc(struct ArenaAllocator *allocator, size_t size) {
  assert(allocator->length + size < allocator->capacity);
  allocator->length += size;
  return (void *)((uint8_t *)allocator->arena + allocator->length - size);
}
