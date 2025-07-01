#include <stdbool.h>
#include <stdio.h>

enum TokenType { T_ILLEGAL, T_EOF, T_INTEGER };

struct Token {
  enum TokenType type;
  union {
    short integer;
  };
};

struct String {
  char *data;
  short length;
};

struct Lexer {
  struct String source;
  short position;
};

struct Lexer lexer_make(struct String source) {
  return (struct Lexer){.source = source, .position = 0};
}

char lexer_peek

    bool
    lexer_next(struct Lexer *lexer, struct Token *out_token) {
  out_token->type = T_ILLEGAL;
  return false;
}

int main() {
  printf("hello world from c\n");
  return 0;
}
