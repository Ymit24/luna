#include "interpreter.h"
#include "arena_allocator.h"
#include "ast.h"
#include "luna_string.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

struct Variable *lookup(struct Environment *environment,
                        struct LunaString symbol) {
  struct Variable *var = environment->variable;
  while (var != NULL &&
         strncmp(var->symbol.data, symbol.data, var->symbol.length) != 0) {
    var = var->next;
  }
  assert(var != NULL);
  return var;
}

void print_variables(struct Environment *environment) {
  struct Variable *variable = environment->variable;

  puts("Variables:");
  while (variable != NULL) {
    printf("\t%s = %d\n", variable->symbol.data, variable->value);
    variable = variable->next;
  }
}

void evaluate_statement(struct Environment *environment,
                        struct StatementNode *stmt) {
  switch (stmt->type) {
  case STMT_EXPR:
    printf("expr: %d\n", evaluate_expression(environment, stmt->node.expr));
    break;
  case STMT_LET: {
    struct Variable variable = (struct Variable){
        .symbol = stmt->node.let->symbol,
        .value = evaluate_expression(environment, stmt->node.let->expression),
        .next = environment->variable,
    };
    environment->variable =
        ast_promote(environment->allocator, &variable, sizeof(struct Variable));
    printf("let %s = %d\n", variable.symbol.data, variable.value);
    break;
  }
  case STMT_ASSIGN: {
    struct Variable *variable = lookup(environment, stmt->node.assign->symbol);
    variable->value =
        evaluate_expression(environment, stmt->node.assign->expression);
    printf("assign %s = %d\n", variable->symbol.data, variable->value);
    break;
  }
  }

  print_variables(environment);
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
    return lookup(environment, expr->node.symbol->value)->value;
  }
  case EXPR_BINARY:
    return evaluate_binary_expression(environment, expr->node.binary);
  }
}
