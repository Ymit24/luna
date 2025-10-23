#ifndef STRING_H
#define STRING_H

#include <stdint.h>

struct LunaString {
  char *data;
  uint16_t length;
};

struct LunaString string_make(char *source);

#endif
