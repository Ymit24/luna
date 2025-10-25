#include "annotator.h"
#include "arena_allocator.h"
#include "ast.h"
#include "luna_string.h"
#include <alloca.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

bool data_types_equal(struct DataType *left, struct DataType *right);
void insert_symbol_entry(struct Annotator *annotator,
                         struct SymbolTableEntry entry);
void annotator_visit_function_statements(
    struct Annotator *annotator, struct FunctionStatementNode *statement);
void annotator_visit_function_statement(
    struct Annotator *annotator, struct FunctionStatementNode *statement);

struct Annotator annotator_make(struct ArenaAllocator *allocator) {
  struct Annotator annotator = (struct Annotator){
      .allocator = allocator,
      .data_type_table = (struct DataTypeTable){.head = NULL},
      .current_function = NULL,
      .root_symbol_table = (struct SymbolTable){.head = NULL,
                                                .is_function = false,
                                                .parent = NULL,
                                                .current_index = 0},
  };
  return annotator;
}

struct DataType *make_void_data_type(struct ArenaAllocator *allocator) {
  return ast_promote(allocator, &(struct DataType){.kind = DTK_VOID},
                     sizeof(struct DataType));
}

struct DataType *make_primitive_data_type(struct Annotator *annotator,
                                          enum PrimitiveType type) {
  return ast_promote(
      annotator->allocator,
      &(struct DataType){.kind = DTK_PRIMITIVE, .value.primitive = type},
      sizeof(struct DataType));
}

struct DataType *make_pointer_data_type(struct Annotator *annotator,
                                        struct DataType *inner) {
  return ast_promote(
      annotator->allocator,
      &(struct DataType){.kind = DTK_POINTER, .value.pointer_inner = inner},
      sizeof(struct DataType));
}

struct DataType *make_function_data_type(struct ArenaAllocator *allocator,
                                         struct FunctionArgumentNode *arguments,
                                         struct DataType *return_type,
                                         struct LunaString *extern_name) {
  return ast_promote(allocator,
                     &(struct DataType){
                         .kind = DTK_FUNCTION,
                         .value.function.return_type = return_type,
                         .value.function.extern_name = extern_name,
                         .value.function.arguments = arguments,
                         .next = NULL,
                     },
                     sizeof(struct DataType));
}

void annotator_initialize_primitives(struct Annotator *annotator) {
  annotator->data_type_table.head = NULL;
  struct DataType *primitives[] = {
      ast_promote(
          annotator->allocator,
          &(struct DataType){.kind = DTK_PRIMITIVE, .value.primitive = P_I8},
          sizeof(struct DataType)),
      ast_promote(
          annotator->allocator,
          &(struct DataType){.kind = DTK_PRIMITIVE, .value.primitive = P_I32},
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
                                     .llvm_value = NULL,
                                     .next = NULL,
                                     .memory_segment = MS_STATIC,
                                 });
  insert_symbol_entry(annotator, (struct SymbolTableEntry){
                                     .symbol = string_make("false"),
                                     .type = primitives[1],
                                     .llvm_value = NULL,
                                     .next = NULL,
                                     .memory_segment = MS_STATIC,
                                 });
}

void print_symbol_table(struct LunaString name,
                        struct SymbolTable *symbol_table) {
  struct SymbolTableEntry *symbol_table_entry = symbol_table->head;
  printf("Symbol Table (%s):\n", name.data);
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
      case DTK_VOID:
        printf("\t%s: void\n", symbol_table_entry->symbol.data);
        break;
      case DTK_POINTER:
        printf("\t%s: ptr\n", symbol_table_entry->symbol.data);
        break;
      }
    } else {
      printf("\t%s: unknown\n", symbol_table_entry->symbol.data);
    }

    symbol_table_entry = symbol_table_entry->next;
  }
}

void print_symbols(struct Annotator *annotator) {
  print_symbol_table(string_make("root"), &annotator->root_symbol_table);
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
    case DTK_VOID:
      printf("\tvoid\n");
      break;
    case DTK_POINTER:
      printf("\tptr\n");
      break;
    }
    data_type = data_type->next;
  }
}

