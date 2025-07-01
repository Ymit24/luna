#include "lexer.h"

#include <ctype.h>
#include <stdlib.h>

struct Lexer lexer_make(struct String source) {
  return (struct Lexer){.source = source, .position = 0};
}

char lexer_peek(struct Lexer *lexer) {
  if (lexer->position < 0 || lexer->position > lexer->source.length) {
    return 0;
  }

  return lexer->source.data[lexer->position];
}

uint16_t lexer_read_integer(struct Lexer *lexer) {
  char buf[16] = {0};
  uint8_t index = 0;

  while (isdigit(lexer_peek(lexer)) && index < 16) {
    buf[index++] = lexer_peek(lexer);
    lexer->position++;
  }

  buf[index] = 0;

  return atoi(buf);
}

void lexer_skip_whitespace(struct Lexer *lexer) {
  while (isspace(lexer_peek(lexer))) {
    lexer->position++;
  }
}

bool lexer_next(struct Lexer *lexer, struct Token *out_token) {
  lexer_skip_whitespace(lexer);
  char current = lexer_peek(lexer);

  if (current == 0) {
    out_token->type = T_EOF;
    return false;
  }

  out_token->type = T_ILLEGAL;

  switch (current) {
  case '+':
    out_token->type = T_PLUS;
    break;
  case '-':
    out_token->type = T_MINUS;
    break;
  case '*':
    out_token->type = T_STAR;
    break;
  case '/':
    out_token->type = T_SLASH;
    break;
  case '(':
    out_token->type = T_LPAREN;
    break;
  case ')':
    out_token->type = T_RPAREN;
    break;
  default: {
    if (isdigit(current)) {
      out_token->type = T_INTEGER;
      out_token->value.integer = lexer_read_integer(lexer);
      return true;
    }
  }
  }

  lexer->position++;

  return true;
}
