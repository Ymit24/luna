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

struct BinaryExpressionNode *ast_make_binary_expression(
    struct ArenaAllocator *allocator, enum BinaryExpressionType type,
    struct ExpressionNode left, struct ExpressionNode right) {
  struct BinaryExpressionNode *node =
      arena_alloc(allocator, sizeof(struct BinaryExpressionNode));
  node->type = type;
  node->left = left;
  node->right = right;
  return node;
}
