#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "annotator.h"
#include "arena_allocator.h"
#include "ast.h"
#include "lexer.h"
#include "llvm-backend/code_generator.h"
#include "luna_string.h"
#include "module_symbol_table_builder.h"
#include "parser.h"
#include "token.h"
#include "llvm-c/Core.h"

struct LineMap {
  uint16_t *line_offsets;
  uint16_t num_lines;
};

enum ContainingNodeType {
  NODE_NONE,
  NODE_MODULE,
  NODE_FUNCTION,
  NODE_VARIABLE,
  NODE_EXPRESSION,
};

struct ContainingNode {
  enum ContainingNodeType type;
  struct SourceLocation location;
  struct LunaString name;
  union {
    struct ModuleNode *module;
    struct FunctionDefinitionExpressionNode *function;
    struct DeclarationStatementNode *variable;
    struct ExpressionNode *expression;
  } node;
};

static struct LineMap build_line_map(struct LunaString source,
                                     struct ArenaAllocator *alloc) {
  uint16_t line_count = 1;
  for (uint16_t i = 0; i < source.length; i++) {
    if (source.data[i] == '\n') {
      line_count++;
    }
  }

  uint16_t *offsets = arena_alloc(alloc, sizeof(uint16_t) * line_count);
  offsets[0] = 0;
  uint16_t current_line = 1;
  for (uint16_t i = 0; i < source.length; i++) {
    if (source.data[i] == '\n') {
      offsets[current_line++] = i + 1;
    }
  }

  return (struct LineMap){.line_offsets = offsets, .num_lines = line_count};
}

static uint16_t line_col_to_byte_offset(struct LineMap map, uint16_t line,
                                        uint16_t col,
                                        struct LunaString source) {
  if (line >= map.num_lines) {
    return source.length;
  }
  uint16_t offset = map.line_offsets[line] + col;
  if (offset > source.length) {
    return source.length;
  }
  return offset;
}

static bool position_in_span(uint16_t pos, struct SourceLocation loc) {
  return loc.start <= pos && pos <= loc.end;
}

static bool should_update_containing_node(uint16_t pos,
                                        struct SourceLocation new_loc,
                                        struct SourceLocation current_best_loc) {
  if (current_best_loc.start == 0 && current_best_loc.end == 0) {
    return true;
  }
  uint16_t new_span = new_loc.end - new_loc.start;
  uint16_t current_span = current_best_loc.end - current_best_loc.start;
  return new_span < current_span && position_in_span(pos, new_loc);
}

static void find_containing_in_expression(struct ExpressionNode *expr,
                                         uint16_t pos,
                                         struct ContainingNode *result);
static void find_containing_in_statement(struct FunctionStatementNode *stmt,
                                       uint16_t pos,
                                       struct ContainingNode *result);
static void find_containing_in_module_statement(
    struct ModuleStatementNode *stmt, uint16_t pos,
    struct ContainingNode *result);

