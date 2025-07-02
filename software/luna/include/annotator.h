#ifndef ANNOTATOR_H
#define ANNOTATOR_H

#include "arena_allocator.h"
#include "luna_string.h"

struct DataType {
  struct LunaString symbol;
  struct DataType *next;
};

struct DataTypeTable {
  struct DataType *head;
};

struct SymbolTableEntry {
  struct LunaString symbol;
  struct SymbolTableEntry *next;
};

struct SymbolTable {
  struct SymbolTableEntry *head;
};

struct Annotator {
  struct SymbolTable symbol_table;
  struct DataTypeTable data_type_table;
  struct ArenaAllocator *allocator;
};

void annotator_initialize_primitives(struct Annotator *annotator);
struct Annotator annotator_make(struct ArenaAllocator *allocator);

#endif
