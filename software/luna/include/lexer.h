#ifndef LEXER_H
#define LEXER_H

#include "luna_string.h"
#include "token.h"

#include <stdbool.h>
#include <stdint.h>

struct Lexer {
  struct String source;
  uint16_t position;
};

struct Lexer lexer_make(struct String source);

char lexer_peek(struct Lexer *lexer);

uint16_t lexer_read_integer(struct Lexer *lexer);

void lexer_skip_whitespace(struct Lexer *lexer);

bool lexer_next(struct Lexer *lexer, struct Token *out_token);

#endif