static void find_containing_in_expression(struct ExpressionNode *expr,
                                         uint16_t pos,
                                         struct ContainingNode *result) {
  if (!expr) {
    return;
  }

  if (!position_in_span(pos, expr->location)) {
    return;
  }

  switch (expr->type) {
  case EXPR_FN_DEF: {
    if (should_update_containing_node(pos, expr->location, result->location)) {
      result->type = NODE_FUNCTION;
      result->location = expr->location;
      result->node.function = expr->node.fn_def;
      result->name = string_make((char *)"<function>");
    }
    if (expr->node.fn_def->body) {
      struct FunctionStatementNode *stmt = expr->node.fn_def->body;
      while (stmt) {
        find_containing_in_statement(stmt, pos, result);
        stmt = stmt->next;
      }
    }
    break;
  }
  case EXPR_MOD_DEF: {
    if (should_update_containing_node(pos, expr->location, result->location)) {
      result->type = NODE_MODULE;
      result->location = expr->location;
      result->node.module = expr->node.module_definition;
      result->name = string_make((char *)"<module>");
    }
    if (expr->node.module_definition->statements) {
      struct ModuleStatementNode *stmt = expr->node.module_definition->statements;
      while (stmt) {
        find_containing_in_module_statement(stmt, pos, result);
        stmt = stmt->next;
      }
    }
    break;
  }
  case EXPR_SYMBOL_LITERAL:
  case EXPR_INTEGER_LITERAL:
  case EXPR_STRING_LITERAL:
  case EXPR_BINARY:
  case EXPR_REF:
  case EXPR_DEREF:
  case EXPR_NOT:
  case EXPR_CAST:
  case EXPR_STRUCT_DEF:
  case EXPR_STRUCT_INIT:
  case EXPR_FIELD_ACCESS:
  case EXPR_ARRAY_INITIALIZER:
  case EXPR_FN_CALL:
  case EXPR_VALUESIZE:
  case EXPR_TYPESIZE: {
    if (should_update_containing_node(pos, expr->location, result->location)) {
      result->type = NODE_EXPRESSION;
      result->location = expr->location;
      result->node.expression = expr;
      const char *type_name = "unknown";
      switch (expr->type) {
      case EXPR_SYMBOL_LITERAL:
        type_name = "symbol";
        break;
      case EXPR_INTEGER_LITERAL:
        type_name = "integer";
        break;
      case EXPR_STRING_LITERAL:
        type_name = "string";
        break;
      case EXPR_BINARY:
        type_name = "binary";
        break;
      case EXPR_REF:
        type_name = "ref";
        break;
      case EXPR_DEREF:
        type_name = "deref";
        break;
      case EXPR_NOT:
        type_name = "not";
        break;
      case EXPR_CAST:
        type_name = "cast";
        break;
      case EXPR_FN_CALL:
        type_name = "call";
        break;
      case EXPR_STRUCT_DEF:
        type_name = "struct_def";
        break;
      case EXPR_STRUCT_INIT:
        type_name = "struct_init";
        break;
      case EXPR_FIELD_ACCESS:
        type_name = "field_access";
        break;
      case EXPR_ARRAY_INITIALIZER:
        type_name = "array";
        break;
      case EXPR_VALUESIZE:
        type_name = "valuesize";
        break;
      case EXPR_TYPESIZE:
        type_name = "typesize";
        break;
      default:
        type_name = "unknown";
        break;
      }
      result->name = string_make((char *)type_name);
    }
    break;
  }
  }
}

static void find_containing_in_statement(struct FunctionStatementNode *stmt,
                                       uint16_t pos,
                                       struct ContainingNode *result) {
  if (!stmt) {
    return;
  }

  if (!position_in_span(pos, stmt->location)) {
    return;
  }

  switch (stmt->type) {
  case FN_STMT_LET:
  case FN_STMT_CONST:
  case FN_STMT_ASSIGN: {
    struct DeclarationStatementNode *decl =
        (stmt->type == FN_STMT_LET || stmt->type == FN_STMT_CONST)
            ? stmt->node.decl
            : NULL;
    if (decl && decl->expression) {
      find_containing_in_expression(decl->expression, pos, result);
    }
    if (decl && should_update_containing_node(pos, stmt->location,
                                             result->location)) {
      result->type = NODE_VARIABLE;
      result->location = stmt->location;
      result->node.variable = decl;
      result->name = decl->symbol;
    }
    break;
  }
  case FN_STMT_RETURN:
    if (stmt->node.ret && stmt->node.ret->expression) {
      find_containing_in_expression(stmt->node.ret->expression, pos, result);
    }
    break;
  case FN_STMT_FN_CALL:
    if (stmt->node.fn_call) {
      struct FunctionCallExpressionNode *call = stmt->node.fn_call;
      if (call->arguments) {
        struct FunctionCallArgumentExpressionsNode *arg = call->arguments;
        while (arg) {
          find_containing_in_expression(arg->argument, pos, result);
          arg = arg->next;
        }
      }
    }
    break;
  case FN_STMT_IF:
    if (stmt->node.if_stmt->condition) {
      find_containing_in_expression(stmt->node.if_stmt->condition, pos, result);
    }
    if (stmt->node.if_stmt->body) {
      struct FunctionStatementNode *s = stmt->node.if_stmt->body;
      while (s) {
        find_containing_in_statement(s, pos, result);
        s = s->next;
      }
    }
    if (stmt->node.if_stmt->next) {
      struct IfStatementNode *else_chain = stmt->node.if_stmt->next;
      while (else_chain) {
        if (else_chain->condition) {
          find_containing_in_expression(else_chain->condition, pos, result);
        }
        if (else_chain->body) {
          struct FunctionStatementNode *s = else_chain->body;
          while (s) {
            find_containing_in_statement(s, pos, result);
            s = s->next;
          }
        }
        else_chain = else_chain->next;
      }
    }
    break;
  case FN_STMT_WHILE:
    if (stmt->node.while_stmt->condition) {
      find_containing_in_expression(stmt->node.while_stmt->condition, pos, result);
    }
    if (stmt->node.while_stmt->body) {
      struct FunctionStatementNode *s = stmt->node.while_stmt->body;
      while (s) {
        find_containing_in_statement(s, pos, result);
        s = s->next;
      }
    }
    break;
  }
}

