#ifndef AST_H
#define AST_H
#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "annotator.h"
#include "arena_allocator.h"
#include "luna_string.h"

enum ModuleStatementType {
  MOD_STMT_LET,
  MOD_STMT_CONST,
};

enum FunctionStatementType {
  FN_STMT_LET,
  FN_STMT_CONST,
  FN_STMT_ASSIGN,
};

struct DeclarationStatementNode {
  struct LunaString symbol;
  struct DataType *data_type; // NOTE: May be unused (e.g. uninitialized)
  struct ExpressionNode *expression;
  bool has_type;
  bool is_const;
};

struct AssignStatementNode {
  struct LunaString symbol;
  struct ExpressionNode *expression;
};

struct ModuleStatementNode {
  enum ModuleStatementType type;
  union {
    struct DeclarationStatementNode *decl;
  } node;
  struct ModuleStatementNode *next;
};

struct FunctionStatementNode {
  enum FunctionStatementType type;
  union {
    struct DeclarationStatementNode *decl;
    struct AssignStatementNode *assign;
  } node;
  struct FunctionStatementNode *next;
};

enum ExpressionType {
  EXPR_BINARY,
  EXPR_INTEGER_LITERAL,
  EXPR_SYMBOL_LITERAL,
  EXPR_FN_DEF,
};

struct ExpressionNode {
  enum ExpressionType type;
  union {
    struct IntegerLiteralNode *integer;
    struct SymbolLiteralNode *symbol;
    struct BinaryExpressionNode *binary;
    struct FunctionDefinitionExpressionNode *fn_def;
  } node;
};

struct IntegerLiteralNode {
  uint16_t value;
};

struct SymbolLiteralNode {
  struct LunaString value;
};

struct FunctionDefinitionExpressionNode {
  struct FunctionStatementNode *body;
  struct DataType *return_type;
};

enum BinaryExpressionType {
  BIN_EXPR_ADD,
  BIN_EXPR_SUB,
  BIN_EXPR_MUL,
  BIN_EXPR_DIV
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

#endif
