#include "interpreter.h"
#include "arena_allocator.h"
#include "ast.h"
#include "luna_string.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

void evaluate_statement(struct Environment *environment,
                        struct StatementNode *stmt) {
  switch (stmt->type) {
  case STMT_EXPR:
    printf("expr: %d\n", evaluate_expression(environment, stmt->node.expr));
    break;
  case STMT_LET: {
    struct Variable var = (struct Variable){
        .symbol = stmt->node.let->symbol,
        .value = evaluate_expression(environment, stmt->node.let->expression),
        .next = environment->variable,
    };
    environment->variable =
        ast_promote(environment->allocator, &var, sizeof(struct Variable));
    printf("let %s = %d\n", stmt->node.let->symbol.data,
           evaluate_expression(environment, stmt->node.let->expression));
    break;
  }
  }
}

void evaluate_statements(struct Environment *environment,
                         struct StatementNode *stmt) {
  struct StatementNode *curr = stmt;

  while (curr != NULL) {
    evaluate_statement(environment, curr);
    curr = curr->next;
  }
}

uint16_t evaluate_binary_expression(struct Environment *environment,
                                    struct BinaryExpressionNode *node) {
  switch (node->type) {
  case BIN_EXPR_ADD:
    return evaluate_expression(environment, node->left) +
           evaluate_expression(environment, node->right);
  case BIN_EXPR_SUB:
    return evaluate_expression(environment, node->left) -
           evaluate_expression(environment, node->right);
  case BIN_EXPR_MUL:
    return evaluate_expression(environment, node->left) *
           evaluate_expression(environment, node->right);
  case BIN_EXPR_DIV:
    return evaluate_expression(environment, node->left) /
           evaluate_expression(environment, node->right);
  }
}

uint16_t evaluate_expression(struct Environment *environment,
                             struct ExpressionNode *expr) {
  switch (expr->type) {
  case EXPR_INTEGER_LITERAL:
    return expr->node.integer->value;
  case EXPR_SYMBOL_LITERAL: {
    printf("Searching for variable: %s\n", expr->node.symbol->value.data);
    struct Variable *var = environment->variable;
    while (var != NULL &&
           strncmp(var->symbol.data, expr->node.symbol->value.data,
                   var->symbol.length) != 0) {
      var = var->next;
    }
    assert(var != NULL);
    return var->value;
  }
  case EXPR_BINARY:
    return evaluate_binary_expression(environment, expr->node.binary);
  }
}
