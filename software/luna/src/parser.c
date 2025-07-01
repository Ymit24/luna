#include "parser.h"
#include "arena_allocator.h"
#include "ast.h"
#include "token.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

struct IntegerLiteralNode *parse_integer_literal(struct Parser *parser);

struct Parser parser_make(struct ArenaAllocator *allocator,
                          struct Token *tokens, uint16_t token_count) {
  return (struct Parser){
      .allocator = allocator,
      .tokens = tokens,
      .token_count = token_count,
      .position = 0,
  };
}

struct Token parser_peek(struct Parser *parser) {
  assert(parser->position < parser->token_count);

  return parser->tokens[parser->position];
}

uint8_t precedence_for_token(enum TokenType type) {
  switch (type) {
  case T_PLUS:
    return 1;
  default:
    return 99;
  }
}

struct ExpressionNode parse_nud(struct Parser *parser, struct Token token) {
  switch (token.type) {
  case T_INTEGER: {
    return (struct ExpressionNode){.type = EXPR_INTEGER_LITERAL,
                                   .node.integer =
                                       parse_integer_literal(parser)};
  }
  default:
    printf("Found type: %d\n", token.type);
    assert(false);
  }
}

struct ExpressionNode parse_plus(struct Parser *parser,
                                 struct ExpressionNode left) {
  struct ExpressionNode right =
      parse_expression(parser, precedence_for_token(T_PLUS));
  return (struct ExpressionNode){
      .type = EXPR_BINARY,
      .node.binary = ast_make_binary_expression(parser->allocator, BIN_EXPR_ADD,
                                                left, right)};
}

struct ExpressionNode parse_expression(struct Parser *parser,
                                       uint8_t precendence) {
  struct Token token = parser_peek(parser);

  struct ExpressionNode left = parse_nud(parser, token);

  while (precedence_for_token(parser_peek(parser).type) < precendence) {
    token = parser_peek(parser);
    parser->position++;

    switch (token.type) {
    case T_PLUS: {
      left = parse_plus(parser, left);
    }
    case T_EOF: {
      puts("Found eof.");
      break;
    }
    default: {
      assert(false);
    }
    }
  }

  return left;
}

struct IntegerLiteralNode *parse_integer_literal(struct Parser *parser) {
  struct Token token = parser_peek(parser);
  assert(token.type == T_INTEGER);

  parser->position++;

  return ast_make_integer_literal(parser->allocator, token.value.integer);
}
