#ifndef SOURCE_SPANS_H
#define SOURCE_SPANS_H

#include "arena_allocator.h"
#include "luna_string.h"
#include <stddef.h>

struct LineMap {
  struct LineMapEntry *head;
  size_t entries;
  size_t source_length;
};

struct LineMapEntry {
  uint32_t line;
  uint32_t start_byte;
  struct LineMapEntry *next;
};

struct SourceFile {
  struct LunaString filepath;
  struct LunaString content;
  struct LineMap line_map;
};

struct Span {
  struct SourceFile *source;
  uint32_t start_offset;
  uint32_t end_offset;
};

struct SourceFile source_file_make(struct ArenaAllocator *allocator,
                                   struct LunaString filepath,
                                   struct LunaString content);

struct LineMap line_map_make(struct ArenaAllocator *allocator,
                             struct LunaString source);

bool line_map_query(struct LineMap *line_map, uint32_t offset,
                    uint32_t *line_number, uint32_t *line_start_offset,
                    uint32_t *line_end_offset);

struct Span span_make(struct SourceFile *source, uint32_t start_offset,
                      uint32_t end_offset);

struct Span span_union(struct Span span1, struct Span span2);

#endif
