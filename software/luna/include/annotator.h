#ifndef ANNOTATOR_H
#define ANNOTATOR_H

#include "arena_allocator.h"
#include "instructions.h"
#include "luna_string.h"
#include <stdbool.h>
#include <stddef.h>

enum DataTypeKind { DTK_PRIMITIVE, DTK_FUNCTION };
enum PrimitiveType { P_INT, P_BOOL };

struct FunctionType {
  struct DataType *return_type;
};

struct DataType {
  enum DataTypeKind kind;
  union {
    struct FunctionType function;
    enum PrimitiveType primitive;
  } value;
  struct DataType *next;
};

struct DataTypeTable {
  struct DataType *head;
};

struct SymbolTableEntry {
  struct LunaString symbol;
  struct DataType *type;
  struct SymbolTableEntry *next;
  enum MemorySegment memory_segment;
  uint16_t index;
};

struct SymbolTable {
  struct SymbolTableEntry *head;
  struct SymbolTable *parent;
  uint16_t current_index;
  bool is_function;
};

struct Annotator {
  struct SymbolTable root_symbol_table;
  struct SymbolTable *current_symbol_table;
  struct DataTypeTable data_type_table;
  struct ArenaAllocator *allocator;
};

void annotator_initialize_primitives(struct Annotator *annotator);
struct Annotator annotator_make(struct ArenaAllocator *allocator);

struct ModuleStatementNode;

void annotator_visit_module_statements(struct Annotator *annotator,
                                       struct ModuleStatementNode *statement);

struct SymbolTableEntry *lookup_symbol_in(struct LunaString symbol,
                                          struct SymbolTable *symbol_table);
struct SymbolTableEntry *lookup_symbol(struct Annotator *annotator,
                                       struct LunaString symbol);
#endif
