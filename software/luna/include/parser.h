#ifndef PARSER_H
#define PARSER_H

#include "arena_allocator.h"
#include "ast.h"
#include "token.h"
#include <stdint.h>

struct Parser {
  struct ArenaAllocator *allocator;
  struct Token *tokens;
  uint16_t token_count;
  uint16_t position;
};

void parser_error(char *msg);

struct Parser parser_make(struct ArenaAllocator *allocator,
                          struct Token *tokens, uint16_t token_count);

struct Token parser_peek(struct Parser *parser);

struct ExpressionNode parse_expression(struct Parser *parser);

#endif
