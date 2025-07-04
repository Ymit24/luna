#include "code_gen.h"
#include "annotator.h"
#include "arena_allocator.h"
#include "ast.h"
#include <assert.h>
#include <stdio.h>

struct CodeGenerator cg_make(struct ArenaAllocator *allocator,
                             struct InstructionBuilder *instruction_builder,
                             struct Annotator *annotator) {
  return (struct CodeGenerator){
      .allocator = allocator,
      .instruction_builder = instruction_builder,
      .annotator = annotator,
  };
}

void cg_visit_statement(struct CodeGenerator *code_generator,
                        struct StatementNode *stmt) {
  switch (stmt->type) {
  case STMT_LET:
    assert(lookup_symbol(code_generator->annotator, stmt->node.decl->symbol) !=
           NULL);
    puts("let safe.");
  case STMT_CONST:
    assert(lookup_symbol(code_generator->annotator, stmt->node.decl->symbol) !=
           NULL);
    puts("const safe.");
  case STMT_ASSIGN:
    assert(lookup_symbol(code_generator->annotator,
                         stmt->node.assign->symbol) != NULL);
    puts("assign safe.");
    break;
  case STMT_EXPR:
    puts("Unsupported statement type");
    assert(0);
    break;
  }
}

void cg_visit_statements(struct CodeGenerator *code_generator,
                         struct StatementNode *stmt) {
  struct StatementNode *curr = stmt;
  while (curr != NULL) {
    cg_visit_statement(code_generator, curr);
    curr = curr->next;
  }
}
