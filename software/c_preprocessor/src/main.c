#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "linked_list.h"

LINKED_LIST_PROTOTYPE(int);
LINKED_LIST_IMPL(int);

void test(void) {
  linked_list_int list = make_linked_list(int);
  linked_list_append(int, &list, 38);
  int out;
  if (linked_list_element_at(int, &list, 0, &out) != 0) {
    perror("failed to find element at zero!");
    return;
  }

  printf("found value ( %d )\n", out);
}

typedef enum {
  HASH,
  IDENT,
  NUM,
  STR_LIT,
  CHAR_LIT,
  HEADER_NAME,
  OPERATORS
} token_type;

typedef struct {
  short start;
  short end;
} span;

typedef struct {
  span source;
  token_type type;
} token;

typedef struct {
} preprocessor_state;

static preprocessor_state preprocessor;

void preprocess_file(FILE *file) {
  char ch;
  while ((ch = fgetc(file)) != EOF) {
    if (ch == '#') {
    }
  }
}

bool is_char(FILE *file, char pattern) { return fgetc(file) == pattern; }

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
    return 1;
  }

  const char *abs_path = realpath(argv[1], NULL);
  if (abs_path == NULL) {
    perror("Error resolving filepath");
    return 1;
  }

  FILE *file = fopen(abs_path, "r");
  if (file == NULL) {
    perror("Error opening the file");
    return 1;
  }

  printf("Hello world c code. Path: %s\nContents:\n", abs_path);

  test();

  preprocess_file(file);

  rewind(file);

  // char ch;
  // while ((ch = fgetc(file)) != EOF) {
  //   putchar(ch);
  // }

  fclose(file);

  printf("Done.\n");

  return 0;
}
