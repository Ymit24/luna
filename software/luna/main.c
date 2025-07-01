#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

#include "lexer.h"
#include "luna_string.h"
#include "token.h"

int main(void) {
  printf("hello world from c 2\n");

  struct Lexer lexer = lexer_make(string_make("123 + 5"));

  struct Token token;
  while (lexer_next(&lexer, &token)) {
    printf("Found token of type: %d (%d)\n", token.type, token.value.integer);
    token.value.integer = 0;
  }

  return 0;
}