struct SymbolTableEntry *lookup_symbol_in(struct LunaString symbol,
                                          struct SymbolTable *symbol_table) {
  printf("About to check symbol: %s\n", symbol.data);
  struct SymbolTableEntry *entry = symbol_table->head;

  while (entry != NULL &&
         strncmp(entry->symbol.data, symbol.data, symbol.length) != 0) {
    entry = entry->next;
  }

  if (entry == NULL) {
    struct SymbolTable *current_symbol_table = symbol_table;
    while (current_symbol_table->parent != NULL) {
      current_symbol_table = current_symbol_table->parent;
      if (current_symbol_table->is_function) {
        continue;
      }
      puts("in non function symbol table, searching here.");
      return lookup_symbol_in(symbol, current_symbol_table);
    }
  }

  puts("done");
  return entry;
}

struct SymbolTableEntry *lookup_symbol(struct Annotator *annotator,
                                       struct LunaString symbol) {
  assert(annotator->current_symbol_table != NULL);
  return lookup_symbol_in(symbol, annotator->current_symbol_table);
}

struct DataType *infer_type(struct Annotator *annotator,
                            struct ExpressionNode *expr) {
  switch (expr->type) {
  case EXPR_INTEGER_LITERAL:
    // TODO: Do we want to auto downsize to I8? we need to handle implicit
    // casting
    if (expr->node.integer->value < 256) {
      puts("infered i8");
      return make_primitive_data_type(annotator, P_I8);
    }
    puts("infered i32");
    return make_primitive_data_type(annotator, P_I32);
  case EXPR_SYMBOL_LITERAL: {
    struct SymbolTableEntry *entry =
        lookup_symbol(annotator, expr->node.symbol->value);
    assert(entry != NULL);
    return entry->type;
  }
  case EXPR_STRING_LITERAL: {
    puts("infered string");
    return make_pointer_data_type(annotator,
                                  make_primitive_data_type(annotator, P_I8));
  }
  case EXPR_BINARY: {
    puts("Infering on binary..");
    struct DataType *left = infer_type(annotator, expr->node.binary->left);
    struct DataType *right = infer_type(annotator, expr->node.binary->right);
    assert(data_types_equal(left, right));
    return left;
  }
  case EXPR_FN_DEF:
    printf("get expr fn def. is null: %d. ret is null: %d\n",
           expr->node.fn_def->function_type == NULL,
           expr->node.fn_def->function_type->value.function.return_type ==
               NULL);
    return expr->node.fn_def->function_type;
  case EXPR_FN_CALL: {
    struct SymbolTableEntry *entry =
        lookup_symbol(annotator, expr->node.symbol->value);
    assert(entry != NULL);
    assert(entry->type->kind == DTK_FUNCTION);

    printf("type infer on function call: '%s' %d\n", entry->symbol.data,
           entry->type->kind);
    return entry->type->value.function.return_type;
  }
  case EXPR_REF: {
    struct SymbolTableEntry *entry =
        lookup_symbol(annotator, expr->node.symbol->value);
    assert(entry != NULL);
    return make_pointer_data_type(annotator, entry->type);
  }
  }

  puts("Failed to infer type.");
  assert(0);
  return NULL;
}

void insert_symbol_entry_in(struct Annotator *annotator,
                            struct SymbolTable *symbol_table,
                            struct SymbolTableEntry entry) {
  struct SymbolTableEntry *entry_ptr = ast_promote(
      annotator->allocator, &entry, sizeof(struct SymbolTableEntry));
  entry_ptr->index = symbol_table->current_index;
  symbol_table->current_index++; // TODO: HANDLE DATA TYPE SIZE
  entry_ptr->next = symbol_table->head;
  symbol_table->head = entry_ptr;
}

void insert_symbol_entry(struct Annotator *annotator,
                         struct SymbolTableEntry entry) {
  puts("Inserting symbol into symbol table");

  insert_symbol_entry_in(annotator, annotator->current_symbol_table, entry);
}

struct SymbolTable *find_parent_table(struct SymbolTable *symbol_table) {
  assert(symbol_table != NULL);
  struct SymbolTable *current = symbol_table;

  while (current->parent != NULL && current->is_function) {
    current = current->parent;
  }

  assert(current != NULL);
  assert(current->is_function == false);
  return current;
}

