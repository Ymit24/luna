#ifndef STRING_H
#define STRING_H

#include <stdint.h>

struct String {
  char *data;
  uint16_t length;
};

struct String string_make(char *source);

#endif

