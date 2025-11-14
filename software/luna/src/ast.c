#include "ast.h"
#include "arena_allocator.h"
#include <stdint.h>
#include <string.h>

struct IntegerLiteralNode *
ast_make_integer_literal(struct ArenaAllocator *allocator, uint16_t value) {
  struct IntegerLiteralNode *node =
      arena_alloc(allocator, sizeof(struct IntegerLiteralNode));
  node->value = value;
  return node;
}

struct BinaryExpressionNode *ast_make_binary_expression(
    struct ArenaAllocator *allocator, enum BinaryExpressionType type,
    struct ExpressionNode *left, struct ExpressionNode *right) {
  struct BinaryExpressionNode *node =
      arena_alloc(allocator, sizeof(struct BinaryExpressionNode));
  node->type = type;
  node->left = left;
  node->right = right;
  return node;
}

struct ExpressionNode *
ast_promote_expression_node(struct ArenaAllocator *allocator,
                            struct ExpressionNode expr) {
  struct ExpressionNode *new =
      arena_alloc(allocator, sizeof(struct ExpressionNode));
  memcpy(new, &expr, sizeof(struct ExpressionNode));
  return new;
}

void *ast_promote(struct ArenaAllocator *allocator, void *src, uint8_t size) {
  void *dest = arena_alloc(allocator, size);
  memcpy(dest, src, size);
  return dest;
}

size_t count_array_initializer_length(
    struct ArrayInitializerExpressionNode *initiailzier) {
  if (initiailzier == NULL) {
    return 0;
  }
  return 1 + count_array_initializer_length(initiailzier->next);
}

char *binary_expression_type_to_string(enum BinaryExpressionType type) {
  switch (type) {
  case BIN_EXPR_ADD:
    return "add";
  case BIN_EXPR_SUB:
    return "subtract";
  case BIN_EXPR_MUL:
    return "multiply";
  case BIN_EXPR_DIV:
    return "divide";
  case BIN_EXPR_LT:
    return "less than";
  case BIN_EXPR_GT:
    return "greater than";
  case BIN_EXPR_EQ:
    return "equals";
  case BIN_EXPR_NEQ:
    return "not equals";
  case BIN_EXPR_LEQ:
    return "less than equals";
  case BIN_EXPR_GEQ:
    return "greater than equals";
    break;
  }
}