static void find_containing_in_module_statement(
    struct ModuleStatementNode *stmt, uint16_t pos,
    struct ContainingNode *result) {
  if (!stmt) {
    return;
  }

  if (!position_in_span(pos, stmt->location)) {
    return;
  }

  switch (stmt->type) {
  case MOD_STMT_LET:
  case MOD_STMT_CONST: {
    struct DeclarationStatementNode *decl = stmt->node.decl;
    if (decl && decl->expression) {
      find_containing_in_expression(decl->expression, pos, result);
    }
    if (decl && should_update_containing_node(pos, stmt->location,
                                             result->location)) {
      result->type = NODE_VARIABLE;
      result->location = stmt->location;
      result->node.variable = decl;
      result->name = decl->symbol;
    }
    break;
  }
  }
}

static struct ContainingNode
find_smallest_containing_node(struct ModuleStatementNode *ast,
                                 uint16_t byte_offset) {
  struct ContainingNode result = {0};
  struct ModuleStatementNode *stmt = ast;
  while (stmt) {
    find_containing_in_module_statement(stmt, byte_offset, &result);
    stmt = stmt->next;
  }
  return result;
}

static const char *node_type_to_string(enum ContainingNodeType type) {
  switch (type) {
  case NODE_NONE:
    return "none";
  case NODE_MODULE:
    return "module";
  case NODE_FUNCTION:
    return "function";
  case NODE_VARIABLE:
    return "variable";
  case NODE_EXPRESSION:
    return "expression";
  default:
    return "unknown";
  }
}

static void test_cursor_location(struct ModuleStatementNode *ast,
                                struct LunaString source, uint16_t line,
                                uint16_t col) {
  uint8_t arena_buffer[UINT16_MAX];
  struct ArenaAllocator local_alloc = arena_make(arena_buffer, sizeof(arena_buffer));
  struct LineMap map = build_line_map(source, &local_alloc);

  uint16_t line_0based = line;
  uint16_t col_0based = col;

  if (line_0based >= map.num_lines) {
    printf("Error: Line %d out of range (0-%d)\n", line_0based,
           map.num_lines - 1);
    return;
  }

  uint16_t byte_offset =
      line_col_to_byte_offset(map, line_0based, col_0based, source);

  printf("\n=== Cursor Location Query ===\n");
  printf("Cursor: line=%d, col=%d, byte_offset=%d\n", line_0based,
         col_0based, byte_offset);

  struct ContainingNode result =
      find_smallest_containing_node(ast, byte_offset);

  printf("\n--- Result ---\n");

  if (result.type == NODE_NONE) {
    printf("No node found at cursor location\n");
    return;
  }

  printf("Type: %s\n", node_type_to_string(result.type));
  printf("Span: %d-%d (length: %d)\n", result.location.start,
         result.location.end,
         result.location.end - result.location.start);

  if (result.name.length > 0) {
    printf("Name: %.*s\n", (int)result.name.length, result.name.data);
  }

  switch (result.type) {
  case NODE_FUNCTION:
    if (result.node.function && result.node.function->body) {
      printf("Function body has statements\n");
    }
    break;
  case NODE_MODULE:
    if (result.node.module) {
      printf("Module has statements\n");
    }
    break;
  case NODE_VARIABLE:
    if (result.node.variable) {
      printf("is_const: %s\n", result.node.variable->is_const ? "true" : "false");
      printf("has_type: %s\n",
             result.node.variable->has_type ? "true" : "false");
    }
    break;
  case NODE_EXPRESSION:
    if (result.node.expression) {
      const char *expr_type = "unknown";
      switch (result.node.expression->type) {
      case EXPR_SYMBOL_LITERAL:
        expr_type = "symbol";
        break;
      case EXPR_INTEGER_LITERAL:
        expr_type = "integer literal";
        break;
      case EXPR_STRING_LITERAL:
        expr_type = "string literal";
        break;
      case EXPR_BINARY:
        expr_type = "binary expression";
        break;
      case EXPR_FN_CALL:
        expr_type = "function call";
        break;
      case EXPR_FN_DEF:
        expr_type = "function definition";
        break;
      case EXPR_MOD_DEF:
        expr_type = "module definition";
        break;
      case EXPR_REF:
        expr_type = "reference";
        break;
      case EXPR_DEREF:
        expr_type = "dereference";
        break;
      case EXPR_NOT:
        expr_type = "not";
        break;
      case EXPR_CAST:
        expr_type = "cast";
        break;
      case EXPR_STRUCT_DEF:
        expr_type = "struct definition";
        break;
      case EXPR_STRUCT_INIT:
        expr_type = "struct initialization";
        break;
      case EXPR_FIELD_ACCESS:
        expr_type = "field access";
        break;
      case EXPR_ARRAY_INITIALIZER:
        expr_type = "array initializer";
        break;
      case EXPR_VALUESIZE:
        expr_type = "valuesize";
        break;
      case EXPR_TYPESIZE:
        expr_type = "typesize";
        break;
      default:
        expr_type = "unknown";
        break;
      }
      printf("Expression type: %s\n", expr_type);
    }
    break;
  default:
    break;
  }

  printf("=== End Query ===\n");
}

