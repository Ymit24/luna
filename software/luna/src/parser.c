#include "parser.h"
#include "arena_allocator.h"
#include "ast.h"
#include "token.h"
#include <assert.h>

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

struct ExpressionNode parse_expression(struct Parser *parser) {
  return (struct ExpressionNode){
      .type = EXPR_INTEGER_LITERAL,
      .node.integer = parse_integer_literal(parser),
  };
}

struct IntegerLiteralNode *parse_integer_literal(struct Parser *parser) {
  struct Token token = parser_peek(parser);
  assert(token.type == T_INTEGER);

  parser->position++;

  return ast_make_integer_literal(parser->allocator, token.value.integer);
}
