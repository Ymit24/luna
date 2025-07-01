#include "ast.h"
#include "arena_allocator.h"
#include <stdint.h>

struct IntegerLiteralNode *
ast_make_integer_literal(struct ArenaAllocator *allocator, uint16_t value) {
  struct IntegerLiteralNode *node =
      arena_alloc(allocator, sizeof(struct IntegerLiteralNode));
  node->value = value;
  return node;
}
