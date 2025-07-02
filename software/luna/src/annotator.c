#include "annotator.h"
#include "arena_allocator.h"
#include "ast.h"
#include "luna_string.h"
#include <assert.h>
#include <stdio.h>
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

void print_symbols(struct Annotator *annotator) {
  struct SymbolTableEntry *symbol_table_entry = annotator->symbol_table.head;
  puts("Symbol Table:");
  while (symbol_table_entry != NULL) {
    if (symbol_table_entry->type != NULL) {
      printf("\t%s: %s\n", symbol_table_entry->symbol.data,
             symbol_table_entry->type->symbol.data);
    } else {

      printf("\t%s: unknown\n", symbol_table_entry->symbol.data);
    }
    symbol_table_entry = symbol_table_entry->next;
  }
}

void print_data_types(struct Annotator *annotator) {
  struct DataType *data_type = annotator->data_type_table.head;
  puts("Type Table:");
  while (data_type != NULL) {
    printf("\t%s\n", data_type->symbol.data);
    data_type = data_type->next;
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

struct DataType *lookup_data_type(struct Annotator *annotator,
                                  struct LunaString symbol) {
  struct DataType *entry = annotator->data_type_table.head;

  while (entry != NULL &&
         strncmp(entry->symbol.data, symbol.data, entry->symbol.length) != 0) {
    entry = entry->next;
  }

  return entry;
}

struct DataType *infer_type(struct Annotator *annotator,
                            struct ExpressionNode *expr) {
  switch (expr->type) {
  case EXPR_INTEGER_LITERAL:
    puts("infered int");
    return lookup_data_type(annotator, string_make("int"));
  case EXPR_SYMBOL_LITERAL: {
    struct SymbolTableEntry *entry =
        lookup_symbol(annotator, expr->node.symbol->value);
    assert(entry != NULL);
    printf("infered %s\n", entry->type->symbol.data);
    return entry->type;
  }
  case EXPR_BINARY: {
    puts("Infering on binary..");
    struct DataType *left = infer_type(annotator, expr->node.binary->left);
    struct DataType *right = infer_type(annotator, expr->node.binary->right);
    assert(left == right);
    return left;
  }
  }

  puts("Failed to infer type.");
  assert(0);
  return NULL;
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
  case STMT_LET: {
    assert(lookup_symbol(annotator, statement->node.decl->symbol) == NULL);
    struct DataType *type =
        infer_type(annotator, statement->node.decl->expression);
    insert_symbol_entry(annotator, (struct SymbolTableEntry){
                                       .symbol = statement->node.decl->symbol,
                                       .type = type,
                                       .next = NULL,
                                   });
    break;
  }
  case STMT_CONST: {
    assert(lookup_symbol(annotator, statement->node.decl->symbol) == NULL);
    struct DataType *type =
        infer_type(annotator, statement->node.decl->expression);
    insert_symbol_entry(annotator, (struct SymbolTableEntry){
                                       .symbol = statement->node.decl->symbol,
                                       .type = type,
                                       .next = NULL,
                                   });
    break;
  }
  case STMT_ASSIGN: {
    struct SymbolTableEntry *entry =
        lookup_symbol(annotator, statement->node.decl->symbol);
    assert(entry != NULL);

    assert(entry->type ==
           infer_type(annotator, statement->node.assign->expression));

    break;
  }
  default:
    assert(0);
    break;
  }
}

void annotator_visit_statements(struct Annotator *annotator,
                                struct StatementNode *statement) {
  struct StatementNode *curr = statement;

  puts("Starting annotation..");
  while (curr != NULL) {
    puts("Annotating new line..");
    annotator_visit_statement(annotator, curr);
    print_symbols(annotator);
    print_data_types(annotator);
    curr = curr->next;
  }
  puts("Finished annotation.");
}
