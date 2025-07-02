#include "interpreter.h"
#include "ast.h"

uint16_t evaluate_binary_expression(struct BinaryExpressionNode *node) {
  switch (node->type) {
  case BIN_EXPR_ADD:
    return evaluate_expression(node->left) + evaluate_expression(node->right);
  case BIN_EXPR_SUB:
    return evaluate_expression(node->left) - evaluate_expression(node->right);
  case BIN_EXPR_MUL:
    return evaluate_expression(node->left) * evaluate_expression(node->right);
  case BIN_EXPR_DIV:
    return evaluate_expression(node->left) / evaluate_expression(node->right);
  }
}

uint16_t evaluate_expression(struct ExpressionNode *expr) {
  switch (expr->type) {
  case EXPR_INTEGER_LITERAL:
    return expr->node.integer->value;
  case EXPR_SYMBOL_LITERAL:
    return 0;
  case EXPR_BINARY:
    return evaluate_binary_expression(expr->node.binary);
  }
}
