#ifndef LEXER_H
#define LEXER_H

#include "arena_allocator.h"
#include "luna_string.h"
#include "token.h"

#include <stdbool.h>
#include <stdint.h>

struct Lexer {
  struct ArenaAllocator *allocator;
  struct LunaString source;
  uint16_t position;
};

struct Lexer lexer_make(struct ArenaAllocator *allocator,
                        struct LunaString source);

char lexer_peek(struct Lexer *lexer);

uint16_t lexer_read_integer(struct Lexer *lexer);

void lexer_skip_whitespace(struct Lexer *lexer);

bool lexer_next(struct Lexer *lexer, struct Token *out_token);

#endif
