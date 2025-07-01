#ifndef AST_H
#define AST_H

#include <stdint.h>

#include "arena_allocator.h"

enum ExpressionType {
  EXPR_BINARY,
  EXPR_INTEGER_LITERAL,
};

struct ExpressionNode {
  enum ExpressionType type;
  union {
    struct IntegerLiteralNode *integer;
    struct BinaryExpressionNode *binary;
  } node;
};

struct IntegerLiteralNode {
  uint16_t value;
};

enum BinaryExpressionType { BIN_EXPR_ADD, BIN_EXPR_SUB, BIN_EXPR_MUL, BIN_EXPR_DIV };

struct BinaryExpressionNode {
  enum BinaryExpressionType type;
  struct ExpressionNode left, right;
};

struct IntegerLiteralNode *
ast_make_integer_literal(struct ArenaAllocator *allocator, uint16_t value);

struct BinaryExpressionNode *ast_make_binary_expression(
    struct ArenaAllocator *allocator, enum BinaryExpressionType type,
    struct ExpressionNode left, struct ExpressionNode right);

#endif
