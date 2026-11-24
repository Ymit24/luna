#ifndef STRING_H
#define STRING_H

#include <stdbool.h>
#include <stdint.h>

struct LunaString {
  char *data;
  uint16_t length;
};

struct LunaString string_make(char *source);
bool strings_equal(struct LunaString left, struct LunaString right);

#endif
