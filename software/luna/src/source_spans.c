#include "source_spans.h"
#include "arena_allocator.h"
#include "ast.h"
#include "luna_string.h"
#include <assert.h>
#include <stdio.h>

struct SourceFile source_file_make(struct ArenaAllocator *allocator,
                                   struct LunaString filepath,
                                   struct LunaString content) {
  struct LineMap line_map = line_map_make(allocator, content);
  return (struct SourceFile){
      .filepath = filepath,
      .content = content,
      .line_map = line_map,
  };
}

struct LineMapEntry *line_map_make_entry(struct ArenaAllocator *allocator,
                                         uint32_t line, uint32_t offset) {
  assert(allocator != NULL);

  return ast_promote(allocator,
                     &(struct LineMapEntry){
                         .line = line,
                         .start_byte = offset,
                         .next = NULL,
                     },
                     sizeof(struct LineMapEntry));
}

struct LineMapEntry *line_map_build(struct ArenaAllocator *allocator,
                                    struct LunaString source,
                                    uint32_t *entry_count) {
  struct LineMapEntry *head = line_map_make_entry(allocator, 0, 0);
  struct LineMapEntry *tail = head;
  *entry_count = 1;

  size_t index = 0;

  while (index < source.length) {
    if (source.data[index] == '\n') {
      tail->next = line_map_make_entry(allocator, *entry_count + 1, index);
      tail = tail->next;
      *entry_count += 1;
    }
    index++;
  }

  return head;
}

struct LineMap line_map_make(struct ArenaAllocator *allocator,
                             struct LunaString source) {
  assert(allocator != NULL);

  uint32_t entries;
  struct LineMapEntry *head = line_map_build(allocator, source, &entries);

  // struct LineMapEntry *tail = head;
  // printf("entry count: %d\n", (int)entries);
  // for (int i = 0; i < (int)entries; i++) {
  //   printf("\t%d: %d,%d\n", i, tail->line, tail->start_byte);
  //   tail = tail->next;
  // }

  return (struct LineMap){
      .entries = entries,
      .head = head,
      .source_length = source.length,
  };
}

bool line_map_query(struct LineMap *line_map, uint32_t offset,
                    uint32_t *line_number, uint32_t *line_start_offset,
                    uint32_t *line_end_offset) {
  assert(line_map != NULL);

  struct LineMapEntry *current = line_map->head;

  while (current != NULL) {
    if (offset > current->start_byte &&
        (current->next == NULL || offset < current->next->start_byte)) {
      *line_number = current->line;
      *line_start_offset = current->start_byte;
      if (current->next == NULL) {
        *line_end_offset = line_map->source_length;
      } else {
        *line_end_offset = current->next->start_byte - 1;
      }
      return true;
    }
    current = current->next;
  }
  return false;
}
