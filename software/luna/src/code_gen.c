#include "code_gen.h"
#include "annotator.h"
#include "arena_allocator.h"
#include "ast.h"
#include "instruction_builder.h"
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
  };
}

void cg_visit_expr(struct CodeGenerator *code_generator,
                   struct ExpressionNode *expr) {
  printf("[cg_visit_expr] %d\n", expr->type);
  switch (expr->type) {
  case EXPR_BINARY:
    printf("[cg_visit_expr] in binary of type %d\n", expr->node.binary->type);
    cg_visit_expr(code_generator, expr->node.binary->left);
    cg_visit_expr(code_generator, expr->node.binary->right);
    switch (expr->node.binary->type) {
    case BIN_EXPR_ADD:
      puts("got add");
      ib_push_argless(code_generator->instruction_builder, IT_ADD);
      break;
    case BIN_EXPR_SUB:
      puts("got sub");
      ib_push_argless(code_generator->instruction_builder, IT_SUB);
      break;
    case BIN_EXPR_MUL:
      puts("unsupported bin expr type");
      assert(0);
      break;
    case BIN_EXPR_DIV:
      puts("unsupported vin expr type");
      assert(0);
      break;
    }
  case EXPR_INTEGER_LITERAL:
    ib_push_push_index(code_generator->instruction_builder, MS_CONST,
                       expr->node.integer->value);
    break;
  case EXPR_SYMBOL_LITERAL:
    puts("unsupported expr type");
    assert(0);
    break;
  case EXPR_FN_DEF:
    puts("pushing function");
    struct LunaString label = ib_push_fn(code_generator->instruction_builder);
    cg_visit_function_statements(code_generator, expr->node.fn_def->body);
    puts("popping function");
    ib_pop_fn(code_generator->instruction_builder);

    ib_push_push_label(code_generator->instruction_builder, label);

    break;
  }
}

void cg_visit_function_statement(struct CodeGenerator *code_generator,
                                 struct FunctionStatementNode *stmt) {
  switch (stmt->type) {
  case FN_STMT_LET:
    assert(lookup_symbol(code_generator->annotator, stmt->node.decl->symbol) !=
           NULL);
    puts("let safe.");
    cg_visit_expr(code_generator, stmt->node.decl->expression);
    break;
  case FN_STMT_CONST:
    assert(lookup_symbol(code_generator->annotator, stmt->node.decl->symbol) !=
           NULL);
    puts("const safe.");
    cg_visit_expr(code_generator, stmt->node.decl->expression);
    break;
  case FN_STMT_ASSIGN:
    assert(lookup_symbol(code_generator->annotator,
                         stmt->node.assign->symbol) != NULL);
    puts("assign safe.");
    printf("assign type: %s -> %d (bin is %d)\n",
           stmt->node.assign->symbol.data, stmt->node.assign->expression->type,
           EXPR_BINARY);
    cg_visit_expr(code_generator, stmt->node.assign->expression);
    break;
  }
}

void cg_visit_module_statement(struct CodeGenerator *code_generator,
                               struct ModuleStatementNode *stmt) {
  switch (stmt->type) {
  case MOD_STMT_LET:
    assert(lookup_symbol(code_generator->annotator, stmt->node.decl->symbol) !=
           NULL);
    puts("let safe.");
    cg_visit_expr(code_generator, stmt->node.decl->expression);
    break;
  case MOD_STMT_CONST:
    assert(lookup_symbol(code_generator->annotator, stmt->node.decl->symbol) !=
           NULL);
    puts("const safe.");
    cg_visit_expr(code_generator, stmt->node.decl->expression);
    break;
  default:
    puts("Unknown module statement type.");
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
