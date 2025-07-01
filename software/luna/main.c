#include <ctype.h>
#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum TokenType { T_ILLEGAL, T_EOF, T_INTEGER };

struct Token {
  enum TokenType type;
  union {
    uint16_t integer;
  };
};

struct String {
  char *data;
  uint16_t length;
};

struct String string_make(char *source) {
  return (struct String){.data = source, .length = strlen(source)};
}

struct Lexer {
  struct String source;
  uint16_t position;
};

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
  default: {
    if (isdigit(current)) {
      out_token->type = T_INTEGER;
      out_token->integer = lexer_read_integer(lexer);
      return true;
    }
  }
  }

  lexer->position++;

  return true;
}

int main() {
  printf("hello world from c 2\n");

  struct Lexer lexer = lexer_make(string_make("123 5"));

  struct Token token;

  while (lexer_next(&lexer, &token)) {
    printf("Found token of type: %d (%d)\n", token.type, token.integer);
  }

  return 0;
}
