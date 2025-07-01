#ifndef AST_H
#define AST_H

#include <stdint.h>

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

enum BinaryExpressionType { BIN_EXPR_ADD };

struct BinaryExpressionNode {
  enum BinaryExpressionType type;
  struct ExpressionNode left, right;
};

#endif
