#include "code_gen.h"
#include "annotator.h"
#include "arena_allocator.h"
#include "ast.h"
#include "instruction_builder.h"
#include "pretty_print.h"
#include <assert.h>
#include <stdio.h>

void cg_visit_function_statement(struct CodeGenerator *code_generator,
                                 struct FunctionStatementNode *stmt);
void cg_visit_function_statements(struct CodeGenerator *code_generator,
                                  struct FunctionStatementNode *stmt);

struct CodeGenerator cg_make(struct ArenaAllocator *allocator,
                             struct InstructionBuilder *instruction_builder,
                             struct Annotator *annotator) {
  return (struct CodeGenerator){
      .allocator = allocator,
      .instruction_builder = instruction_builder,
      .annotator = annotator,
      .current_symbol_table = &annotator->root_symbol_table,
  };
}

void cg_visit_expr(struct CodeGenerator *code_generator,
                   struct ExpressionNode *expr) {
  switch (expr->type) {
  case EXPR_BINARY:
    pp_debug("Generating code for binary expression: %s", 
             expr->node.binary->type == BIN_EXPR_ADD ? "ADD" :
             expr->node.binary->type == BIN_EXPR_SUB ? "SUB" :
             expr->node.binary->type == BIN_EXPR_MUL ? "MUL" : "DIV");
    cg_visit_expr(code_generator, expr->node.binary->left);
    cg_visit_expr(code_generator, expr->node.binary->right);
    switch (expr->node.binary->type) {
    case BIN_EXPR_ADD:
      pp_debug("Emitting ADD instruction");
      ib_push_argless(code_generator->instruction_builder, IT_ADD);
      break;
    case BIN_EXPR_SUB:
      pp_debug("Emitting SUB instruction");
      ib_push_argless(code_generator->instruction_builder, IT_SUB);
      break;
    case BIN_EXPR_MUL:
      pp_error("Multiplication not yet supported");
      assert(0);
      break;
    case BIN_EXPR_DIV:
      pp_error("Division not yet supported");
      assert(0);
      break;
    }
    break;
  case EXPR_INTEGER_LITERAL:
    pp_debug("Generating PUSH CONST %d", expr->node.integer->value);
    ib_push_push_index(code_generator->instruction_builder, MS_CONST,
                       expr->node.integer->value);
    break;
  case EXPR_SYMBOL_LITERAL:
    pp_error("Symbol literals not yet supported in code generation");
    assert(0);
    break;
  case EXPR_FN_DEF:
    pp_debug("Starting function code generation");
    struct LunaString label = ib_push_fn(code_generator->instruction_builder);
    struct SymbolTable *old_current = code_generator->current_symbol_table;
    code_generator->current_symbol_table = &expr->node.fn_def->symbol_table;
    cg_visit_function_statements(code_generator, expr->node.fn_def->body);
    code_generator->current_symbol_table = old_current;
    pp_debug("Completed function code generation");
    ib_pop_fn(code_generator->instruction_builder);

    ib_push_push_label(code_generator->instruction_builder, label);
    break;
  }
}

void cg_visit_decl(struct CodeGenerator *code_generator,
                   struct DeclarationStatementNode *decl) {

  struct SymbolTableEntry *symbol =
      lookup_symbol_in(decl->symbol, code_generator->current_symbol_table);
  assert(symbol != NULL);
  pp_debug("Generating code for declaration: %.*s", decl->symbol.length, decl->symbol.data);
  cg_visit_expr(code_generator, decl->expression);
  
  const char *segment_name = (symbol->memory_segment == MS_LOCAL) ? "LOCAL" :
                            (symbol->memory_segment == MS_STATIC) ? "STATIC" : "CONST";
  pp_debug("Emitting POP %s %d", segment_name, symbol->index);
  ib_push_pop(code_generator->instruction_builder, symbol->memory_segment,
              symbol->index);
}

void cg_visit_function_statement(struct CodeGenerator *code_generator,
                                 struct FunctionStatementNode *stmt) {
  switch (stmt->type) {
  case FN_STMT_LET:
    cg_visit_decl(code_generator, stmt->node.decl);
    break;
  case FN_STMT_CONST:
    cg_visit_decl(code_generator, stmt->node.decl);
    break;
  case FN_STMT_ASSIGN:
    assert(lookup_symbol_in(stmt->node.assign->symbol,
                            code_generator->current_symbol_table) != NULL);
    pp_debug("Generating assignment for: %.*s", 
             stmt->node.assign->symbol.length, stmt->node.assign->symbol.data);
    cg_visit_expr(code_generator, stmt->node.assign->expression);
    break;
  }
}

void cg_visit_module_statement(struct CodeGenerator *code_generator,
                               struct ModuleStatementNode *stmt) {
  switch (stmt->type) {
  case MOD_STMT_LET:
    cg_visit_decl(code_generator, stmt->node.decl);
    break;
  case MOD_STMT_CONST:
    cg_visit_decl(code_generator, stmt->node.decl);
    break;
  default:
    pp_error("Unknown module statement type");
    assert(0);
    break;
  }
}

void cg_visit_function_statements(struct CodeGenerator *code_generator,
                                  struct FunctionStatementNode *stmt) {
  struct FunctionStatementNode *curr = stmt;
  while (curr != NULL) {
    cg_visit_function_statement(code_generator, curr);
    curr = curr->next;
  }
}

void cg_visit_module_statements(struct CodeGenerator *code_generator,
                                struct ModuleStatementNode *stmt) {
  struct ModuleStatementNode *curr = stmt;
  while (curr != NULL) {
    cg_visit_module_statement(code_generator, curr);
    curr = curr->next;
  }
}