static void print_indent(int indent) {
  for (int i = 0; i < indent; i++) {
    printf("  ");
  }
}

static void print_tree_connector(int indent, bool is_last) {
  print_indent(indent);
  printf("%s ", is_last ? "└─" : "├─");
}

static void print_span(struct SourceLocation loc) {
  printf("[file=%.*s, span=%d-%d]", (int)loc.filename.length,
         loc.filename.data, loc.start, loc.end);
}

static void print_expression_node(struct ExpressionNode *node, int indent);

static void print_binary_node(struct BinaryExpressionNode *node, int indent) {
  print_tree_connector(indent, false);
  printf("left: ");
  print_expression_node(node->left, 0);
  printf("\n");

  print_tree_connector(indent, false);
  printf("right: ");
  print_expression_node(node->right, 0);
  printf("\n");
}

static void print_expression_node(struct ExpressionNode *node, int indent) {
  (void)indent;
  if (!node) {
    printf("(null)");
    return;
  }

  switch (node->type) {
  case EXPR_BINARY: {
    printf("ExpressionNode (EXPR_BINARY) ");
    print_span(node->location);
    printf("\n");
    print_binary_node(node->node.binary, indent + 1);
    break;
  }
  case EXPR_INTEGER_LITERAL:
    printf("ExpressionNode (EXPR_INTEGER_LITERAL) ");
    print_span(node->location);
    printf(" value=%lld", (long long)node->node.integer->value);
    break;
  case EXPR_SYMBOL_LITERAL:
    printf("ExpressionNode (EXPR_SYMBOL_LITERAL) ");
    print_span(node->location);
    printf(" symbol=%.*s", (int)node->node.scoped_symbol->symbol->value.length,
           node->node.scoped_symbol->symbol->value.data);
    break;
  case EXPR_STRING_LITERAL:
    printf("ExpressionNode (EXPR_STRING_LITERAL) ");
    print_span(node->location);
    printf(" value=%.*s", (int)node->node.string->value.length,
           node->node.string->value.data);
    break;
  case EXPR_FN_DEF:
    printf("ExpressionNode (EXPR_FN_DEF) ");
    print_span(node->location);
    printf("\n");
    print_tree_connector(indent + 1, true);
    printf("function_definition\n");
    break;
  case EXPR_FN_CALL:
    printf("ExpressionNode (EXPR_FN_CALL) ");
    print_span(node->location);
    printf("\n");
    break;
  case EXPR_REF:
    printf("ExpressionNode (EXPR_REF) ");
    print_span(node->location);
    printf("\n");
    print_tree_connector(indent + 1, true);
    printf("inner: ");
    print_expression_node(node->node.ref, 0);
    printf("\n");
    break;
  case EXPR_DEREF:
    printf("ExpressionNode (EXPR_DEREF) ");
    print_span(node->location);
    printf("\n");
    print_tree_connector(indent + 1, true);
    printf("inner: ");
    print_expression_node(node->node.deref, 0);
    printf("\n");
    break;
  case EXPR_NOT:
    printf("ExpressionNode (EXPR_NOT) ");
    print_span(node->location);
    printf("\n");
    print_tree_connector(indent + 1, true);
    printf("inner: ");
    print_expression_node(node->node.not, 0);
    printf("\n");
    break;
  case EXPR_STRUCT_DEF:
    printf("ExpressionNode (EXPR_STRUCT_DEF) ");
    print_span(node->location);
    printf("\n");
    break;
  case EXPR_STRUCT_INIT:
    printf("ExpressionNode (EXPR_STRUCT_INIT) ");
    print_span(node->location);
    printf("\n");
    break;
  case EXPR_FIELD_ACCESS:
    printf("ExpressionNode (EXPR_FIELD_ACCESS) ");
    print_span(node->location);
    printf("\n");
    print_tree_connector(indent + 1, false);
    printf("expression: ");
    print_expression_node(node->node.struct_field_access->expression, 0);
    printf("\n");
    print_tree_connector(indent + 1, true);
    printf("field=%.*s\n",
           (int)node->node.struct_field_access->symbol.length,
           node->node.struct_field_access->symbol.data);
    break;
  case EXPR_CAST:
    printf("ExpressionNode (EXPR_CAST) ");
    print_span(node->location);
    printf("\n");
    print_tree_connector(indent + 1, true);
    printf("inner: ");
    print_expression_node(node->node.cast->expr, 0);
    printf("\n");
    break;
  case EXPR_ARRAY_INITIALIZER:
    printf("ExpressionNode (EXPR_ARRAY_INITIALIZER) ");
    print_span(node->location);
    printf("\n");
    break;
  case EXPR_VALUESIZE:
    printf("ExpressionNode (EXPR_VALUESIZE) ");
    print_span(node->location);
    printf("\n");
    print_tree_connector(indent + 1, true);
    printf("expr: ");
    print_expression_node(node->node.valuesize, 0);
    printf("\n");
    break;
  case EXPR_TYPESIZE:
    printf("ExpressionNode (EXPR_TYPESIZE) ");
    print_span(node->location);
    printf("\n");
    break;
  case EXPR_MOD_DEF:
    printf("ExpressionNode (EXPR_MOD_DEF) ");
    print_span(node->location);
    printf("\n");
    break;
  default:
    printf("ExpressionNode (UNKNOWN) ");
    print_span(node->location);
    break;
  }
}

