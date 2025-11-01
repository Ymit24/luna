#include "annotator.h"
#include "arena_allocator.h"
#include "ast.h"
#include "luna_string.h"
#include <alloca.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool data_types_equal(struct DataType *left, struct DataType *right);
void insert_symbol_entry(struct Annotator *annotator,
                         struct SymbolTableEntry entry);
void annotator_visit_function_statements(
    struct Annotator *annotator, struct FunctionStatementNode *statement);
void annotator_visit_function_statement(
    struct Annotator *annotator, struct FunctionStatementNode *statement);

void print_struct_def_data_type(
    struct StructDefinitionExpressionNode *definition);

void annotator_visit_expr(struct Annotator *annotator,
                          struct ExpressionNode *expr);

struct Annotator annotator_make(struct ArenaAllocator *allocator) {
  struct Annotator annotator = (struct Annotator){
      .allocator = allocator,
      .data_type_table = (struct DataTypeTable){.head = NULL},
      .current_function = NULL,
      .root_symbol_table = (struct SymbolTable){.head = NULL,
                                                .type = STT_MOD,
                                                .parent = NULL,
                                                .current_index = 0},
  };
  return annotator;
}

struct DataType *make_void_data_type(struct ArenaAllocator *allocator) {
  return ast_promote(allocator, &(struct DataType){.kind = DTK_VOID},
                     sizeof(struct DataType));
}

struct DataType *make_structure_data_type(struct ArenaAllocator *allocator,
                                          struct StructType type) {
  return ast_promote(allocator,
                     &(struct DataType){.kind = DTK_STRUCTURE,
                                        .next = NULL,
                                        .value.structure = type},
                     sizeof(struct DataType));
}

struct DataType *
make_structure_def_data_type(struct ArenaAllocator *allocator,
                             struct StructDefinitionType type) {
  return ast_promote(allocator,
                     &(struct DataType){.kind = DTK_STRUCTURE_DEF,
                                        .next = NULL,
                                        .value.structure_definition = type},
                     sizeof(struct DataType));
}

