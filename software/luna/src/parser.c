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
  case T_MINUS:
    return 1;
  case T_STAR:
    return 2;
  case T_SLASH:
    return 2;
  case T_RPAREN:
    return 0;
  default:
    return 99;
  }
}

struct ExpressionNode parse_nud(struct Parser *parser, struct Token token) {
  switch (token.type) {
  case T_INTEGER:
    return (struct ExpressionNode){.type = EXPR_INTEGER_LITERAL,
                                   .node.integer =
                                       parse_integer_literal(parser)};
  case T_LPAREN:
    assert(parser_peek(parser).type == T_LPAREN);
    parser->position++;
    struct ExpressionNode result = parse_expression(parser, 0);
    assert(parser_peek(parser).type == T_RPAREN);
    parser->position++; // consume the close paren.

    return result;
  default:
    printf("Found type: %d\n", token.type);
    assert(false);
  }
}

struct ExpressionNode parse_expression(struct Parser *parser,
                                       uint8_t precendence) {
  struct Token token = parser_peek(parser);

  struct ExpressionNode left = parse_nud(parser, token);

  while (precendence < precedence_for_token(parser_peek(parser).type)) {
    token = parser_peek(parser);
    if (token.type == T_EOF) {
      return left;
    }
    parser->position++;

    switch (token.type) {
    case T_PLUS:
      left = (struct ExpressionNode){
          .type = EXPR_BINARY,
          .node.binary = ast_make_binary_expression(
              parser->allocator, BIN_EXPR_ADD, left,
              parse_expression(parser, precedence_for_token(T_PLUS)))};
      break;
    case T_MINUS:
      left = (struct ExpressionNode){
          .type = EXPR_BINARY,
          .node.binary = ast_make_binary_expression(
              parser->allocator, BIN_EXPR_SUB, left,
              parse_expression(parser, precedence_for_token(T_MINUS)))};
      break;
    case T_STAR:
      left = (struct ExpressionNode){
          .type = EXPR_BINARY,
          .node.binary = ast_make_binary_expression(
              parser->allocator, BIN_EXPR_MUL, left,
              parse_expression(parser, precedence_for_token(T_STAR)))};
      break;
    case T_SLASH:
      left = (struct ExpressionNode){
          .type = EXPR_BINARY,
          .node.binary = ast_make_binary_expression(
              parser->allocator, BIN_EXPR_DIV, left,
              parse_expression(parser, precedence_for_token(T_SLASH)))};
      break;
    default:
      printf("Found unexpected token of type %d\n", token.type);
      assert(false);
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