void annotator_visit_expr(struct Annotator *annotator,
                          struct ExpressionNode *expr) {
  switch (expr->type) {
  case EXPR_BINARY:
    annotator_visit_expr(annotator, expr->node.binary->left);
    annotator_visit_expr(annotator, expr->node.binary->right);
    break;
  case EXPR_INTEGER_LITERAL:
    break;
  case EXPR_SYMBOL_LITERAL:
    break;
  case EXPR_STRING_LITERAL:
    break;
  case EXPR_FN_DEF: {
    puts("Visiting function expression");
    struct SymbolTable *old_current = annotator->current_symbol_table;
    struct FunctionType *old_function = annotator->current_function;
    expr->node.fn_def->symbol_table = (struct SymbolTable){
        .head = NULL,
        .is_function = true,
        .parent = find_parent_table(annotator->current_symbol_table),
    };
    annotator->current_symbol_table = &expr->node.fn_def->symbol_table;
    annotator->current_function =
        &expr->node.fn_def->function_type->value.function;

    struct FunctionArgumentNode *argument =
        annotator->current_function->arguments;
    puts("about to add arguments to functions symbol table.");
    while (argument != NULL) {
      printf("\tadding an argument..\n");
      insert_symbol_entry_in(annotator, annotator->current_symbol_table,
                             (struct SymbolTableEntry){
                                 .symbol = argument->symbol,
                                 .type = argument->data_type,
                                 .llvm_value = NULL,
                                 .next = NULL,
                             });
      argument = argument->next;
    }
    annotator_visit_function_statements(annotator, expr->node.fn_def->body);
    annotator->current_function = old_function;
    annotator->current_symbol_table = old_current;
    puts("Done visiting function expression");
    print_symbol_table(string_make("anon function"),
                       &expr->node.fn_def->symbol_table);
    break;
  }
  case EXPR_FN_CALL:
    puts("deleteme: Got to this spot and not sure if needed.");
    // TODO: Do we need to do anything here?
    break;
  case EXPR_REF:
    puts("deleteme: Got to this spot and not sure if needed.");
    // TODO: Do we need to do anything here?
    break;
  }
}

void annotator_visit_decl(struct Annotator *annotator,
                          struct DeclarationStatementNode *decl) {
  assert(lookup_symbol(annotator, decl->symbol) == NULL);
  struct DataType *type = infer_type(annotator, decl->expression);
  printf("got type infer done for %s\n", decl->symbol.data);
  if (decl->data_type != NULL) {
    printf("decl type: %d\n", decl->data_type->kind);
  }
  if (type != NULL) {
    printf("infered type: %d\n", type->kind);
  }
  if (decl->has_type) {
    puts("has type, about to check equality");
    printf("is TYPE null?: %d : %d\n", type != NULL, decl->data_type != NULL);
    assert(type != NULL);

    printf("1. type: %p :: %d\n", (void *)type, type == NULL);
    assert(data_types_equal(type, decl->data_type));
    puts("types matched.");
    printf("type: %d\n", type->kind);
  } else {
    puts("no type");
    decl->data_type = type;
    decl->has_type = true;
  }
  puts("insert decl");
  printf("\n-----\nsymb: %s\n++++\n", decl->symbol.data);
  insert_symbol_entry(annotator, (struct SymbolTableEntry){
                                     .symbol = decl->symbol,
                                     .type = type,
                                     .llvm_value = NULL,
                                     .next = NULL,
                                 });
  annotator_visit_expr(annotator, decl->expression);
}

void annotator_visit_module_statement(struct Annotator *annotator,
                                      struct ModuleStatementNode *statement) {
  switch (statement->type) {
  case MOD_STMT_LET:
  case MOD_STMT_CONST:
    annotator_visit_decl(annotator, statement->node.decl);
    break;
  default:
    puts("Unknown module statement.");
    assert(0);
    break;
  }
}
void annotator_visit_function_statements(
    struct Annotator *annotator, struct FunctionStatementNode *statement) {
  struct FunctionStatementNode *curr = statement;

  puts("Starting function annotation..");

  while (curr != NULL) {
    puts("Annotating new line..");
    annotator_visit_function_statement(annotator, curr);
    print_symbols(annotator);
    print_data_types(annotator);
    curr = curr->next;
  }
  puts("Finished function annotation.");
}

