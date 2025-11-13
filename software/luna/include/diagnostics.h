#ifndef DIAGNOSTICS_H
#define DIAGNOSTICS_H

#include "luna_string.h"
#include "source_spans.h"

struct Diagnostic {
  struct SourceFile *source;
  struct LunaString message;
  uint64_t start_offset;
  uint64_t end_offset;
  struct Diagnostic *next;
};

struct Diagnostic diagnostic_make(struct SourceFile *source,
                                             struct LunaString message,
                                             uint64_t start, uint64_t end);

void diagnostic_print(struct Diagnostic *diagnostic);

#endif
