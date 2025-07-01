#include "luna_string.h"
#include <string.h>

struct LunaString string_make(char *source) {
  return (struct LunaString){.data = source, .length = strlen(source)};
}
