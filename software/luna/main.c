#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

#include "annotator.h"
#include "arena_allocator.h"
#include "ast.h"
#include "code_gen.h"
#include "instruction_builder.h"
#include "instructions.h"
#include "lexer.h"
#include "luna_string.h"
#include "parser.h"
#include "token.h"

int main(void) {
  puts("Luna Compiler");

  uint8_t arena[UINT16_MAX];

  struct ArenaAllocator allocator = arena_make(&arena, UINT16_MAX);

  struct Lexer lexer =
      lexer_make(&allocator, string_make("let a = 5 - (2 + 1);"
                                         // "const c = true;"
                                         // "a = 7;"
                                         "let x = 3;"
                                         // "let y = 5 + x;"
                                         // "a = 4;"
                                         "let g: int = 5;"
                                         "const main = fn(): int {"
                                         "  const abc = 456;"
                                         "  const inner = fn(): int {"
                                         "    const innervar = 92;"
                                         "  };"
                                         "  abc = 5;"
                                         "};"));

  struct Token toks[1024];
  uint16_t tok_index = 0;

  puts("tokens:");
  while (lexer_next(&lexer, &toks[tok_index++])) {
    if (false) {
      printf("\t%d\n", toks[tok_index - 1].type);
    }
  }
  puts("");

  printf("Found a total of %d tokens.\n", tok_index - 1);

  struct Parser parser = parser_make(&allocator, toks, tok_index);

  struct ModuleStatementNode *stmt = parse_module_statements(&parser);

  struct Annotator annotator = annotator_make(&allocator);

  annotator_initialize_primitives(&annotator);
  annotator_visit_module_statements(&annotator, stmt);

  struct InstructionBuilder ib = instruction_builder_make(&allocator);

  struct CodeGenerator code_generator = cg_make(&allocator, &ib, &annotator);

  puts("Start code gen");
  cg_visit_module_statements(&code_generator, stmt);
  puts("Done code gen");

  // evaluate_statements(environment_make(&allocator), stmt);

  puts("Code Generated:");
  struct InstructionGroup *curr = ib.head;

  while (curr != NULL) {
    struct Instruction *instr = curr->head;
    while (instr != NULL) {
      switch (instr->type) {
      case IT_PUSH: {
        switch (instr->value.pushpoplea.memory_segment) {
        case MS_LOCAL:
          printf("\tpush local %d\n", instr->value.pushpoplea.value.index);
          break;
        case MS_CONST:
          if (instr->value.pushpoplea.is_index) {
            printf("\tpush const %d\n", instr->value.pushpoplea.value.index);
          } else {
            printf("\tpush const %s\n",
                   instr->value.pushpoplea.value.label.data);
          }
          break;
        }
        break;
      }
      case IT_POP:
        switch (instr->value.pushpoplea.memory_segment) {
        case MS_LOCAL:
          printf("\tpop local %d\n", instr->value.pushpoplea.value.index);
          break;
        case MS_CONST:
          printf("\tpop const %d\n", instr->value.pushpoplea.value.index);
          break;
        }
        break;
      case IT_LEA:
        switch (instr->value.pushpoplea.memory_segment) {
        case MS_LOCAL:
          printf("\tlea local %d\n", instr->value.pushpoplea.value.index);
          break;
        case MS_CONST:
          puts("Illegal lea of const.");
          break;
        }
        break;
      case IT_LOAD:
        puts("\tload");
        break;
      case IT_STORE:
        puts("\tstore");
        break;
      case IT_LABEL:
        printf("label %s\n", instr->value.label.data);
        break;
      case IT_ADD:
        puts("\tadd");
        break;
      case IT_SUB:
        puts("\tsub");
        break;
      }
      instr = instr->next;
    }
    curr = curr->next;
    puts("");
  }

  printf("Arena Allocator used %d/%d (%0.2f%%) memory\n", allocator.length,
         allocator.capacity,
         (double)allocator.length / (double)allocator.capacity * 100.0);
  return 0;
}
