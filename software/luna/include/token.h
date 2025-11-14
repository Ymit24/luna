#ifndef TOKEN_H
#define TOKEN_H

#include "luna_string.h"
#include "source_spans.h"
#include <stdint.h>

enum TokenType {
  T_ILLEGAL,
  T_EOF,
  T_INTEGER,
  T_PLUS,
  T_MINUS,
  T_STAR,
  T_AMPERSAND,
  T_SLASH,
  T_LPAREN,
  T_RPAREN,
  T_LBRACE,
  T_RBRACE,
  T_LBRACK,
  T_RBRACK,
  T_LANGLE,
  T_RANGLE,
  T_SEMICOLON,
  T_COLON,
  T_COMMA,
  T_PERIOD,
  T_LET,
  T_CONST,
  T_EXCLAMATION,
  T_EQUALS,
  T_EQEQ,
  T_NTEQ,
  T_LEQ,
  T_GEQ,
  T_SYMBOL,
  T_FN,
  T_RETURN,
  T_CAST,
  T_EXTERN,
  T_VARIADIC,
  T_VALUESIZE,
  T_TYPESIZE,
  T_STRING,
  T_IF,
  T_ELSE,
  T_STRUCT,
  T_WHILE,
  T_MOD
};

struct Token {
  enum TokenType type;
  struct Span span;
  union {
    uint16_t integer;
    struct LunaString symbol;
  } value;
};

#endif
