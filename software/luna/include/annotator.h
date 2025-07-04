#ifndef ANNOTATOR_H
#define ANNOTATOR_H

#include "arena_allocator.h"
#include "luna_string.h"
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

extern struct DataType DT_INT;
extern struct DataType DT_BOOL;

struct SymbolTableEntry {
  struct LunaString symbol;
  struct DataType *type;
  struct SymbolTable *subtable;
  struct SymbolTableEntry *next;
};

struct SymbolTable {
  struct SymbolTableEntry *head;
};

struct Annotator {
  struct SymbolTable symbol_table;
  struct SymbolTable *current_symbol_table;
  struct DataTypeTable data_type_table;
  struct ArenaAllocator *allocator;
};

void annotator_initialize_primitives(struct Annotator *annotator);
struct Annotator annotator_make(struct ArenaAllocator *allocator);

struct ModuleStatementNode;

void annotator_visit_module_statements(struct Annotator *annotator,
                                       struct ModuleStatementNode *statement);

struct SymbolTableEntry *lookup_symbol(struct Annotator *annotator,
                                       struct LunaString symbol_path[],
                                       size_t symbol_path_length);

struct SymbolTableEntry *lookup_symbol_current(struct Annotator *annotator,
                                               struct LunaString symbol_path);

#endif
