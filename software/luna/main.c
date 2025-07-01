#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

#include "lexer.h"
#include "luna_string.h"
#include "token.h"

int main(void) {
  printf("hello world from c 2\n");

  struct Lexer lexer = lexer_make(string_make("123 + 5"));

  struct Token toks[1024];
  uint16_t tok_index = 0;

  while (lexer_next(&lexer, &toks[tok_index++])) {
    struct Token token = toks[tok_index - 1];
    printf("Found token of type: %d (%d)\n", token.type, token.value.integer);
    token.value.integer = 0;
  }

  printf("Found a total of %d tokens.\n", tok_index - 1);

  return 0;
}