static void print_function_statement_node(struct FunctionStatementNode *stmt,
                                       int indent, bool is_last);

static void print_function_statement_list(struct FunctionStatementNode *head,
                                        int indent) {
  struct FunctionStatementNode *curr = head;
  while (curr) {
    struct FunctionStatementNode *next = curr->next;
    print_function_statement_node(curr, indent, next == NULL);
    curr = next;
  }
}

static void print_function_statement_node(struct FunctionStatementNode *stmt,
                                       int indent, bool is_last) {
  if (!stmt) {
    print_tree_connector(indent, is_last);
    printf("(null)\n");
    return;
  }

  const char *type_str;
  switch (stmt->type) {
  case FN_STMT_LET:
    type_str = "FN_STMT_LET";
    break;
  case FN_STMT_CONST:
    type_str = "FN_STMT_CONST";
    break;
  case FN_STMT_ASSIGN:
    type_str = "FN_STMT_ASSIGN";
    break;
  case FN_STMT_RETURN:
    type_str = "FN_STMT_RETURN";
    break;
  case FN_STMT_FN_CALL:
    type_str = "FN_STMT_FN_CALL";
    break;
  case FN_STMT_IF:
    type_str = "FN_STMT_IF";
    break;
  case FN_STMT_WHILE:
    type_str = "FN_STMT_WHILE";
    break;
  default:
    type_str = "UNKNOWN";
    break;
  }

  print_tree_connector(indent, is_last);
  printf("FunctionStatementNode (%s) ", type_str);
  print_span(stmt->location);
  printf("\n");

  switch (stmt->type) {
  case FN_STMT_LET:
  case FN_STMT_CONST:
    print_tree_connector(indent + 1, false);
    printf("symbol: %.*s\n", (int)stmt->node.decl->symbol.length,
           stmt->node.decl->symbol.data);
    if (stmt->node.decl->has_type) {
      print_tree_connector(indent + 1, false);
      printf("has_type: true\n");
    } else {
      print_tree_connector(indent + 1, false);
      printf("has_type: false\n");
    }
    print_tree_connector(indent + 1, false);
    printf("is_const: %s\n", stmt->type == FN_STMT_CONST ? "true" : "false");
    print_tree_connector(indent + 1, true);
    printf("expression: ");
    print_expression_node(stmt->node.decl->expression, 0);
    printf("\n");
    break;

  case FN_STMT_RETURN:
    if (stmt->node.ret->expression) {
      print_tree_connector(indent + 1, true);
      printf("expression: ");
      print_expression_node(stmt->node.ret->expression, 0);
      printf("\n");
    } else {
      print_tree_connector(indent + 1, true);
      printf("expression: (void)\n");
    }
    break;

  case FN_STMT_FN_CALL:
    print_tree_connector(indent + 1, true);
    printf("function_call\n");
    break;

  case FN_STMT_IF: {
    print_tree_connector(indent + 1, false);
    printf("condition: ");
    print_expression_node(stmt->node.if_stmt->condition, 0);
    printf("\n");
    print_tree_connector(indent + 1, false);
    printf("body:\n");
    print_function_statement_list(stmt->node.if_stmt->body, indent + 2);
    if (stmt->node.if_stmt->next) {
      print_tree_connector(indent + 1, true);
      printf("else:\n");
      struct IfStatementNode *curr = stmt->node.if_stmt->next;
      while (curr) {
        if (curr->condition) {
          print_tree_connector(indent + 2, curr->next == NULL);
          printf("condition: ");
          print_expression_node(curr->condition, 0);
          printf("\n");
          print_tree_connector(indent + 2, curr->next == NULL);
          printf("body:\n");
          print_function_statement_list(curr->body, indent + 3);
        } else {
          print_tree_connector(indent + 2, true);
          printf("body:\n");
          print_function_statement_list(curr->body, indent + 3);
        }
        curr = curr->next;
      }
    }
    break;
  }

  case FN_STMT_WHILE:
    print_tree_connector(indent + 1, false);
    printf("condition: ");
    print_expression_node(stmt->node.while_stmt->condition, 0);
    printf("\n");
    print_tree_connector(indent + 1, true);
    printf("body:\n");
    print_function_statement_list(stmt->node.while_stmt->body, indent + 2);
    break;

  default:
    break;
  }
}

