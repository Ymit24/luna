#include "symbol_table_builder.h"
#include "annotator.h"
#include "ast.h"
#include <assert.h>
#include <stdio.h>

void sym_tbl_visit_module_statements(struct Annotator *annotator,
                                     struct ModuleStatementNode *root_module,
                                     struct SymbolTable *module_symbol_table);

void sym_tbl_visit_function_definition(
    struct Annotator *annotator,
    struct FunctionDefinitionExpressionNode *fn_def);

void sym_tbl_visit_function_statement(struct Annotator *annotator,
                                      struct FunctionStatementNode *stmt);

void sym_tbl_visit_function_call(struct Annotator *annotator,
                                 struct FunctionCallExpressionNode *fn_call);

void sym_tbl_visit_expr(struct Annotator *annotator,
                        struct ExpressionNode *expr);

void sym_tbl_visit_array_initializer(
    struct Annotator *annotator,
    struct ArrayInitializerExpressionNode *initializer);

void sym_tbl_visit_structure_initializer(
    struct Annotator *annotator,
    struct StructInitializationExpressionNode *initializer);

void sym_tbl_visit_function_decl(struct Annotator *annotator,
                                 struct DeclarationStatementNode *decl) {
  assert(lookup_symbol_in_local(decl->symbol,
                                annotator->current_symbol_table) == NULL);

  insert_symbol_entry(annotator, (struct SymbolTableEntry){
                                     .symbol = decl->symbol,
                                     .symbol_location = SL_LOCAL,
                                     .type = decl->data_type,
                                     .next = NULL,
                                     .llvm_value = NULL,
                                     .llvm_structure_type = NULL,
                                 });

  if (decl->expression->type == EXPR_FN_DEF) {
    struct FunctionArgumentNode *args =
        decl->expression->node.fn_def->function_type->value.function.arguments;

    while (args != NULL) {
      annotator->current_symbol_table =
          &decl->expression->node.fn_def->symbol_table;
      struct SymbolTable *old_symbol_table = annotator->current_symbol_table;
      insert_symbol_entry(annotator, (struct SymbolTableEntry){
                                         .symbol = args->symbol,
                                         .type = args->data_type,
                                         .symbol_location = SL_ARGUMENT,
                                         .llvm_structure_type = NULL,
                                         .llvm_value = NULL,
                                         .next = NULL,
                                     });
      annotator->current_symbol_table = old_symbol_table;

      args = args->next;
    }

    sym_tbl_visit_function_definition(annotator, decl->expression->node.fn_def);
  }
}

void sym_tbl_visit_while(struct Annotator *annotator,
                         struct WhileStatementNode *stmt) {

  struct SymbolTable *old_symbol_table = annotator->current_symbol_table;
  annotator->current_symbol_table = &stmt->symbol_table;

  struct FunctionStatementNode *body_stmt = stmt->body;
  while (body_stmt != NULL) {
    sym_tbl_visit_function_statement(annotator, body_stmt);
    body_stmt = body_stmt->next;
  }

  annotator->current_symbol_table = old_symbol_table;
}

void sym_tbl_visit_if(struct Annotator *annotator,
                      struct IfStatementNode *stmt) {

  struct SymbolTable *old_symbol_table = annotator->current_symbol_table;
  annotator->current_symbol_table = &stmt->symbol_table;

  struct FunctionStatementNode *body_stmt = stmt->body;
  while (body_stmt != NULL) {
    sym_tbl_visit_function_statement(annotator, body_stmt);
    body_stmt = body_stmt->next;
  }

  annotator->current_symbol_table = old_symbol_table;
}

void sym_tbl_visit_structure_initializer(
    struct Annotator *annotator,
    struct StructInitializationExpressionNode *initializer) {
  struct StructFieldInitializerExpressionNode *field = initializer->fields;
  while (field != NULL) {
    sym_tbl_visit_expr(annotator, field->expression);
    field = field->next;
  }
}

void sym_tbl_visit_array_initializer(
    struct Annotator *annotator,
    struct ArrayInitializerExpressionNode *initializer) {
  while (initializer != NULL) {
    sym_tbl_visit_expr(annotator, initializer->initializer);
    initializer = initializer->next;
  }
}

