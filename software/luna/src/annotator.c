#include "annotator.h"
#include "arena_allocator.h"
#include "ast.h"
#include "luna_string.h"
#include <assert.h>
#include <string.h>

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

struct SymbolTableEntry *lookup_symbol(struct Annotator *annotator,
                                       struct LunaString symbol) {
  struct SymbolTableEntry *entry = annotator->symbol_table.head;

  while (entry != NULL &&
         strncmp(entry->symbol.data, symbol.data, entry->symbol.length) != 0) {
    entry = entry->next;
  }

  return entry;
}

void insert_symbol_entry(struct Annotator *annotator,
                         struct SymbolTableEntry entry) {
  struct SymbolTableEntry *entry_ptr = ast_promote(
      annotator->allocator, &entry, sizeof(struct SymbolTableEntry));
  entry_ptr->next = annotator->symbol_table.head;
  annotator->symbol_table.head = entry_ptr;
}

void annotator_visit_statement(struct Annotator *annotator,
                               struct StatementNode *statement) {
  switch (statement->type) {
  case STMT_LET:
    assert(lookup_symbol(annotator, statement->node.decl->symbol) == NULL);
    insert_symbol_entry(annotator, (struct SymbolTableEntry){
                                       .symbol = statement->node.decl->symbol,
                                       .next = NULL,
                                   });
    break;
  case STMT_CONST:
    assert(lookup_symbol(annotator, statement->node.decl->symbol) == NULL);
    insert_symbol_entry(annotator, (struct SymbolTableEntry){
                                       .symbol = statement->node.decl->symbol,
                                       .next = NULL,
                                   });
    break;
  case STMT_ASSIGN:
    assert(lookup_symbol(annotator, statement->node.decl->symbol) != NULL);
    break;
  default:
    assert(0);
    break;
  }
}

void annotator_visit_statements(struct Annotator *annotator,
                                struct StatementNode *statement) {
  struct StatementNode *curr = statement;

  while (curr != NULL) {
    annotator_visit_statement(annotator, curr);
    curr = curr->next;
  }
}
