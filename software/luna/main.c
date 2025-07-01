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
  case BIN_EXPR_ADD: {
    printf("(BinaryExpressionNode type=add, left=");
    print_expression(&node->left);
    printf(", right=");
    print_expression(&node->right);
    printf(")");
    break;
  }
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
  printf("hello world from c 2\n");

  struct Lexer lexer = lexer_make(string_make("123 + 5 + 11"));

  struct Token toks[1024];
  uint16_t tok_index = 0;

  while (lexer_next(&lexer, &toks[tok_index++])) {
    struct Token token = toks[tok_index - 1];
    printf("Found token of type: %d (%d)\n", token.type, token.value.integer);
    token.value.integer = 0;
  }

  printf("Found a total of %d tokens.\n", tok_index - 1);

  uint8_t arena[1024];

  struct ArenaAllocator allocator = arena_make(&arena, 1024);

  struct Parser parser = parser_make(&allocator, toks, tok_index);
  printf("Created a parser.\n");

  struct ExpressionNode node = parse_expression(&parser, 9);

  printf("Parsed expression node of type %d\n", node.type);

  print_expression(&node);

  switch (node.type) {
  case EXPR_INTEGER_LITERAL: {
    printf("\tinteger value: %d\n", node.node.integer->value);
    break;
  }
  case EXPR_BINARY: {
    puts("Found binary node.");
    break;
  }
  }

  printf("Evaluation: %d\n", evaluate_expression(&node));

  return 0;
}
