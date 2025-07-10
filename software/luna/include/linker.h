#ifndef LINKER_H
#define LINKER_H

#include "instruction_builder.h"
#include "arena_allocator.h"
#include "luna_string.h"

#define MAX_LABELS 256

struct LabelEntry {
  struct LunaString label;
  uint16_t address;
  bool is_resolved;
};

struct Linker {
  struct LabelEntry labels[MAX_LABELS];
  uint16_t label_count;
  struct ArenaAllocator *allocator;
};

// Create and initialize linker
struct Linker linker_init(struct ArenaAllocator *allocator);

// First pass: collect all labels and their addresses
void linker_collect_labels(struct Linker *linker, struct InstructionBuilder *builder);

// Second pass: resolve label references to addresses  
void linker_resolve_addresses(struct Linker *linker, struct InstructionBuilder *builder);

// Look up address for a label
uint16_t linker_get_address(struct Linker *linker, struct LunaString label);

#endif // LINKER_H 
