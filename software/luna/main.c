#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

#include "arena_allocator.h"
#include "ast.h"
#include "interpretor.h"
#include "lexer.h"
#include "luna_string.h"
#include "parser.h"
#include "token.h"

void print_expression(struct ExpressionNode *node);

void print_binary_expression(struct BinaryExpressionNode *node) {

  switch (node->type) {
  case BIN_EXPR_ADD:
    printf("(BinaryExpressionNode type=add, left=");
    print_expression(node->left);
    printf(", right=");
    print_expression(node->right);
    printf(")");
    break;
  case BIN_EXPR_SUB:
    printf("(BinaryExpressionNode type=sub, left=");
    print_expression(node->left);
    printf(", right=");
    print_expression(node->right);
    printf(")");
    break;
  case BIN_EXPR_MUL:
    printf("(BinaryExpressionNode type=mul, left=");
    print_expression(node->left);
    printf(", right=");
    print_expression(node->right);
    printf(")");
    break;
  case BIN_EXPR_DIV:
    printf("(BinaryExpressionNode type=div, left=");
    print_expression(node->left);
    printf(", right=");
    print_expression(node->right);
    printf(")");
    break;
  }
}

void print_expression(struct ExpressionNode *node) {
  switch (node->type) {
  case EXPR_INTEGER_LITERAL: {
    printf("(ExpressionNode type=integeral_literal, value=%d)",
           node->node.integer->value);
    break;
  }
  case EXPR_BINARY: {
    printf("(ExpressionNode type=binary, node=");
    print_binary_expression(node->node.binary);
    printf(")");
    break;
  }
  }
}

int main(void) {
  puts("Luna Compiler");

  uint8_t arena[1024];

  struct ArenaAllocator allocator = arena_make(&arena, 1024);

  struct Lexer lexer =
      lexer_make(&allocator, string_make("5 - (2 + 1); 10; let x = 10;"));

  struct Token toks[1024];
  uint16_t tok_index = 0;

  while (lexer_next(&lexer, &toks[tok_index++])) {
    struct Token token = toks[tok_index - 1];
    printf("Found token of type: %d (%d)\n", token.type, token.value.integer);
    token.value.integer = 0;
  }

  printf("Found a total of %d tokens.\n", tok_index - 1);

  struct Parser parser = parser_make(&allocator, toks, tok_index);
  printf("Created a parser.\n");

  struct StatementNode *stmt = parse_statements(&parser);

  assert(stmt->type == STMT_EXPR);

  printf("Statement has next: %d\n", stmt->next != NULL);

  while (stmt != NULL) {
    puts("\n\n-----------\n");
    printf("Printing expression: %d\n", stmt->type);
    print_expression(stmt->node.expr);
    printf("\nEvaluation: %d\n", evaluate_expression(stmt->node.expr));
    puts("\n+++++++++++\n\n");
    stmt = stmt->next;
  }

  return 0;
}
