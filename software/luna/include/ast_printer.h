#ifndef AST_PRINTER_H
#define AST_PRINTER_H

#include "ast.h"
#include <stdint.h>

// AST pretty printing functions
void ast_print_module_statements(struct ModuleStatementNode *stmt, uint16_t indent);
void ast_print_function_statements(struct FunctionStatementNode *stmt, uint16_t indent);
void ast_print_expression(struct ExpressionNode *expr, uint16_t indent);
void ast_print_declaration(struct DeclarationStatementNode *decl, uint16_t indent);
void ast_print_assignment(struct AssignStatementNode *assign, uint16_t indent);

// Helper functions
void ast_print_indent(uint16_t indent);
void ast_print_data_type(struct DataType *type);
const char* ast_get_binary_op_name(enum BinaryExpressionType type);
const char* ast_get_expression_type_name(enum ExpressionType type);

#endif 
