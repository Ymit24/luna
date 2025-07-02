#include "annotator.h"
#include "arena_allocator.h"
#include "ast.h"
#include "luna_string.h"

struct Annotator annotator_make(struct ArenaAllocator *allocator) {
  return (struct Annotator){
      .allocator = allocator,
      .data_type_table = (struct DataTypeTable){.head = NULL},
      .symbol_table = (struct SymbolTable){.head = NULL},
  };
}

void annotator_initialize_primitives(struct Annotator *annotator) {
  annotator->data_type_table.head = NULL;
  struct DataType *primitives[] = {ast_promote(
      annotator->allocator, &(struct DataType){.symbol = string_make("int")},
      sizeof(struct DataType))

  };

  for (uint16_t i = 0; i < 1; i++) {
    primitives[i]->next = annotator->data_type_table.head;
    annotator->data_type_table.head = primitives[i];
  }
}
