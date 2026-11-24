#include "main.h"
#include <hal/io.h>

#define min(x, y) (x) < (y) ? (x) : (y)

typedef struct {
  const char *string;
  const short length;
} string_t;

extern void printf(char *);

string_t make_string(const char *);

int main(int argc, char **argz) {
  printf("Hello Wo\
            rld!");
  int a = 10;
  a++;
}

short char_str_len(const char *);

string_t make_string(const char *content) {
  return (string_t){.string = content, .length = char_str_len(content)};
}
