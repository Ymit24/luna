#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

#include "annotator.h"
#include "arena_allocator.h"
#include "ast.h"
#include "ast_printer.h"
#include "code_gen.h"
#include "instruction_builder.h"
#include "instruction_executor.h"
#include "instructions.h"
#include "lexer.h"
#include "luna_string.h"
#include "parser.h"
#include "pretty_print.h"
#include "token.h"

int main(void) {
  pp_header("Luna Compiler");

  uint8_t arena[UINT16_MAX];
  struct ArenaAllocator allocator = arena_make(&arena, UINT16_MAX);

  pp_section_start("Lexical Analysis");
  struct Lexer lexer =
      lexer_make(&allocator, string_make("let a = 5 - (2 + 1);"
                                         "let x = 3;"
                                         "let g: int = 5;"
                                         "const main = fn(): int {"
                                         "  const abc = 456;"
                                         "  const inner = fn(): int {"
                                         "    const doubleinner = 9;"
                                         "  };"
                                         "  abc = 5;"
                                         "};"));

  struct Token toks[1024];
  uint16_t tok_index = 0;

  pp_step("Tokenizing source code...");
  while (lexer_next(&lexer, &toks[tok_index++])) {
    // Tokens processed
  }

  pp_success("Found %d tokens", tok_index - 1);
  pp_section_end();

  pp_section_start("Syntax Analysis");
  pp_step("Parsing module statements...");
  struct Parser parser = parser_make(&allocator, toks, tok_index);
  struct ModuleStatementNode *stmt = parse_module_statements(&parser);
  pp_success("Parsing completed successfully");

  pp_subheader("Abstract Syntax Tree");
  printf(BOLD CYAN "🌳 Parsed AST Structure:\n" RESET);
  ast_print_module_statements(stmt, 0);
  pp_section_end();

  pp_section_start("Semantic Analysis");
  pp_step("Initializing annotator...");
  struct Annotator annotator = annotator_make(&allocator);
  annotator.current_symbol_table = &annotator.root_symbol_table;

  annotator_initialize_primitives(&annotator);
  pp_step("Analyzing symbols and types...");
  annotator_visit_module_statements(&annotator, stmt);
  pp_success("Semantic analysis completed");
  pp_section_end();

  pp_section_start("Code Generation");
  pp_step("Initializing instruction builder...");
  struct InstructionBuilder ib = instruction_builder_make(&allocator);
  struct CodeGenerator code_generator = cg_make(&allocator, &ib, &annotator);

  pp_step("Generating VM instructions...");
  cg_visit_module_statements(&code_generator, stmt);
  pp_success("Code generation completed");

  pp_subheader("Generated Instructions");
  struct InstructionGroup *curr = ib.head;

  while (curr != NULL) {
    struct Instruction *instr = curr->head;
    while (instr != NULL) {
      printf(DIM "  ");
      switch (instr->type) {
      case IT_PUSH: {
        switch (instr->value.pushpoplea.memory_segment) {
        case MS_LOCAL:
          printf("push local %d", instr->value.pushpoplea.value.index);
          break;
        case MS_STATIC:
          printf("push static %d", instr->value.pushpoplea.value.index);
          break;
        case MS_CONST:
          if (instr->value.pushpoplea.is_index) {
            printf("push const %d", instr->value.pushpoplea.value.index);
          } else {
            printf("push const %s", instr->value.pushpoplea.value.label.data);
          }
          break;
        }
        break;
      }
      case IT_POP:
        switch (instr->value.pushpoplea.memory_segment) {
        case MS_LOCAL:
          printf("pop local %d", instr->value.pushpoplea.value.index);
          break;
        case MS_STATIC:
          printf("pop static %d", instr->value.pushpoplea.value.index);
          break;
        case MS_CONST:
          printf("pop const %d", instr->value.pushpoplea.value.index);
          break;
        }
        break;
      case IT_LEA:
        switch (instr->value.pushpoplea.memory_segment) {
        case MS_LOCAL:
          printf("lea local %d", instr->value.pushpoplea.value.index);
          break;
        case MS_STATIC:
          printf("lea static %d", instr->value.pushpoplea.value.index);
          break;
        case MS_CONST:
          printf("lea const (illegal)");
          break;
        }
        break;
      case IT_LOAD:
        printf("load");
        break;
      case IT_STORE:
        printf("store");
        break;
      case IT_LABEL:
        printf(CYAN "%s:" RESET, instr->value.label.data);
        printf("\n");
        goto next_instruction;
      case IT_ADD:
        printf("add");
        break;
      case IT_SUB:
        printf("sub");
        break;
      }
      printf(RESET "\n");
      next_instruction:
      instr = instr->next;
    }
    curr = curr->next;
    if (curr != NULL) printf("\n");
  }
  pp_section_end();

  // Execute the generated code using the VM
  pp_header("VM Execution");
  struct VMState vm = vm_init(&allocator);
  vm_execute_program(&vm, &ib);

  pp_memory_usage(allocator.length, allocator.capacity);
  return 0;
}
