#include "lexer.h"
#include "arena_allocator.h"
#include "luna_string.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

struct Lexer lexer_make(struct ArenaAllocator *allocator,
                        struct LunaString source) {
  return (struct Lexer){
      .allocator = allocator, .source = source, .position = 0};
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

struct LunaString lexer_read_symbol(struct Lexer *lexer) {
  char *buf = arena_alloc(lexer->allocator, 64 * sizeof(char));
  uint8_t index = 0;

  while (isalpha(lexer_peek(lexer)) && index < 64) {
    buf[index++] = lexer_peek(lexer);
    lexer->position++;
  }

  buf[index] = 0;
  return (struct LunaString){.data = &buf[0], .length = index};
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
  case ';':
    out_token->type = T_SEMICOLON;
    break;
  case ':':
    out_token->type = T_COLON;
    break;
  case '=':
    out_token->type = T_EQUALS;
    break;
  default: {
    if (isdigit(current)) {
      out_token->type = T_INTEGER;
      out_token->value.integer = lexer_read_integer(lexer);
      return true;
    } else {
      if (lexer->source.length - lexer->position >= 3 &&
          strncmp("let", &lexer->source.data[lexer->position], 3) == 0) {
        out_token->type = T_LET;
        lexer->position += 3;
        return true;
      } else if (lexer->source.length - lexer->position >= 5 &&
                 strncmp("const", &lexer->source.data[lexer->position], 5) ==
                     0) {
        out_token->type = T_CONST;
        lexer->position += 5;
        return true;
      } else {
        out_token->type = T_SYMBOL;
        out_token->value.symbol = lexer_read_symbol(lexer);
        return true;
      }
    }
  }
  }

  lexer->position++;

  return true;
}
