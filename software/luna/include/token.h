#ifndef TOKEN_H
#define TOKEN_H

#include "luna_string.h"
#include <stdint.h>

enum TokenType {
  T_ILLEGAL,
  T_EOF,
  T_INTEGER,
  T_PLUS,
  T_MINUS,
  T_STAR,
  T_SLASH,
  T_LPAREN,
  T_RPAREN,
  T_SEMICOLON,
  T_LET,
  T_CONST,
  T_EQUALS,
  T_SYMBOL,
};

struct Token {
  enum TokenType type;
  union {
    uint16_t integer;
    struct LunaString symbol;
  } value;
};

#endif