void sym_tbl_visit_expr(struct Annotator *annotator,
                        struct ExpressionNode *expr) {
  printf("in expr %d\n", expr->type);
  switch (expr->type) {
  case EXPR_BINARY:
    sym_tbl_visit_expr(annotator, expr->node.binary->left);
    sym_tbl_visit_expr(annotator, expr->node.binary->right);
    break;
  case EXPR_FN_DEF:
    sym_tbl_visit_function_definition(annotator, expr->node.fn_def);
    break;
  case EXPR_FN_CALL:
    sym_tbl_visit_function_call(annotator, expr->node.fn_call);
    break;
  case EXPR_REF:
    // TODO: maybe something here in future
    break;
  case EXPR_DEREF:
    sym_tbl_visit_expr(annotator, expr->node.deref);
    break;
  case EXPR_STRUCT_DEF:
    break;
  case EXPR_STRUCT_INIT:
    sym_tbl_visit_structure_initializer(annotator, expr->node.struct_init);
    break;
  case EXPR_FIELD_ACCESS:
    break;
  case EXPR_CAST:
    sym_tbl_visit_expr(annotator, expr->node.cast->expr);
    break;
  case EXPR_ARRAY_INITIALIZER:
    sym_tbl_visit_array_initializer(annotator, expr->node.array_initializers);
    break;
  case EXPR_MOD_DEF:
    sym_tbl_visit_module_statements(
        annotator, expr->node.module_definition->statements,
        &expr->node.module_definition->symbol_table);
    break;
  case EXPR_VALUESIZE:
    sym_tbl_visit_expr(annotator, expr->node.valuesize);
    break;
  case EXPR_TYPESIZE:
    break;
  case EXPR_INTEGER_LITERAL:
  case EXPR_SYMBOL_LITERAL:
  case EXPR_STRING_LITERAL:
    break;
  }
  printf("out expr %d\n", expr->type);
}

void sym_tbl_visit_function_call(struct Annotator *annotator,
                                 struct FunctionCallExpressionNode *fn_call) {
  assert(fn_call != NULL);
  struct FunctionCallArgumentExpressionsNode *arguments = fn_call->arguments;
  while (arguments != NULL) {
    sym_tbl_visit_expr(annotator, arguments->argument);
    arguments = arguments->next;
  }
}

void sym_tbl_visit_function_statement(struct Annotator *annotator,
                                      struct FunctionStatementNode *stmt) {
  printf("open function stmt %d\n", stmt->type);
  switch (stmt->type) {
  case FN_STMT_LET:
  case FN_STMT_CONST:
    sym_tbl_visit_function_decl(annotator, stmt->node.decl);
    break;
  case FN_STMT_IF:
    sym_tbl_visit_if(annotator, stmt->node.if_stmt);
    break;
  case FN_STMT_WHILE:
    sym_tbl_visit_while(annotator, stmt->node.while_stmt);
    break;
  case FN_STMT_ASSIGN:
    if (stmt->node.assign->result_expression->type == EXPR_FN_DEF) {
      sym_tbl_visit_function_definition(
          annotator, stmt->node.assign->result_expression->node.fn_def);
    }
    break;
  case FN_STMT_RETURN:
    if (stmt->node.ret->expression != NULL) {
      sym_tbl_visit_expr(annotator,stmt->node.ret->expression);
    }
    break;
  case FN_STMT_FN_CALL:
    sym_tbl_visit_function_call(annotator, stmt->node.fn_call);
    break;
  }
  printf("close function stmt %d\n", stmt->type);
}

void sym_tbl_visit_function_definition(
    struct Annotator *annotator,
    struct FunctionDefinitionExpressionNode *fn_def) {
  puts("open fn def");
  assert(fn_def != NULL);
  struct SymbolTable *old_symbol_table = annotator->current_symbol_table;
  annotator->current_symbol_table = &fn_def->symbol_table;

  if (fn_def->body != NULL) {
    sym_tbl_visit_function_statement(annotator, fn_def->body);
  }

  annotator->current_symbol_table = old_symbol_table;
  puts("close fn def");
}

void sym_tbl_visit_module_decl(struct Annotator *annotator,
                               struct DeclarationStatementNode *decl) {
  assert(lookup_symbol_in_local(decl->symbol,
                                annotator->current_symbol_table) == NULL);

  insert_symbol_entry(annotator, (struct SymbolTableEntry){
                                     .symbol = decl->symbol,
                                     .symbol_location = SL_MODULE,
                                     .type = decl->data_type,
                                     .next = NULL,
                                     .llvm_value = NULL,
                                     .llvm_structure_type = NULL,
                                 });

  sym_tbl_visit_expr(annotator, decl->expression);
}

void sym_tbl_visit_module_statement(
    struct Annotator *annotator, struct ModuleStatementNode *module_statement) {
  puts("visit module statement.");
  switch (module_statement->type) {
  case MOD_STMT_LET:
  case MOD_STMT_CONST:
    sym_tbl_visit_module_decl(annotator, module_statement->node.decl);
    break;
  }
}

void sym_tbl_visit_module_statements(struct Annotator *annotator,
                                     struct ModuleStatementNode *root_module,
                                     struct SymbolTable *module_symbol_table) {
  struct ModuleStatementNode *stmt = root_module;
  struct SymbolTable *old_symbol_table = annotator->current_symbol_table;
  annotator->current_symbol_table = module_symbol_table;

  printf("opening module\n");

  while (stmt != NULL) {
    sym_tbl_visit_module_statement(annotator, stmt);

    stmt = stmt->next;
  }
  annotator->current_symbol_table = old_symbol_table;
  printf("closing module\n");
}

void sym_tbl_build(struct Annotator *annotator,
                   struct ModuleStatementNode *root_module) {
  sym_tbl_visit_module_statements(annotator, root_module,
                                  &annotator->root_symbol_table);
}