static void print_module_statement_node(struct ModuleStatementNode *stmt,
                                      int indent, bool is_last) {
  if (!stmt) {
    print_tree_connector(indent, is_last);
    printf("(null)\n");
    return;
  }

  const char *type_str;
  switch (stmt->type) {
  case MOD_STMT_LET:
    type_str = "MOD_STMT_LET";
    break;
  case MOD_STMT_CONST:
    type_str = "MOD_STMT_CONST";
    break;
  default:
    type_str = "UNKNOWN";
    break;
  }

  print_tree_connector(indent, is_last);
  printf("ModuleStatementNode (%s) ", type_str);
  print_span(stmt->location);
  printf("\n");

  print_tree_connector(indent + 1, false);
  printf("symbol: %.*s\n", (int)stmt->node.decl->symbol.length,
         stmt->node.decl->symbol.data);

  if (stmt->node.decl->has_type) {
    print_tree_connector(indent + 1, false);
    printf("has_type: true\n");
  } else {
    print_tree_connector(indent + 1, false);
    printf("has_type: false\n");
  }

  print_tree_connector(indent + 1, false);
  printf("is_const: %s\n", stmt->type == MOD_STMT_CONST ? "true" : "false");

  print_tree_connector(indent + 1, true);
  printf("expression: ");
  print_expression_node(stmt->node.decl->expression, 0);
  printf("\n");
}

static void print_module_statement_list(struct ModuleStatementNode *head,
                                      int indent) {
  (void)head;
  (void)indent;
  struct ModuleStatementNode *curr = head;
  while (curr) {
    struct ModuleStatementNode *next = curr->next;
    print_module_statement_node(curr, indent, next == NULL);
    curr = next;
  }
}

