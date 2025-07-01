#include "luna_string.h"
#include <string.h>

struct String string_make(char *source) {
  return (struct String){.data = source, .length = strlen(source)};
}