void annotator_visit_function_statement(
    struct Annotator *annotator, struct FunctionStatementNode *statement) {
  switch (statement->type) {

  case FN_STMT_LET:
  case FN_STMT_CONST:
    annotator_visit_decl(annotator, statement->node.decl);
    break;
  case FN_STMT_ASSIGN: {
    struct SymbolTableEntry *entry =
        lookup_symbol(annotator, statement->node.decl->symbol);

    assert(entry != NULL);

    assert(data_types_equal(
        infer_type(annotator, statement->node.assign->expression),
        entry->type));

    annotator_visit_expr(annotator, statement->node.assign->expression);
    break;
  }
  case FN_STMT_RETURN: {
    if (statement->node.ret->expression == NULL) {
      assert(annotator->current_function->return_type->kind == DTK_VOID);
      break;
    }
    assert(
        data_types_equal(infer_type(annotator, statement->node.ret->expression),
                         annotator->current_function->return_type));
    break;
  }
  case FN_STMT_FN_CALL: {
    // TODO: check types
    break;
  }
  default:
    assert(0);
    break;
  }
}

void annotator_visit_module_statements(struct Annotator *annotator,
                                       struct ModuleStatementNode *statement) {
  struct ModuleStatementNode *curr = statement;

  puts("Starting module annotation..");
  while (curr != NULL) {
    puts("Annotating new line..");
    annotator_visit_module_statement(annotator, curr);
    print_symbols(annotator);
    print_data_types(annotator);
    curr = curr->next;
  }
  puts("Finished module annotation.");
}

// NOTE: This really means, can i store left into right.
// e.g. i8 -> i32 is safe but i32 -> i8 is not safe
// TODO: rewrite this
bool data_types_equal(struct DataType *left, struct DataType *right) {
  assert(left != NULL);
  assert(right != NULL);

  if (left->kind == DTK_PRIMITIVE &&
      (left->value.primitive == P_I8 || left->value.primitive == P_I32)) {
    puts("left is primitive");
    if (right->kind == DTK_POINTER &&
        right->value.pointer_inner->kind == DTK_PRIMITIVE) {
      puts("right is pointer to primitive");

      if ((right->value.pointer_inner->value.primitive == P_I8 ||
           right->value.pointer_inner->value.primitive == P_I32)) {
        puts("infered magic pointer case");
        return true;
      }
    }
  }

  if (left->kind != right->kind) {
    return false;
  }

  switch (left->kind) {
  case DTK_PRIMITIVE:
    if ((left->value.primitive == P_I8 || left->value.primitive == P_I32) &&
        (right->value.primitive == P_I8 || right->value.primitive == P_I32)) {
      if (left->value.primitive == P_I8) {
        puts("left is i8");
      } else {
        puts("left is i32");
      }
      if (right->value.primitive == P_I8) {
        puts("right is i8");
      } else {
        puts("right is i32");
      }
      if (left->value.primitive == P_I32 && right->value.primitive == P_I8) {
        puts("Tried to put i32 into an i8 storage.");
        return false;
      }
      puts("compatible int to int storage.");
      return true;
    }
    if (left->value.primitive != right->value.primitive) {
      return false;
    }
    return true;
  case DTK_POINTER:
    return data_types_equal(left->value.pointer_inner,
                            right->value.pointer_inner);
  case DTK_FUNCTION:
    puts("recurse case");
    printf("left: %d, %d\n", left->value.function.return_type == NULL,
           right->value.function.return_type == NULL);
    if (left->value.function.extern_name != NULL ||
        right->value.function.extern_name != NULL) {
      if (left->value.function.extern_name == NULL ||
          right->value.function.extern_name == NULL) {
        return false;
      } else {
        if (left->value.function.extern_name->length !=
            right->value.function.extern_name->length) {
          return false;
        }
        return strncmp(left->value.function.extern_name->data,
                       right->value.function.extern_name->data,
                       left->value.function.extern_name->length) == 0;
      }
    }
    return data_types_equal(left->value.function.return_type,
                            right->value.function.return_type);
  case DTK_VOID:
    return true;
  };
  return false;
}