struct DataType *
make_integer_primitive_data_type(struct ArenaAllocator *allocator,
                                 uint16_t bitwidth, bool is_signed) {
  return ast_promote(
      allocator,
      &(struct DataType){.kind = DTK_PRIMITIVE,
                         .value.primitive =
                             (struct PrimitiveType){.kind = P_INT,
                                                    .bitwidth = bitwidth,
                                                    .is_signed = is_signed}},
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
                                         struct LunaString *extern_name,
                                         bool is_variadic) {
  return ast_promote(allocator,
                     &(struct DataType){
                         .kind = DTK_FUNCTION,
                         .value.function.return_type = return_type,
                         .value.function.extern_name = extern_name,
                         .value.function.arguments = arguments,
                         .value.function.is_variadic = is_variadic,
                         .next = NULL,
                     },
                     sizeof(struct DataType));
}

void annotator_initialize_primitives(struct Annotator *annotator) {
  assert(0);
  annotator->data_type_table.head = NULL;
  // struct DataType *primitives[] = {
  //     ast_promote(
  //         annotator->allocator,
  //         &(struct DataType){.kind = DTK_PRIMITIVE, .value.primitive = P_I8},
  //         sizeof(struct DataType)),
  //     ast_promote(
  //         annotator->allocator,
  //         &(struct DataType){.kind = DTK_PRIMITIVE, .value.primitive =
  //         P_I32}, sizeof(struct DataType)),
  //     ast_promote(
  //         annotator->allocator,
  //         &(struct DataType){.kind = DTK_PRIMITIVE, .value.primitive =
  //         P_BOOL}, sizeof(struct DataType))
  //
  // };
  //
  // for (uint16_t i = 0; i < 1; i++) {
  //   primitives[i]->next = annotator->data_type_table.head;
  //   annotator->data_type_table.head = primitives[i];
  // }
  //
  // insert_symbol_entry(annotator, (struct SymbolTableEntry){
  //                                    .symbol = string_make("true"),
  //                                    .type = primitives[1],
  //                                    .llvm_value = NULL,
  //                                    .llvm_structure_type = NULL,
  //                                    .next = NULL,
  //                                    .memory_segment = MS_STATIC,
  //                                    .symbol_location = SL_MODULE,
  //                                });
  // insert_symbol_entry(annotator, (struct SymbolTableEntry){
  //                                    .symbol = string_make("false"),
  //                                    .type = primitives[1],
  //                                    .llvm_value = NULL,
  //                                    .llvm_structure_type = NULL,
  //                                    .next = NULL,
  //                                    .memory_segment = MS_STATIC,
  //                                    .symbol_location = SL_MODULE,
  //                                });
}

void print_symbol_table(struct LunaString name,
                        struct SymbolTable *symbol_table) {
  struct SymbolTableEntry *symbol_table_entry = symbol_table->head;
  printf("Symbol Table (%s):\n", name.data);
  while (symbol_table_entry != NULL) {
    if (symbol_table_entry->type != NULL) {
      printf("\t%s: ", symbol_table_entry->symbol.data);
      print_data_type(symbol_table_entry->type);
      printf("\n");
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
    print_data_type(data_type);
    data_type = data_type->next;
  }
}

// TODO: this fails for when say "a" exists and you're looking for "ab", it will
// sometimes find "a" still.
struct SymbolTableEntry *lookup_symbol_in(struct LunaString symbol,
                                          struct SymbolTable *symbol_table) {
  printf("About to check symbol: '%s'\n", symbol.data);
  printf("here is the current symbol table:\n");

  print_symbol_table(string_make("anon"), symbol_table);
  printf("\n\n");

  struct SymbolTableEntry *entry = symbol_table->head;

  while (entry != NULL) {
    if (entry->symbol.length == symbol.length &&
        strncmp(entry->symbol.data, symbol.data, symbol.length) == 0) {
      printf("found match!\n");
      break;
    }
    printf("did not match with: %s\n", entry->symbol.data);

    entry = entry->next;
  }

  if (entry == NULL) {
    puts("didn't find match in this level, checking next.");
    printf("has parent table: %d\n", symbol_table->parent != NULL);
    struct SymbolTable *current_symbol_table = symbol_table;
    while (current_symbol_table->parent != NULL) {
      current_symbol_table = current_symbol_table->parent;
      if (current_symbol_table->type == STT_FN) {
        puts("skipping functional symbol table");
        // continue;
      }
      puts("in non function symbol table, searching here.");
      return lookup_symbol_in(symbol, current_symbol_table);
    }
    return NULL;
  }

  if (entry->symbol.length != symbol.length) {
    puts("this shouldnt happen..");
    return NULL;
  }

  puts("done");
  return entry;
}

struct SymbolTableEntry *lookup_symbol(struct Annotator *annotator,
                                       struct LunaString symbol) {
  assert(annotator->current_symbol_table != NULL);
  return lookup_symbol_in(symbol, annotator->current_symbol_table);
}

struct StructDefinitionExpressionNode *
get_or_resolve_struct_definition_from_type(struct DataType *type,
                                           struct SymbolTable *symbol_table) {
  assert(type != NULL);

  assert(type->kind == DTK_STRUCTURE ||
         (type->kind == DTK_POINTER && type->value.pointer_inner != NULL &&
          type->value.pointer_inner->kind == DTK_STRUCTURE));

  struct StructType *struct_type = NULL;
  if (type->kind == DTK_STRUCTURE) {
    struct_type = &type->value.structure;
  } else {
    struct_type = &type->value.pointer_inner->value.structure;
  }

  if (struct_type->definition != NULL) {
    return struct_type->definition;
  }

  struct SymbolTableEntry *entry =
      lookup_symbol_in(struct_type->name, symbol_table);
  assert(entry != NULL);

  assert(entry->type != NULL);
  assert(entry->type->kind == DTK_STRUCTURE_DEF);
  assert(entry->type->value.structure_definition.definition != NULL);

  struct_type->definition = entry->type->value.structure_definition.definition;

  return struct_type->definition;
}

struct DataType *infer_type_of_field_access(
    struct StructFieldAccessExpressionNode *field_accessor,
    struct SymbolTable *symbol_table) {
  assert(field_accessor != NULL);

  struct SymbolTableEntry *entry =
      lookup_symbol_in(field_accessor->symbol, symbol_table);

  assert(entry != NULL);
  assert(entry->type != NULL);
  if (field_accessor->next == NULL) {
    return entry->type;
  }

  struct StructDefinitionExpressionNode *definition =
      get_or_resolve_struct_definition_from_type(entry->type, symbol_table);

  assert(definition != NULL);

  struct DataType *next = infer_type_of_field_access(field_accessor->next,
                                                     &definition->symbol_table);

  return next;
}

struct DataType *infer_type(struct Annotator *annotator,
                            struct ExpressionNode *expr) {
  assert(expr != NULL);

  switch (expr->type) {
  case EXPR_INTEGER_LITERAL:
    // TODO: Do we want to auto downsize to I8? we need to handle implicit
    // casting
    if (labs(expr->node.integer->value) < UINT8_MAX) {
      puts("infered i/u8");
      return make_integer_primitive_data_type(annotator->allocator, 8,
                                              expr->node.integer->value < 0);
    } else if (labs(expr->node.integer->value) < UINT16_MAX) {
      puts("infered i/u16");
      return make_integer_primitive_data_type(annotator->allocator, 16,
                                              expr->node.integer->value < 0);
    } else if (labs(expr->node.integer->value) < UINT32_MAX) {
      puts("infered i/u32");
      return make_integer_primitive_data_type(annotator->allocator, 32,
                                              expr->node.integer->value < 0);
    }
    puts("infered by default i/u64");
    return make_integer_primitive_data_type(annotator->allocator, 64,
                                            expr->node.integer->value < 0);
  case EXPR_SYMBOL_LITERAL: {
    puts("inferring on symb lit.");
    assert(expr->node.symbol != NULL);
    printf("symbol is: %s\n", expr->node.symbol->value.data);
    struct SymbolTableEntry *entry =
        lookup_symbol(annotator, expr->node.symbol->value);
    assert(entry != NULL);
    return entry->type;
  }
  case EXPR_STRING_LITERAL: {
    puts("infered string");
    return make_pointer_data_type(annotator, make_integer_primitive_data_type(
                                                 annotator->allocator, 8, 0));
  }
  case EXPR_BINARY: {
    puts("Infering on binary..");
    struct DataType *left = infer_type(annotator, expr->node.binary->left);
    struct DataType *right = infer_type(annotator, expr->node.binary->right);
    assert(data_types_equal(left, right));
    // TODO: return the "greater" of the two types. e.g. between i32 & *i32 ->
    // *i32, or i8 & i64 -> i64
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

    printf("type infer on function call: '%s'\n", entry->symbol.data);
    return entry->type->value.function.return_type;
  }
  case EXPR_REF: {
    return make_pointer_data_type(
        annotator, infer_type_of_field_access(expr->node.struct_field_access,
                                              annotator->current_symbol_table));
  }
  case EXPR_DEREF: {
    puts("doing deref");
    assert(expr->node.deref != NULL);

    struct DataType *inner = infer_type(annotator, expr->node.deref);

    assert(inner != NULL);
    assert(inner->kind == DTK_POINTER);
    printf("\n\n\t<<<<<--------------inner pointer type: [");
    print_data_type(inner->value.pointer_inner);
    printf("]\n\n\n\n\n");
    return inner->value.pointer_inner;
  }
  case EXPR_STRUCT_DEF: {
    puts("making structure definition.");
    struct StructFieldDefinitionNode *field = expr->node.struct_def->fields;
    while (field != NULL) {
      if (field->type->kind == DTK_STRUCTURE &&
          field->type->value.structure.definition == NULL) {
        puts("structure type hasnt been resolved yet, resolving.");
        struct SymbolTableEntry *struct_def_symbol =
            lookup_symbol(annotator, field->type->value.structure.name);
        assert(struct_def_symbol != NULL);
        assert(struct_def_symbol->type != NULL);
        assert(struct_def_symbol->type->kind == DTK_STRUCTURE_DEF);

        field->type->value.structure.definition =
            struct_def_symbol->type->value.structure_definition.definition;
        printf("field type: ");
        print_data_type(field->type);
        puts("");
      }
      field = field->next;
    }
    struct DataType *data_type = make_structure_def_data_type(
        annotator->allocator,
        (struct StructDefinitionType){.definition = expr->node.struct_def});

    if (expr->node.struct_def->fields != NULL &&
        expr->node.struct_def->fields->type->kind == DTK_STRUCTURE) {
      printf("data type result: ");
      print_data_type(data_type);
      puts("");
    }

    return data_type;
  }
  case EXPR_STRUCT_INIT:
    puts("expr struct init.");

    puts("looking for structure definition..");
    struct SymbolTableEntry *entry = lookup_symbol_in(
        expr->node.struct_init->name, annotator->current_symbol_table);

    printf("did find? %d\n", entry != NULL);
    puts("doing checks..");

    assert(entry != NULL);
    assert(entry->type != NULL);
    assert(entry->type->kind == DTK_STRUCTURE_DEF);
    puts("passed checks.");
    printf("struct def symbol has type:\n\t");
    print_data_type(entry->type);
    puts("");
    printf("definition is:\n\t");
    print_struct_def_data_type(
        entry->type->value.structure_definition.definition);
    puts("");

    struct DataType *data_type = make_structure_data_type(
        annotator->allocator,
        (struct StructType){
            .name = expr->node.struct_init->name,
            .definition = entry->type->value.structure_definition.definition});
    printf("created data type:\n\t");
    print_data_type(data_type);
    puts("");
    return data_type;
  case EXPR_FIELD_ACCESS:
    puts("expr field access");
    struct DataType *field_accessor_type = infer_type_of_field_access(
        expr->node.struct_field_access, annotator->current_symbol_table);
    printf("found field accessor of type: ");
    print_data_type(field_accessor_type);
    puts("");
    return field_accessor_type;
  case EXPR_CAST:
    assert(expr->node.cast != NULL);
    return expr->node.cast->type;
  default:
    puts("fell through default");
    printf("kind: %d\n", expr->type);
    break;
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

struct SymbolTable *
find_parent_table(struct SymbolTable *symbol_table,
                  enum SymbolTableType new_symbol_table_type) {
  assert(symbol_table != NULL);
  struct SymbolTable *current = symbol_table;

  switch (new_symbol_table_type) {
  case STT_FN:
    puts("looking from function");
    while (current->parent != NULL && current->type != STT_MOD) {
      current = current->parent;
    }
    assert(current != NULL);
    assert(current->type != STT_FN);
    return current;
  case STT_MOD:
  case STT_SCOPE:
  case STT_STRUCT:
    puts("looking from scope");
    // current = current->parent;
    break;
  };

  assert(current != NULL);
  return current;
}

void annotator_visit_expr(struct Annotator *annotator,
                          struct ExpressionNode *expr) {
  assert(expr != NULL);
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
        .type = STT_FN,
        .parent = find_parent_table(annotator->current_symbol_table, STT_FN),
    };
    annotator->current_symbol_table = &expr->node.fn_def->symbol_table;
    annotator->current_function =
        &expr->node.fn_def->function_type->value.function;

    struct FunctionArgumentNode *argument =
        annotator->current_function->arguments;
    puts("about to add arguments to functions symbol table.");
    while (argument != NULL) {
      printf("\tadding an argument..\n");
      // TODO: Maybe do memory segment here for parameter
      insert_symbol_entry_in(annotator, annotator->current_symbol_table,
                             (struct SymbolTableEntry){
                                 .symbol = argument->symbol,
                                 .type = argument->data_type,
                                 .llvm_value = NULL,
                                 .llvm_structure_type = NULL,
                                 .next = NULL,
                                 .symbol_location = SL_ARGUMENT,
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
    puts("deleteme: (fn_call) Got to this spot and not sure if needed.");
    assert(expr->node.fn_call != NULL);
    struct FunctionCallArgumentExpressionsNode *arg =
        expr->node.fn_call->arguments;
    while (arg != NULL) {
      annotator_visit_expr(annotator, arg->argument);
      arg = arg->next;
    }
    // assert(0);
    // TODO: Do we need to do anything here?
    break;
  case EXPR_REF: {
    puts("deleteme: (fn_ref) Got to this spot and not sure if needed.");
    struct SymbolTableEntry *entry =
        lookup_symbol(annotator, expr->node.ref_symbol->symbol);

    assert(entry != NULL);
    assert(entry->type != NULL);

    // TODO: Do we need to do anything here?
    break;
  }
  case EXPR_DEREF: {
    puts("visiting deref.");
    assert(expr->node.deref != NULL);

    struct DataType *inner = infer_type(annotator, expr->node.deref);

    assert(inner != NULL);
    assert(inner->kind == DTK_POINTER);
    puts("done visit deref.");
    break;
  }
  case EXPR_STRUCT_DEF: {
    puts("[visit expr for struct] not sure if anything should happen here..");
    assert(expr->node.struct_def != NULL);

    struct SymbolTable *old_symbol_table = annotator->current_symbol_table;

    expr->node.struct_def->symbol_table = (struct SymbolTable){
        .head = NULL,
        .type = STT_STRUCT,
        .parent =
            find_parent_table(annotator->current_symbol_table, STT_STRUCT),
    };

    annotator->current_symbol_table = &expr->node.struct_def->symbol_table;

    struct StructFieldDefinitionNode *field = expr->node.struct_def->fields;

    while (field != NULL) {
      insert_symbol_entry_in(annotator, annotator->current_symbol_table,
                             (struct SymbolTableEntry){
                                 .symbol = field->name,
                                 .type = field->type,
                                 .llvm_value = NULL,
                                 .llvm_structure_type = NULL,
                                 // TODO: do we need new symbol location?
                                 .symbol_location = SL_LOCAL,
                                 .next = NULL,

                             });
      field = field->next;
    }

    annotator->current_symbol_table = old_symbol_table;
    break;
  }
  case EXPR_STRUCT_INIT:
    puts("[visit expr for struct initializer] not sure if anything should "
         "happen here..");

    struct SymbolTableEntry *struct_def_symbol =
        lookup_symbol(annotator, expr->node.struct_init->name);
    assert(struct_def_symbol != NULL);
    assert(struct_def_symbol->type != NULL);
    assert(struct_def_symbol->type->kind == DTK_STRUCTURE_DEF);

    struct StructDefinitionExpressionNode *def =
        struct_def_symbol->type->value.structure_definition.definition;

    assert(def != NULL);

    if (def->fields != NULL) {
      assert(expr->node.struct_init->fields != NULL);

      struct StructFieldDefinitionNode *field_def = def->fields;
      struct StructFieldInitializerExpressionNode *field_init =
          expr->node.struct_init->fields;

      while (field_def != NULL) {
        assert(field_init != NULL);
        assert(strings_equal(field_def->name, field_init->name));

        assert(data_types_equal(infer_type(annotator, field_init->expression),
                                field_def->type));

        field_def = field_def->next;
        field_init = field_init->next;
      }
    }
    break;
  case EXPR_FIELD_ACCESS:
    puts("[visit expr for struct field access] not sure if anything should "
         "happen here..");
    break;
  case EXPR_CAST:
    puts("[visit cast]");
    annotator_visit_expr(annotator, expr->node.cast->expr);
    break;
  }
}

void annotator_visit_decl(struct Annotator *annotator,
                          struct DeclarationStatementNode *decl,
                          bool is_module) {
  assert(decl != NULL);
  printf("symbol is: %s\n", decl->symbol.data);
  assert(lookup_symbol(annotator, decl->symbol) == NULL);
  puts("precheck");
  struct DataType *type = infer_type(annotator, decl->expression);
  puts("postcheck");

  assert(type != NULL);

  if (decl->data_type != NULL && decl->data_type->kind == DTK_STRUCTURE) {
    if (decl->data_type->value.structure.definition == NULL) {
      puts("structure type hasnt been resolved yet, resolving.");
      struct SymbolTableEntry *struct_def_symbol =
          lookup_symbol(annotator, decl->data_type->value.structure.name);
      assert(struct_def_symbol != NULL);
      assert(struct_def_symbol->type != NULL);
      assert(struct_def_symbol->type->kind == DTK_STRUCTURE_DEF);

      // puts("should remove this.");
      // assert(0);

      decl->data_type->value.structure.definition =
          struct_def_symbol->type->value.structure_definition.definition;
    }
  }

  printf("\n");
  printf("[annotator_visit_decl] symbol (%s) has infered type: (",
         decl->symbol.data);
  print_data_type(decl->data_type);
  printf(") and the decl expression type is inferred as (");
  print_data_type(type);
  printf(")\n");

  puts("d.");

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
  // TODO: Memory segment here.
  insert_symbol_entry(annotator,
                      (struct SymbolTableEntry){
                          .symbol = decl->symbol,
                          .type = decl->data_type,
                          .llvm_value = NULL,
                          .llvm_structure_type = NULL,
                          .next = NULL,
                          .symbol_location = is_module ? SL_MODULE : SL_LOCAL,
                      });
  annotator_visit_expr(annotator, decl->expression);
}

void annotator_visit_module_statement(struct Annotator *annotator,
                                      struct ModuleStatementNode *statement) {
  switch (statement->type) {
  case MOD_STMT_LET:
  case MOD_STMT_CONST:
    annotator_visit_decl(annotator, statement->node.decl, true);
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

void annotator_visit_while_statement(struct Annotator *annotator,
                                     struct WhileStatementNode *while_stmt) {
  assert(while_stmt != NULL);

  struct SymbolTable *old_current = annotator->current_symbol_table;

  while_stmt->symbol_table = (struct SymbolTable){
      .head = NULL,
      .type = STT_SCOPE,
      .parent = find_parent_table(annotator->current_symbol_table, STT_SCOPE),
  };

  annotator->current_symbol_table = &while_stmt->symbol_table;

  annotator_visit_function_statements(annotator, while_stmt->body);

  annotator->current_symbol_table = old_current;
}

void annotator_visit_if_statement(struct Annotator *annotator,
                                  struct IfStatementNode *if_stmt) {
  if (if_stmt->condition != NULL) {
    annotator_visit_expr(annotator, if_stmt->condition);
  } else {
    assert(if_stmt->next == NULL);
  }

  struct SymbolTable *old_current = annotator->current_symbol_table;

  if_stmt->symbol_table = (struct SymbolTable){
      .head = NULL,
      .type = STT_SCOPE,
      .parent = find_parent_table(annotator->current_symbol_table, STT_SCOPE),
  };

  annotator->current_symbol_table = &if_stmt->symbol_table;

  annotator_visit_function_statements(annotator, if_stmt->body);

  annotator->current_symbol_table = old_current;

  if (if_stmt->next != NULL) {
    annotator_visit_if_statement(annotator, if_stmt->next);
  }
}

void annotator_visit_function_statement(
    struct Annotator *annotator, struct FunctionStatementNode *statement) {
  switch (statement->type) {
  case FN_STMT_LET:
  case FN_STMT_CONST:
    annotator_visit_decl(annotator, statement->node.decl, false);
    break;
  case FN_STMT_ASSIGN: {
    struct DataType *source_type =
        infer_type(annotator, statement->node.assign->source_expression);
    struct DataType *dest_type =
        infer_type(annotator, statement->node.assign->result_expression);

    assert(data_types_equal(dest_type, source_type));

    // NOTE: check inner expressions for validity
    annotator_visit_expr(annotator, statement->node.assign->source_expression);
    annotator_visit_expr(annotator, statement->node.assign->result_expression);
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
    printf("\n\n\t\tFN_STMT_FN_CALL\n\n");
    struct FunctionCallExpressionNode *fn_call = statement->node.fn_call;
    assert(fn_call != NULL);

    printf("\n\tname is: %s\n\n", fn_call->name.data);

    struct SymbolTableEntry *entry = lookup_symbol(annotator, fn_call->name);
    assert(entry != NULL);
    assert(entry->type->kind == DTK_FUNCTION);
    struct FunctionType *fn_def = &entry->type->value.function;

    struct FunctionCallArgumentExpressionsNode *call_arg = fn_call->arguments;
    puts("about to check call args.");
    while (call_arg != NULL) {
      annotator_visit_expr(annotator, call_arg->argument);
      call_arg = call_arg->next;
    }
    puts("done check call args");

    if (fn_def->is_variadic) {
      puts("WARN: dont do type inference for variadic function");
      break;
    }

    call_arg = fn_call->arguments;
    struct FunctionArgumentNode *def_arg = fn_def->arguments;

    puts("checking args..");
    while (call_arg != NULL && def_arg != NULL) {
      struct DataType *type = infer_type(annotator, call_arg->argument);

      assert(data_types_equal(type, def_arg->data_type));

      call_arg = call_arg->next;
      def_arg = def_arg->next;

      printf("call arg null: %d, def arg null %d\n", call_arg == NULL,
             def_arg == NULL);

      assert((call_arg == NULL) == (def_arg == NULL));
    }

    // TODO: check types
    break;
  }
  case FN_STMT_WHILE:
    puts("TODO: do stuff for while.");

    annotator_visit_while_statement(annotator, statement->node.while_stmt);
    break;
  case FN_STMT_IF:
    puts("TODO: do stuff for if.");

    annotator_visit_if_statement(annotator, statement->node.if_stmt);
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

  if (left->kind == DTK_PRIMITIVE && left->value.primitive.kind == P_INT &&
      right->kind == DTK_POINTER) {
    // TODO: Handle proper pointer math stuff
    return true;
  }

  if (left->kind != right->kind) {
    return false;
  }

  switch (left->kind) {
  case DTK_PRIMITIVE:
    if (left->value.primitive.kind == P_INT &&
        right->value.primitive.kind == P_INT) {
      puts("Allowing any int to go into any other int.");
      return true;
    } else if (left->value.primitive.kind == P_FLOAT &&
               right->value.primitive.kind == P_INT) {
      puts("Illegal to store float directly into int, must convert manually.");
      return false;
    }
    puts("Other prim prim is valid.");
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
  case DTK_STRUCTURE:
    puts("unimplemented behavior for structs.");
    if (!strings_equal(left->value.structure.name,
                       right->value.structure.name)) {
      return false;
    }

    puts("[NOTE] we should check for more than just symbol equality, but for "
         "now lets just do this.");

    // TODO: implement structural equality check.

    return true;
    assert(0);
    break;
  case DTK_STRUCTURE_DEF:
    puts("unimplemented behavior for struct defs.");
    assert(0);
    break;
  };
  return false;
}

// NOTE: hi it`s me grace your wife`

void print_struct_def_data_type(
    struct StructDefinitionExpressionNode *definition) {
  if (definition == NULL) {
    printf("struct{?}");
    return;
  }
  printf("struct{");
  struct StructFieldDefinitionNode *current = definition->fields;
  while (current != NULL) {
    printf("%s:", current->name.data);
    print_data_type(current->type);
    if (current->next != NULL) {
      printf(",");
    }
    current = current->next;
  }
  printf("}");
}

void print_data_type(struct DataType *data_type) {
  if (data_type == NULL) {
    printf("[null data_type]");
    return;
  }
  switch (data_type->kind) {
  case DTK_VOID:
    printf("void");
    break;
  case DTK_PRIMITIVE:
    switch (data_type->value.primitive.kind) {
    case P_INT:
      if (data_type->value.primitive.is_signed) {
        printf("i");
      } else {
        printf("u");
      }
      printf("%d", data_type->value.primitive.bitwidth);
      break;
    case P_FLOAT:
      printf("<float, unimplemented>");
      break;
      ;
    }
    break;
  case DTK_POINTER:
    printf("*");
    print_data_type(data_type->value.pointer_inner);
    break;
  case DTK_FUNCTION: {
    printf("fn");
    struct FunctionType function = data_type->value.function;
    if (function.extern_name != NULL) {
      printf("@extern[%s]", function.extern_name->data);
    }
    if (function.is_variadic == true) {
      printf("@variadic");
    }
    struct FunctionArgumentNode *argument = function.arguments;
    if (argument != NULL) {
      printf("(");

      while (argument != NULL) {
        print_data_type(argument->data_type);
        argument = argument->next;
        if (argument != NULL) {
          printf(",");
        }
      }

      printf(")");
    }
    if (function.return_type != NULL) {
      printf(":");
      print_data_type(function.return_type);
    }
    break;
  }
  case DTK_STRUCTURE:
    printf("(%s)", data_type->value.structure.name.data);
    print_struct_def_data_type(data_type->value.structure.definition);
    break;
  case DTK_STRUCTURE_DEF: {
    print_struct_def_data_type(
        data_type->value.structure_definition.definition);
    break;
  }
  }
}
