#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

#include "annotator.h"
#include "arena_allocator.h"
#include "ast.h"
#include "code_gen.h"
#include "instruction_builder.h"
#include "lexer.h"
#include "luna_string.h"
#include "parser.h"
#include "token.h"

int main(void) {
  puts("Luna Compiler");

  uint8_t arena[10024];

  struct ArenaAllocator allocator = arena_make(&arena, 10024);

  struct Lexer lexer =
      lexer_make(&allocator, string_make(
                                        "let a = 5 - (2 + 1);"
                                         "const c = true;"
                                         "a = 10;"
                                         "let x = 10;"
                                         "let y = 5 + x;"
                                         "a = 10;"
                                         "let g: int = 5;"
                                         // "const main = fn(): int {"
                                         // "  const abc = 123;"
                                         // "};"
                                         ));

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

  struct StatementNode *stmt = parse_statements(&parser);

  struct Annotator annotator = annotator_make(&allocator);

  annotator_initialize_primitives(&annotator);
  annotator_visit_statements(&annotator, stmt);

  struct InstructionBuilder ib = instruction_builder_make(&allocator);

  struct CodeGenerator code_generator =
      cg_make(&allocator, &ib, &annotator);

  puts("Start code gen");
  cg_visit_statements(&code_generator, stmt);
  puts("Done code gen");

  // evaluate_statements(environment_make(&allocator), stmt);

  printf("Arena Allocator used %d/%d (%0.2f%%) memory\n", allocator.length,
         allocator.capacity,
         (double)allocator.length / (double)allocator.capacity * 100.0);
  return 0;
}
