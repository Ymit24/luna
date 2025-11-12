#ifndef SOURCE_SPANS_H
#define SOURCE_SPANS_H

#include "luna_string.h"
struct SourceFile {
  struct LunaString filepath;
  struct LunaString content;
};

#endif
