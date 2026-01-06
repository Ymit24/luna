#include <stdio.h>
#include <string.h>
#include "lsp_query.h"
#include "parser.h"
#include "lexer.h"

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

struct LineMap build_line_map(struct LunaString source,
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

uint16_t line_col_to_byte_offset(struct LineMap map, uint16_t line,
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

struct ContainingNode
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

const char *node_type_to_string(enum ContainingNodeType type) {
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

void query_hover_info(struct ContainingNode node, char *out_buf, size_t max_len) {
  if (node.type == NODE_NONE) {
    snprintf(out_buf, max_len, "No information available at this location");
    return;
  }

  switch (node.type) {
  case NODE_VARIABLE: {
    if (node.name.length > 0) {
      const char *const_str = node.node.variable->is_const ? "true" : "false";
      const char *type_str = node.node.variable->has_type ? "typed" : "inferred";
      snprintf(out_buf, max_len,
               "# Variable: `%.*s`\n"
               "**Const:** %s\n"
               "**Type annotation:** %s",
               (int)node.name.length, node.name.data,
               const_str, type_str);
    } else {
      snprintf(out_buf, max_len, "Variable declaration");
    }
    break;
  }
  case NODE_FUNCTION: {
    snprintf(out_buf, max_len, "# Function");
    break;
  }
  case NODE_MODULE: {
    snprintf(out_buf, max_len, "# Module");
    break;
  }
  case NODE_EXPRESSION: {
    const char *expr_type = "unknown";
    switch (node.node.expression->type) {
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
    snprintf(out_buf, max_len, "# Expression\n**Type:** %s", expr_type);
    break;
  }
  default:
    snprintf(out_buf, max_len, "Unknown node type");
    break;
  }
}
