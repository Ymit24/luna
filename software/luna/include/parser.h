#ifndef PARSER_H
#define PARSER_H

#include "arena_allocator.h"
#include "ast.h"
#include "diagnostics.h"
#include "source_spans.h"
#include "token.h"
#include <stdint.h>

struct Parser {
  struct ArenaAllocator *allocator;
  struct Diagnostics *diagnostics;
  struct SourceFile source_file;
  struct Token *tokens;
  uint16_t token_count;
  uint16_t position;
};

void parser_error(char *msg);

struct Parser parser_make(struct ArenaAllocator *allocator,
                          struct Diagnostics *diagnostics,
                          struct SourceFile source_file, struct Token *tokens,
                          uint16_t token_count);

struct Token parser_peek(struct Parser *parser);

struct ModuleStatementNode *parse_module_statements(struct Parser *parser);

struct ModuleStatementNode *parse_module_statement(struct Parser *parser);

struct ExpressionNode *parse_expression(struct Parser *parser,
                                        uint8_t precendence);

#endif
