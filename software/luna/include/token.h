#ifndef TOKEN_H
#define TOKEN_H

#include <stdint.h>

enum TokenType { T_ILLEGAL, T_EOF, T_INTEGER, T_PLUS, T_MINUS };

struct Token {
  enum TokenType type;
  union {
    uint16_t integer;
  } value;
};

#endif

