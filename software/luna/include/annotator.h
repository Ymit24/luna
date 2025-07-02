#ifndef ANNOTATOR_H
#define ANNOTATOR_H
#pragma once

#include "arena_allocator.h"
#include "luna_string.h"

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

struct StatementNode;

void annotator_visit_statements(struct Annotator *annotator,
                                struct StatementNode *statement);

#endif
