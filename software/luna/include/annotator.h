#ifndef ANNOTATOR_H
#define ANNOTATOR_H

#include "arena_allocator.h"
#include "instructions.h"
#include "luna_string.h"
#include "llvm-c/Types.h"
#include <stdbool.h>
#include <stddef.h>

enum DataTypeKind {
  DTK_PRIMITIVE,
  DTK_FUNCTION,
  DTK_VOID,
  DTK_POINTER,
  DTK_ARRAY,
  DTK_STRUCTURE,
  DTK_STRUCTURE_DEF,
  DTK_MODULE_DEF,
  DTK_MODULE,
};

enum PrimitiveKind { P_INT, P_FLOAT };

struct PrimitiveType {
  enum PrimitiveKind kind;
  uint8_t bitwidth;
  bool is_signed; // ONLY for int types
};

struct FunctionType {
  struct DataType *return_type;
  struct FunctionArgumentNode *arguments;
  struct LunaString *extern_name;
  bool is_variadic;
};

struct StructDefinitionType {
  struct StructDefinitionExpressionNode *definition;
};

struct StructType {
  struct ScopedSymbolLiteralNode *name;
  struct StructDefinitionExpressionNode *definition;
};

struct ModuleDefinitionType {
  struct ModuleNode *module_definition;
};

struct ModuleType {
  struct ScopedSymbolLiteralNode *name;
  struct ModuleDefinitionType *module_definition;
};

struct ArrayType {
  struct DataType *element_type;
  uint64_t length;
};

struct DataType {
  enum DataTypeKind kind;
  union {
    struct FunctionType function;
    struct PrimitiveType primitive;
    struct DataType *pointer_inner;
    struct StructType structure;
    struct ArrayType array;
    struct StructDefinitionType structure_definition;
    struct ModuleDefinitionType *module_definition;
    struct ModuleType *module;
  } value;
  struct DataType *next;
};

struct DataTypeTable {
  struct DataType *head;
};

enum SymbolLocation {
  SL_MODULE,
  SL_ARGUMENT,
  SL_LOCAL,
};

struct SymbolTableEntry {
  struct LunaString symbol;
  struct DataType *type;
  LLVMValueRef llvm_value;
  LLVMTypeRef llvm_structure_type;
  struct SymbolTableEntry *next;
  enum MemorySegment memory_segment;
  enum SymbolLocation symbol_location;
  uint16_t index;
};

// NOTE: STT_SCOPE is for control flow like if, for, while, etc...
enum SymbolTableType { STT_MOD, STT_FN, STT_SCOPE, STT_STRUCT };

struct SymbolTable {
  struct SymbolTableEntry *head;
  struct SymbolTable *parent;
  uint16_t current_index;
  enum SymbolTableType type;
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
struct DataType *make_structure_def_data_type(struct ArenaAllocator *allocator,
                                              struct StructDefinitionType type);
struct DataType *make_structure_data_type(struct ArenaAllocator *allocator,
                                          struct StructType type);
struct DataType *make_function_data_type(struct ArenaAllocator *allocator,
                                         struct FunctionArgumentNode *arguments,
                                         struct DataType *return_type,
                                         struct LunaString *extern_name,
                                         bool is_variadic);

void print_data_type(struct DataType *data_type);

struct StructDefinitionExpressionNode *
get_or_resolve_struct_definition_from_type(struct DataType *type,
                                           struct SymbolTable *symbol_table);

struct ExpressionNode;
struct DataType *infer_type(struct Annotator *annotator,
                            struct ExpressionNode *expr);

struct StructFieldAccessExpressionNode;
struct DataType *infer_type_of_field_access(
    struct StructFieldAccessExpressionNode *field_accessor,
    struct SymbolTable *symbol_table);

void print_symbol_table(struct LunaString name,
                        struct SymbolTable *symbol_table);

struct DataType *
make_integer_primitive_data_type(struct ArenaAllocator *allocator,
                                 uint16_t bitwidth, bool is_signed);
struct DataType *make_array_data_type(struct ArenaAllocator *allocator,
                                      struct DataType *element_type,
                                      uint64_t length);
struct DataType *make_pointer_data_type(struct ArenaAllocator *allocator,
                                        struct DataType *inner);

struct DataType *get_common_type(struct ArenaAllocator *allocator,
                                 struct DataType *left, struct DataType *right);

bool scoped_symbols_equal(struct ScopedSymbolLiteralNode *left,
                          struct ScopedSymbolLiteralNode *right);
struct SymbolTableEntry *
lookup_scoped_symbol_in(struct ScopedSymbolLiteralNode *scoped_symbol,
                        struct SymbolTable *symbol_table);

void print_scoped_symbol(struct ScopedSymbolLiteralNode *scoped_symbol);

struct SymbolTable *
find_parent_table(struct SymbolTable *symbol_table,
                  enum SymbolTableType new_symbol_table_type);
#endif
