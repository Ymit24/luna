#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

#include "annotator.h"
#include "arena_allocator.h"
#include "ast.h"
#include "interpreter.h"
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
  case EXPR_SYMBOL_LITERAL: {
    printf("(ExpressionNode type=symbol_literal, value=%s)",
           node->node.symbol->value.data);
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

  uint8_t arena[10024];

  struct ArenaAllocator allocator = arena_make(&arena, 10024);

  struct Lexer lexer = lexer_make(
      &allocator, string_make("let a = 5 - (2 + 1); const c = 7; a = 10; let x "
                              "= 10; let y = 5 + x; a = 10; let g: int = 5;"));

  struct Token toks[1024];
  uint16_t tok_index = 0;

  puts("tokens:");
  while (lexer_next(&lexer, &toks[tok_index++])) {
    if (false) {
      printf("\t%d\n", toks[tok_index - 1].type);
    }
  }
  puts("");

  printf("Found a total of %d tokens.\n", tok_index - 1);

  struct Parser parser = parser_make(&allocator, toks, tok_index);

  struct StatementNode *stmt = parse_statements(&parser);

  struct Annotator annotator = annotator_make(&allocator);


  annotator_initialize_primitives(&annotator);

  struct Environment environment = (struct Environment){
      .allocator = &allocator,
      .variable = NULL,
  };

  evaluate_statements(&environment, stmt);

  printf("Arena Allocator used %d/%d (%0.2f%%) memory\n", allocator.length,
         allocator.capacity,
         (double)allocator.length / (double)allocator.capacity * 100.0);
  return 0;
}
