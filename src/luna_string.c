#include "luna_string.h"
#include <string.h>

struct LunaString string_make(char *source) {
  return (struct LunaString){.data = source, .length = strlen(source)};
}

bool strings_equal(struct LunaString left, struct LunaString right) {
  if (left.length != right.length) {
    return false;
  }

  return strncmp(left.data, right.data, left.length) == 0;
}