const char *VERSION = "0.1.3-dev";

struct LunaString read_file(const char *path, struct ArenaAllocator *alloc) {
  FILE *file = fopen(path, "rb");
  if (!file) {
    fprintf(stderr, "could not open file: %s\n", path);
    assert(0);
  }
  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  fseek(file, 0, SEEK_SET);
  char *buffer = arena_alloc(alloc, (uint16_t)size + 1);
  if (!buffer) {
    fprintf(stderr, "not enough arena space for source\n");
    assert(0);
  }
  size_t n = fread(buffer, 1, (size_t)size, file);
  fclose(file);
  buffer[n] = 0;
  return (struct LunaString){.data = buffer, .length = (uint16_t)n};
}

struct ModuleStatementNode *parse_source_file(struct ArenaAllocator *allocator,
                                               char *source_file,
                                               uint16_t cursor_line,
                                               uint16_t cursor_col) {

  struct LunaString src = read_file(source_file, allocator);
  struct LunaString filename = string_make(source_file);
  struct Lexer lexer = lexer_make(allocator, src, filename);

  struct Token toks[4096];
  uint16_t tok_index = 0;

  puts("tokens:");
  while (lexer_next(&lexer, &toks[tok_index++])) {
    struct Token t = toks[tok_index - 1];
    uint16_t span_len = t.location.end - t.location.start;
    printf("Token %d: file=%.*s, span=%d-%d (len=%d), text=\"%.*s\"\n",
           t.type,
           (int)t.location.filename.length, t.location.filename.data,
           t.location.start, t.location.end, span_len,
           (int)span_len, src.data + t.location.start);
  }
  puts("");

  printf("Found a total of %d tokens.\n", tok_index - 1);

  struct Parser parser = parser_make(allocator, toks, tok_index);

  struct ModuleStatementNode *stmt = parse_module_statements(&parser);

  puts("done parsing");
  puts("\n\nAST Structure:");
  print_module_statement_list(stmt, 0);

  if (cursor_line != (uint16_t)-1 && cursor_col != (uint16_t)-1) {
    test_cursor_location(stmt, src, cursor_line, cursor_col);
  }

  return stmt;
}

struct ModuleStatementNode *wrap_module(struct ArenaAllocator *allocator,
                                        struct ModuleStatementNode *old_root,
                                        struct LunaString file_name) {

  struct ModuleNode *file_module = ast_promote(allocator,
                                               &(struct ModuleNode){
                                                   .statements = old_root,
                                               },
                                               sizeof(struct ModuleNode));

  struct ExpressionNode *file_module_definition = ast_promote_expression_node(
      allocator,
      (struct ExpressionNode){.type = EXPR_MOD_DEF,
                              .node.module_definition = file_module});

  struct DeclarationStatementNode *root_decl =
      ast_promote(allocator,
                  &(struct DeclarationStatementNode){
                      .symbol = file_name,
                      .data_type = NULL,
                      .has_type = false,
                      .is_const = true,
                      .expression = file_module_definition,
                  },
                  sizeof(struct DeclarationStatementNode));

  struct ModuleStatementNode *new_root =
      ast_promote(allocator,
                  &(struct ModuleStatementNode){
                      .type = MOD_STMT_CONST,
                      .node.decl = root_decl,
                      .next = NULL,
                  },
                  sizeof(struct ModuleStatementNode));

  return new_root;
}

struct LunaString get_module_name_from_file(struct ArenaAllocator *allocator,
                                            char *file_path) {
  const char *last_slash = strrchr(file_path, '/');
  const char *result = last_slash ? last_slash + 1 : file_path;
  const char *last_dot = strrchr(result, '.');

  size_t size;
  if (last_dot && last_dot > result)
    size = (size_t)(last_dot - result);
  else
    size = strlen(result);

  char *new_str = arena_alloc(allocator, size + 1);
  memcpy(new_str, result, size);
  new_str[size] = '\0';

  return string_make(new_str);
}

