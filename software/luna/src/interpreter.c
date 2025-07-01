#include "ast.h"
#include "interpreter.h"

uint16_t evaluate_binary_expression(struct BinaryExpressionNode *node) {
  switch (node->type) {
  case BIN_EXPR_ADD:
    return evaluate_expression(&node->left) + evaluate_expression(&node->right);
  }
}

uint16_t evaluate_expression(struct ExpressionNode *expr) {
  switch (expr->type) {
  case EXPR_INTEGER_LITERAL:
    return expr->node.integer->value;
  case EXPR_BINARY:
    return evaluate_binary_expression(expr->node.binary);
  }
}
