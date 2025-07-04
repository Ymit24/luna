#include "annotator.h"
#include "arena_allocator.h"
#include "ast.h"
#include "luna_string.h"
#include <alloca.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

struct DataType DT_INT = (struct DataType){
    .kind = DTK_PRIMITIVE,
    .value.primitive = P_INT,
    .next = NULL,
};

struct DataType DT_BOOL = (struct DataType){
    .kind = DTK_PRIMITIVE,
    .value.primitive = P_BOOL,
    .next = NULL,
};

bool data_types_equal(struct DataType *left, struct DataType *right);

void insert_symbol_entry(struct Annotator *annotator,
                         struct SymbolTableEntry entry);

struct Annotator annotator_make(struct ArenaAllocator *allocator) {
  return (struct Annotator){
      .allocator = allocator,
      .data_type_table = (struct DataTypeTable){.head = NULL},
      .symbol_table = (struct SymbolTable){.head = NULL},
  };
}

struct DataType *make_primitive_data_type(struct Annotator *annotator,
                                          enum PrimitiveType type) {
  return ast_promote(
      annotator->allocator,
      &(struct DataType){.kind = DTK_PRIMITIVE, .value.primitive = type},
      sizeof(struct DataType));
}

struct DataType *make_function_data_type(struct Annotator *annotator,
                                         struct DataType *return_type) {
  return ast_promote(annotator->allocator,
                     &(struct DataType){
                         .kind = DTK_FUNCTION,
                         .value.function.return_type = return_type,
                         .next = NULL,
                     },
                     sizeof(struct DataType));
}

void annotator_initialize_primitives(struct Annotator *annotator) {
  annotator->data_type_table.head = NULL;
  struct DataType *primitives[] = {
      ast_promote(
          annotator->allocator,
          &(struct DataType){.kind = DTK_PRIMITIVE, .value.primitive = P_INT},
          sizeof(struct DataType)),
      ast_promote(
          annotator->allocator,
          &(struct DataType){.kind = DTK_PRIMITIVE, .value.primitive = P_BOOL},
          sizeof(struct DataType))

  };

  for (uint16_t i = 0; i < 1; i++) {
    primitives[i]->next = annotator->data_type_table.head;
    annotator->data_type_table.head = primitives[i];
  }

  insert_symbol_entry(annotator, (struct SymbolTableEntry){
                                     .symbol = string_make("true"),
                                     .type = primitives[1],
                                     .next = NULL,
                                 });
  insert_symbol_entry(annotator, (struct SymbolTableEntry){
                                     .symbol = string_make("false"),
                                     .type = primitives[1],
                                     .next = NULL,
                                 });
}

void print_symbols(struct Annotator *annotator) {
  struct SymbolTableEntry *symbol_table_entry = annotator->symbol_table.head;
  puts("Symbol Table:");
  while (symbol_table_entry != NULL) {
    if (symbol_table_entry->type != NULL) {
      switch (symbol_table_entry->type->kind) {
      case DTK_PRIMITIVE:
        printf("\t%s: %d\n", symbol_table_entry->symbol.data,
               symbol_table_entry->type->value.primitive);
        break;
      case DTK_FUNCTION:
        printf("\t%s: fn\n", symbol_table_entry->symbol.data);
        break;
      }
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
    switch (data_type->kind) {
    case DTK_PRIMITIVE:
      printf("\t%d\n", data_type->value.primitive);
      break;
    case DTK_FUNCTION:
      printf("\tfn\n");
      break;
    }
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

struct DataType *infer_type(struct Annotator *annotator,
                            struct ExpressionNode *expr) {
  switch (expr->type) {
  case EXPR_INTEGER_LITERAL:
    puts("infered int");
    return make_primitive_data_type(annotator, P_INT);
  case EXPR_SYMBOL_LITERAL: {
    struct SymbolTableEntry *entry =
        lookup_symbol(annotator, expr->node.symbol->value);
    assert(entry != NULL);
    return entry->type;
  }
  case EXPR_BINARY: {
    puts("Infering on binary..");
    struct DataType *left = infer_type(annotator, expr->node.binary->left);
    struct DataType *right = infer_type(annotator, expr->node.binary->right);
    assert(data_types_equal(left, right));
    return left;
  }
  case EXPR_FN_DEF:
    return make_function_data_type(annotator, expr->node.fn_def->return_type);
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
                               struct ModuleStatementNode *statement) {
  switch (statement->type) {
  case MOD_STMT_LET: {
    assert(lookup_symbol(annotator, statement->node.decl->symbol) == NULL);
    struct DataType *type =
        infer_type(annotator, statement->node.decl->expression);
    if (statement->node.decl->has_type) {
      assert(data_types_equal(type, statement->node.decl->data_type));
    } else {
      statement->node.decl->data_type = type;
      statement->node.decl->has_type = true;
    }
    insert_symbol_entry(annotator, (struct SymbolTableEntry){
                                       .symbol = statement->node.decl->symbol,
                                       .type = type,
                                       .next = NULL,
                                   });
    break;
  }
  case MOD_STMT_CONST: {
    assert(lookup_symbol(annotator, statement->node.decl->symbol) == NULL);
    struct DataType *type =
        infer_type(annotator, statement->node.decl->expression);
    if (statement->node.decl->has_type) {
      assert(data_types_equal(type, statement->node.decl->data_type));
    } else {
      statement->node.decl->data_type = type;
      statement->node.decl->has_type = true;
    }
    insert_symbol_entry(annotator, (struct SymbolTableEntry){
                                       .symbol = statement->node.decl->symbol,
                                       .type = type,
                                       .next = NULL,
                                   });
    break;
  }
  case MOD_STMT_ASSIGN: {
    struct SymbolTableEntry *entry =
        lookup_symbol(annotator, statement->node.decl->symbol);
    assert(entry != NULL);

    assert(data_types_equal(
        entry->type,
        infer_type(annotator, statement->node.assign->expression)));

    break;
  }
  default:
    assert(0);
    break;
  }
}

void annotator_visit_statements(struct Annotator *annotator,
                                struct ModuleStatementNode *statement) {
  struct ModuleStatementNode *curr = statement;

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

bool data_types_equal(struct DataType *left, struct DataType *right) {
  if (left->kind != right->kind) {
    return false;
  }

  switch (left->kind) {
  case DTK_PRIMITIVE:
    if (left->value.primitive != right->value.primitive) {
      return false;
    }
    return true;
  case DTK_FUNCTION:
    return data_types_equal(left->value.function.return_type,
                            right->value.function.return_type);
  };
}
