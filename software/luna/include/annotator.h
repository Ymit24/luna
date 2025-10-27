#ifndef ANNOTATOR_H
#define ANNOTATOR_H

#include "arena_allocator.h"
#include "instructions.h"
#include "luna_string.h"
#include "llvm-c/Types.h"
#include <stdbool.h>
#include <stddef.h>

enum DataTypeKind { DTK_PRIMITIVE, DTK_FUNCTION, DTK_VOID, DTK_POINTER };
// TODO: strings arent primitives, i/u8 is prim
enum PrimitiveType { P_I8, P_I32, P_BOOL };

struct FunctionType {
  struct DataType *return_type;
  struct FunctionArgumentNode *arguments;
  struct LunaString *extern_name;
  bool is_variadic;
};

struct DataType {
  enum DataTypeKind kind;
  union {
    struct FunctionType function;
    enum PrimitiveType primitive;
    struct DataType *pointer_inner;
  } value;
  struct DataType *next;
};

struct DataTypeTable {
  struct DataType *head;
};

struct SymbolTableEntry {
  struct LunaString symbol;
  struct DataType *type;
  LLVMValueRef llvm_value;
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
  struct FunctionType *current_function;
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

struct DataType *make_void_data_type(struct ArenaAllocator *allocator);
struct DataType *make_function_data_type(struct ArenaAllocator *allocator,
                                         struct FunctionArgumentNode *arguments,
                                         struct DataType *return_type,
                                         struct LunaString *extern_name,
                                         bool is_variadic);

void print_data_type(struct DataType *data_type);

struct ExpressionNode;
struct DataType *infer_type(struct Annotator *annotator,
                            struct ExpressionNode *expr);

void print_symbol_table(struct LunaString name,
                        struct SymbolTable *symbol_table);
#endif
