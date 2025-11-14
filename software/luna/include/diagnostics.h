#ifndef DIAGNOSTICS_H
#define DIAGNOSTICS_H

#include "arena_allocator.h"
#include "luna_string.h"
#include "source_spans.h"

struct Diagnostic {
  struct LunaString message;
  struct Span span;
  struct Diagnostic *next;
};

struct Diagnostics {
  struct Diagnostic *head;
  struct Diagnostic *tail;
  size_t length;
};

struct Diagnostic diagnostic_make(struct LunaString message, struct Span span);

struct Diagnostics diagnostics_make(void);

void diagnostic_emit(struct ArenaAllocator *allocator,
                     struct Diagnostics *diagnostics,
                     struct Diagnostic *diagnostic);

void diagnostic_print(struct Diagnostic *diagnostic);
void diagnostics_print(struct Diagnostics *diagnostics);

#endif