int main(int argc, char **argv) {
  puts("Luna Compiler");

  uint8_t arena[UINT16_MAX * 16];

  struct ArenaAllocator allocator = arena_make(&arena, sizeof(arena));

  if (argc < 2) {
    fprintf(stderr, "usage: %s <source.luna> ...\n", argv[0]);
    return 1;
  }

  if (strcmp(argv[1], "-v") == 0) {
    printf("version: %s\n", VERSION);
    return 0;
  }

  struct ModuleStatementNode *new_root = NULL;
  struct ModuleStatementNode *current = NULL;
  uint16_t cursor_line = (uint16_t)-1;
  uint16_t cursor_col = (uint16_t)-1;

  int file_start_idx = 1;
  for (int i = 1; i < (int)argc; i++) {
    if (strcmp(argv[i], "--cursor") == 0 && i + 1 < argc) {
      char *cursor_arg = argv[++i];
      char *colon = strchr(cursor_arg, ':');
      if (colon) {
        *colon = '\0';
        cursor_line = (uint16_t)atoi(cursor_arg);
        cursor_col = (uint16_t)atoi(colon + 1);
        printf("Cursor query: line=%d, col=%d\n", cursor_line, cursor_col);
        file_start_idx = i + 1;
      } else {
        fprintf(stderr, "Invalid cursor format. Expected: --cursor line:col\n");
        return 1;
      }
      continue;
    }
  }

  for (int i = file_start_idx; i < (int)argc; i++) {
    printf("parsing file (%d)->(%s)\n", i, argv[i]);
    struct ModuleStatementNode *source_root =
        parse_source_file(&allocator, argv[i], cursor_line, cursor_col);

    if (current != NULL) {
      current->next =
          wrap_module(&allocator, source_root,
                      get_module_name_from_file(&allocator, argv[i]));
      current = current->next;
    } else {
      new_root = wrap_module(&allocator, source_root,
                             get_module_name_from_file(&allocator, argv[i]));
      current = new_root;
    }
  }
  new_root = wrap_module(&allocator, new_root, string_make("src"));

  puts("Debug mode - skipping annotate/codegen");
  return 0;

  struct Annotator annotator = annotator_make(&allocator);
  annotator.current_symbol_table = &annotator.root_symbol_table;

  puts("\n\n\n\n\n\n");
  puts("----------------------------------------------------------");
  puts("Starting mstb visitation.");
  puts("\n\n");

  mstb_visit_module(&annotator, new_root);

  insert_symbol_entry(&annotator, (struct SymbolTableEntry){
                                      .symbol = string_make("src"),
                                      .type = new_root->node.decl->data_type,
                                      .llvm_value = NULL,
                                      // .llvm_structure_type = NULL,
                                      .next = NULL,
                                      .symbol_location = SL_MODULE,
                                      .index = 0,
                                  });

  puts("Finished mstb.");

  mstb_infer_types(
      &annotator,
      &new_root->node.decl->expression->node.module_definition->symbol_table);

  puts("Finished infer types");

  print_symbol_table(string_make("Root1"), &annotator.root_symbol_table);
  annotator.current_symbol_table = &annotator.root_symbol_table;

  // annotator.root_symbol_table =
  //     new_root->node.decl->expression->node.module_definition->symbol_table;

  puts("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
  puts("\n\n\n\n\n\n");

  // assert(0);

  annotator_initialize_primitives(&annotator);
  puts("About to visit module statements.");
  annotator_visit_module_statements(&annotator, new_root);
  puts("Done visit module statements.");

  print_symbol_table(string_make("Root2"), &annotator.root_symbol_table);

  // assert(0);

  struct CodeGenerator code_generator = cg_make(&allocator, &annotator);

  puts("Start code gen");
  cg_prepare_module(&code_generator, new_root);
  puts("Done prep");
  LLVMValueRef global_module_initializer = cg_visit_module_statements(
      &code_generator, new_root, true, string_make("core"));
  cg_make_entrypoint(&code_generator, global_module_initializer);
  puts("Done code gen");

  // LLVMDumpModule(code_generator.module);

  char *error = NULL;
  if (LLVMPrintModuleToFile(code_generator.module, "out/compiled.ll", &error) !=
      0) {
    fprintf(stderr, "Failed to write module: %s", error);
    LLVMDisposeMessage(error);
  }

  printf("Arena Allocator used %ld/%ld (%0.2f%%) memory\n", allocator.length,
         allocator.capacity,
         (double)allocator.length / (double)allocator.capacity * 100.0);
  return 0;
}
