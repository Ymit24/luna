#include "interpreter.h"
#include "annotator.h"
#include "arena_allocator.h"
#include "ast.h"
#include "luna_string.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

struct Environment *environment_make(struct ArenaAllocator *allocator) {
  struct Environment environment = (struct Environment){
      .allocator = allocator,
      .variable = NULL,
  };

  environment.variable = ast_promote(environment.allocator,
                                     &(struct Variable){
                                         .symbol = string_make("true"),
                                         .type = &DT_BOOL,
                                         .value = 1,
                                         .next = environment.variable,
                                         .is_const = false,
                                     },
                                     sizeof(struct Variable));

  environment.variable = ast_promote(environment.allocator,
                                     &(struct Variable){
                                         .symbol = string_make("false"),
                                         .type = &DT_BOOL,
                                         .value = 0,
                                         .next = environment.variable,
                                         .is_const = false,
                                     },
                                     sizeof(struct Variable));

  return ast_promote(allocator, &environment, sizeof(struct Environment));
}

struct Variable *lookup(struct Environment *environment,
                        struct LunaString symbol) {
  struct Variable *var = environment->variable;
  while (var != NULL &&
         strncmp(var->symbol.data, symbol.data, var->symbol.length) != 0) {
    var = var->next;
  }
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
                        struct ModuleStatementNode *stmt) {
  switch (stmt->type) {
  case MOD_STMT_LET: {
    assert(lookup(environment, stmt->node.decl->symbol) == NULL);
    struct Variable variable = (struct Variable){
        .symbol = stmt->node.decl->symbol,
        .type = stmt->node.decl->data_type,
        .value = evaluate_expression(environment, stmt->node.decl->expression),
        .next = environment->variable,
        .is_const = false,
    };
    environment->variable =
        ast_promote(environment->allocator, &variable, sizeof(struct Variable));
    assert(stmt->node.decl->has_type);
    printf("let %s: %d = %d\n", variable.symbol.data,
           stmt->node.decl->data_type->kind, variable.value);
    break;
  }
  case MOD_STMT_CONST: {
    struct Variable variable = (struct Variable){
        .symbol = stmt->node.decl->symbol,
        .value = evaluate_expression(environment, stmt->node.decl->expression),
        .type = stmt->node.decl->data_type,
        .next = environment->variable,
        .is_const = true,
    };
    environment->variable =
        ast_promote(environment->allocator, &variable, sizeof(struct Variable));
    printf("let %s = %d\n", variable.symbol.data, variable.value);
    break;
  }
  case MOD_STMT_ASSIGN: {
    struct Variable *variable = lookup(environment, stmt->node.assign->symbol);

    assert(variable != NULL);
    assert(!variable->is_const);
    variable->value =
        evaluate_expression(environment, stmt->node.assign->expression);
    printf("assign %s = %d\n", variable->symbol.data, variable->value);
    break;
  }
  }

  print_variables(environment);
}

void evaluate_statements(struct Environment *environment,
                         struct ModuleStatementNode *stmt) {
  struct ModuleStatementNode *curr = stmt;

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
    struct Variable *variable = lookup(environment, expr->node.symbol->value);
    assert(variable != NULL);
    return variable->value;
  }
  case EXPR_BINARY:
    return evaluate_binary_expression(environment, expr->node.binary);
  case EXPR_FN_DEF:
    puts("TODO");
    assert(0);
    break;
  }
}
