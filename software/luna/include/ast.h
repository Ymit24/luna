#ifndef AST_H
#define AST_H

#include <stdbool.h>
#include <stdint.h>

#include "annotator.h"
#include "arena_allocator.h"
#include "luna_string.h"
#include "source_spans.h"

enum ModuleStatementType {
  MOD_STMT_LET,
  MOD_STMT_CONST,
};

enum FunctionStatementType {
  FN_STMT_LET,
  FN_STMT_CONST,
  FN_STMT_ASSIGN,
  FN_STMT_RETURN,
  FN_STMT_FN_CALL, // TODO: maybe this becomes general expression
  FN_STMT_IF,
  FN_STMT_WHILE,
};

struct FunctionArgumentNode {
  struct LunaString symbol;
  struct DataType *data_type;
  struct FunctionArgumentNode *next;
};

struct DeclarationStatementNode {
  struct LunaString symbol;
  struct DataType *data_type; // NOTE: May be unused (e.g. uninitialized)
  struct ExpressionNode *expression;
  struct Span span;
  bool has_type;
  bool is_const;
};

struct AssignStatementNode {
  struct ExpressionNode *source_expression;
  struct ExpressionNode *result_expression;
};

struct ModuleNode {
  struct ModuleStatementNode *statements;
  struct SymbolTable symbol_table;
};

struct ModuleStatementNode {
  enum ModuleStatementType type;
  union {
    struct DeclarationStatementNode *decl;
  } node;
  struct Span span;
  struct ModuleStatementNode *next;
};

struct ReturnStatementNode {
  struct ExpressionNode *expression;
};

struct FunctionStatementNode {
  enum FunctionStatementType type;
  union {
    struct DeclarationStatementNode *decl;
    struct AssignStatementNode *assign;
    struct ReturnStatementNode *ret;
    struct FunctionCallExpressionNode *fn_call;
    struct IfStatementNode *if_stmt;
    struct WhileStatementNode *while_stmt;
  } node;
  struct FunctionStatementNode *next;
};

struct IfStatementNode {
  struct ExpressionNode *condition;
  struct FunctionStatementNode *body;
  struct IfStatementNode *next;
  struct SymbolTable symbol_table;
};

struct WhileStatementNode {
  struct ExpressionNode *condition;
  struct FunctionStatementNode *body;
  struct SymbolTable symbol_table;
};

enum ExpressionType {
  EXPR_BINARY,
  EXPR_INTEGER_LITERAL,
  EXPR_SYMBOL_LITERAL,
  EXPR_STRING_LITERAL,
  EXPR_FN_DEF,
  EXPR_FN_CALL,
  EXPR_REF,
  EXPR_DEREF,
  EXPR_STRUCT_DEF,
  EXPR_STRUCT_INIT,
  EXPR_FIELD_ACCESS,
  EXPR_CAST,
  EXPR_ARRAY_INITIALIZER,
  EXPR_MOD_DEF,
  EXPR_VALUESIZE,
  EXPR_TYPESIZE
};

struct CastExpressionNode {
  struct ExpressionNode *expr;
  struct DataType *type;
};

struct ArrayInitializerExpressionNode {
  struct ExpressionNode *initializer;
  struct ArrayInitializerExpressionNode *next;
};

struct ExpressionNode {
  enum ExpressionType type;
  struct Span span;
  union {
    struct IntegerLiteralNode *integer;
    struct ScopedSymbolLiteralNode *scoped_symbol;
    struct StringLiteralNode *string;
    struct BinaryExpressionNode *binary;
    struct FunctionDefinitionExpressionNode *fn_def;
    struct FunctionCallExpressionNode *fn_call;
    struct StructFieldAccessExpressionNode *ref_symbol;
    struct ExpressionNode *deref;
    struct StructDefinitionExpressionNode *struct_def;
    struct StructInitializationExpressionNode *struct_init;
    struct StructFieldAccessExpressionNode *struct_field_access;
    struct CastExpressionNode *cast;
    struct ExpressionNode *valuesize;
    struct DataType *typesize;
    struct ArrayInitializerExpressionNode *array_initializers;
    struct ModuleNode *module_definition;
  } node;
};

struct IntegerLiteralNode {
  int64_t value;
};

struct SymbolLiteralNode {
  struct LunaString value;
  struct Span span;
};

struct ScopedSymbolLiteralNode {
  struct SymbolLiteralNode *symbol;
  struct ScopedSymbolLiteralNode *next;
  struct Span span;
  uint8_t remaining;
};

struct StringLiteralNode {
  struct LunaString value;
};

struct StructFieldDefinitionNode {
  struct LunaString name;
  struct DataType *type;
  struct StructFieldDefinitionNode *next;
};

struct StructDefinitionExpressionNode {
  struct StructFieldDefinitionNode *fields;
  struct SymbolTable symbol_table;
  LLVMTypeRef llvm_structure_type;
};

struct StructFieldAccessInnerExpressionNode {
  struct SymbolLiteralNode symbol;
  struct StructFieldAccessInnerExpressionNode *next;
};

struct StructFieldAccessExpressionNode {
  struct ScopedSymbolLiteralNode *root_symbol;
  struct StructFieldAccessInnerExpressionNode *next;
};

struct StructFieldInitializerExpressionNode {
  struct LunaString name;
  struct ExpressionNode *expression;
  struct StructFieldInitializerExpressionNode *next;
};

struct StructInitializationExpressionNode {
  struct ScopedSymbolLiteralNode *name;
  struct StructFieldInitializerExpressionNode *fields;
};

struct FunctionDefinitionExpressionNode {
  struct FunctionStatementNode *body;
  struct DataType *function_type;
  struct SymbolTable symbol_table;
};

struct FunctionCallArgumentExpressionsNode {
  struct ExpressionNode *argument;
  struct FunctionCallArgumentExpressionsNode *next;
};

struct FunctionCallExpressionNode {
  struct ScopedSymbolLiteralNode *name;
  struct FunctionCallArgumentExpressionsNode *arguments;
};

enum BinaryExpressionType {
  BIN_EXPR_ADD,
  BIN_EXPR_SUB,
  BIN_EXPR_MUL,
  BIN_EXPR_DIV,
  BIN_EXPR_LT,
  BIN_EXPR_GT,
  BIN_EXPR_EQ,
  BIN_EXPR_NEQ,
  BIN_EXPR_LEQ,
  BIN_EXPR_GEQ,
};

struct BinaryExpressionNode {
  enum BinaryExpressionType type;
  struct ExpressionNode *left, *right;
};

struct IntegerLiteralNode *
ast_make_integer_literal(struct ArenaAllocator *allocator, uint16_t value);

struct BinaryExpressionNode *ast_make_binary_expression(
    struct ArenaAllocator *allocator, enum BinaryExpressionType type,
    struct ExpressionNode *left, struct ExpressionNode *right);

struct ExpressionNode *
ast_promote_expression_node(struct ArenaAllocator *allocator,
                            struct ExpressionNode expr);

void *ast_promote(struct ArenaAllocator *allocator, void *src, uint8_t size);

size_t count_array_initializer_length(
    struct ArrayInitializerExpressionNode *initiailzier);

char *binary_expression_type_to_string(enum BinaryExpressionType type);
#endif
