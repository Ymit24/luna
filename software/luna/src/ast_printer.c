#include "ast_printer.h"
#include "annotator.h"
#include "ast.h"
#include "pretty_print.h"
#include <stdio.h>

void ast_print_indent(uint16_t indent) {
  for (uint16_t i = 0; i < indent; i++) {
    printf("  ");
  }
}

const char* ast_get_binary_op_name(enum BinaryExpressionType type) {
  switch (type) {
  case BIN_EXPR_ADD: return "+";
  case BIN_EXPR_SUB: return "-";
  case BIN_EXPR_MUL: return "*";
  case BIN_EXPR_DIV: return "/";
  default: return "?";
  }
}

const char* ast_get_expression_type_name(enum ExpressionType type) {
  switch (type) {
  case EXPR_BINARY: return "Binary";
  case EXPR_INTEGER_LITERAL: return "Integer";
  case EXPR_SYMBOL_LITERAL: return "Symbol";
  case EXPR_FN_DEF: return "Function";
  default: return "Unknown";
  }
}

void ast_print_data_type(struct DataType *type) {
  if (type == NULL) {
    printf(RED "null" RESET);
    return;
  }
  
  switch (type->kind) {
  case DTK_PRIMITIVE:
    switch (type->value.primitive) {
    case P_INT:
      printf(GREEN "int" RESET);
      break;
    case P_BOOL:
      printf(GREEN "bool" RESET);
      break;
    default:
      printf(RED "unknown_primitive" RESET);
    }
    break;
  case DTK_FUNCTION:
    printf(YELLOW "function" RESET);
    if (type->value.function.return_type) {
      printf(" -> ");
      ast_print_data_type(type->value.function.return_type);
    }
    break;
  default:
    printf(RED "unknown_type" RESET);
  }
}

void ast_print_expression(struct ExpressionNode *expr, uint16_t indent) {
  if (expr == NULL) {
    ast_print_indent(indent);
    printf(RED "null expression\n" RESET);
    return;
  }

  ast_print_indent(indent);
  printf(BLUE "📄 Expression" RESET " (" CYAN "%s" RESET "):\n", 
         ast_get_expression_type_name(expr->type));

  switch (expr->type) {
  case EXPR_INTEGER_LITERAL:
    ast_print_indent(indent + 1);
    printf(GREEN "🔢 Value: %d\n" RESET, expr->node.integer->value);
    break;

  case EXPR_SYMBOL_LITERAL:
    ast_print_indent(indent + 1);
    printf(MAGENTA "🏷️  Symbol: %.*s\n" RESET, 
           expr->node.symbol->value.length, expr->node.symbol->value.data);
    break;

  case EXPR_BINARY:
    ast_print_indent(indent + 1);
    printf(YELLOW "⚡ Operator: %s\n" RESET, 
           ast_get_binary_op_name(expr->node.binary->type));
    
    ast_print_indent(indent + 1);
    printf(DIM "├─ Left:\n" RESET);
    ast_print_expression(expr->node.binary->left, indent + 2);
    
    ast_print_indent(indent + 1);
    printf(DIM "└─ Right:\n" RESET);
    ast_print_expression(expr->node.binary->right, indent + 2);
    break;

  case EXPR_FN_DEF:
    ast_print_indent(indent + 1);
    printf(CYAN "🎯 Return Type: " RESET);
    ast_print_data_type(expr->node.fn_def->return_type);
    printf("\n");
    
    ast_print_indent(indent + 1);
    printf(DIM "└─ Body:\n" RESET);
    ast_print_function_statements(expr->node.fn_def->body, indent + 2);
    break;
  }
}

void ast_print_declaration(struct DeclarationStatementNode *decl, uint16_t indent) {
  if (decl == NULL) {
    ast_print_indent(indent);
    printf(RED "null declaration\n" RESET);
    return;
  }

  ast_print_indent(indent);
  printf(GREEN "📝 Declaration" RESET " (" MAGENTA "%.*s" RESET "):\n",
         decl->symbol.length, decl->symbol.data);

  if (decl->has_type && decl->data_type) {
    ast_print_indent(indent + 1);
    printf(CYAN "🏷️  Type: " RESET);
    ast_print_data_type(decl->data_type);
    printf("\n");
  }

  ast_print_indent(indent + 1);
  printf(BLUE "📊 Const: %s\n" RESET, decl->is_const ? "true" : "false");

  ast_print_indent(indent + 1);
  printf(DIM "└─ Expression:\n" RESET);
  ast_print_expression(decl->expression, indent + 2);
}

void ast_print_assignment(struct AssignStatementNode *assign, uint16_t indent) {
  if (assign == NULL) {
    ast_print_indent(indent);
    printf(RED "null assignment\n" RESET);
    return;
  }

  ast_print_indent(indent);
  printf(YELLOW "📤 Assignment" RESET " (" MAGENTA "%.*s" RESET "):\n",
         assign->symbol.length, assign->symbol.data);

  ast_print_indent(indent + 1);
  printf(DIM "└─ Expression:\n" RESET);
  ast_print_expression(assign->expression, indent + 2);
}

void ast_print_function_statements(struct FunctionStatementNode *stmt, uint16_t indent) {
  if (stmt == NULL) {
    ast_print_indent(indent);
    printf(DIM "(empty function body)\n" RESET);
    return;
  }

  struct FunctionStatementNode *current = stmt;
  uint16_t count = 0;

  while (current != NULL) {
    count++;
    current = current->next;
  }

  current = stmt;
  uint16_t index = 0;

  while (current != NULL) {
    ast_print_indent(indent);
    const char *connector = (index == count - 1) ? "└─" : "├─";
    printf(DIM "%s " RESET, connector);

    switch (current->type) {
    case FN_STMT_LET:
      printf(GREEN "Let Statement:\n" RESET);
      ast_print_declaration(current->node.decl, indent + 1);
      break;
    case FN_STMT_CONST:
      printf(BLUE "Const Statement:\n" RESET);
      ast_print_declaration(current->node.decl, indent + 1);
      break;
    case FN_STMT_ASSIGN:
      printf(YELLOW "Assignment Statement:\n" RESET);
      ast_print_assignment(current->node.assign, indent + 1);
      break;
    }

    current = current->next;
    index++;
  }
}

void ast_print_module_statements(struct ModuleStatementNode *stmt, uint16_t indent) {
  if (stmt == NULL) {
    ast_print_indent(indent);
    printf(DIM "(empty module)\n" RESET);
    return;
  }

  struct ModuleStatementNode *current = stmt;
  uint16_t count = 0;

  // Count statements
  while (current != NULL) {
    count++;
    current = current->next;
  }

  current = stmt;
  uint16_t index = 0;

  while (current != NULL) {
    ast_print_indent(indent);
    const char *connector = (index == count - 1) ? "└─" : "├─";
    printf(DIM "%s " RESET, connector);

    switch (current->type) {
    case MOD_STMT_LET:
      printf(GREEN "Module Let Statement:\n" RESET);
      ast_print_declaration(current->node.decl, indent + 1);
      break;
    case MOD_STMT_CONST:
      printf(BLUE "Module Const Statement:\n" RESET);
      ast_print_declaration(current->node.decl, indent + 1);
      break;
    }

    current = current->next;
    index++;
    
    if (current != NULL) {
      printf("\n");
    }
  }
} 
