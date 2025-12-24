#include "annotator.h"
#include "arena_allocator.h"
#include "ast.h"
#include "luna_string.h"
#include "module_symbol_table_builder.h"
#include <alloca.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// bool data_types_equal(struct DataType *left, struct DataType *right);
bool can_operate_data_types(struct DataType *left, struct DataType *right,
                            enum BinaryExpressionType operation);
bool can_store_data_type_in(struct Annotator *annotator,
                            struct DataType *value_type,
                            struct DataType *storage_type);
void annotator_visit_function_statements(
    struct Annotator *annotator, struct FunctionStatementNode *statement);
void annotator_visit_function_statement(
    struct Annotator *annotator, struct FunctionStatementNode *statement);

struct ModuleDefinitionType *
get_or_resolve_mod_definition_from_type(struct DataType *type,
                                        struct SymbolTable *symbol_table);

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

struct DataType *make_pointer_data_type(struct ArenaAllocator *allocator,
                                        struct DataType *inner) {
  return ast_promote(
      allocator,
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

struct DataType *make_array_data_type(struct ArenaAllocator *allocator,
                                      struct DataType *element_type,
                                      uint64_t length) {
  return ast_promote(allocator,
                     &(struct DataType){
                         .kind = DTK_ARRAY,
                         .value.array.element_type = element_type,
                         .value.array.length = length,
                         .next = NULL,
                     },
                     sizeof(struct DataType));
}

void annotator_initialize_primitives(struct Annotator *annotator) {
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
      if (symbol_table_entry->type->kind == DTK_MODULE_DEF) {
        print_symbol_table(symbol_table_entry->symbol,
                           &symbol_table_entry->type->value.module_definition
                                ->module_definition->symbol_table);
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
    print_data_type(data_type);
    data_type = data_type->next;
  }
}

void print_scoped_symbol(struct ScopedSymbolLiteralNode *scoped_symbol) {
  struct ScopedSymbolLiteralNode *symbol = scoped_symbol;
  while (symbol != NULL) {
    printf("%s", symbol->symbol->value.data);
    if (symbol->next != NULL) {
      printf("::");
    }
    symbol = symbol->next;
  }
}

struct SymbolTableEntry *
lookup_scoped_symbol_in(struct ScopedSymbolLiteralNode *scoped_symbol,
                        struct SymbolTable *symbol_table) {
  printf("About to check symbol: '");
  print_scoped_symbol(scoped_symbol);
  printf("'\n");
  printf("here is the current symbol table:\n");

  print_symbol_table(string_make("anon"), symbol_table);
  printf("\n\n");

  struct SymbolTableEntry *entry =
      lookup_symbol_in(scoped_symbol->symbol->value, symbol_table);

  assert(entry != NULL);

  if (scoped_symbol->next == NULL) {
    return entry;
  }

  assert(entry->type != NULL);

  // TODO/NOTE: This may be the wrong symbol table
  struct ModuleDefinitionType *module_definition =
      get_or_resolve_mod_definition_from_type(entry->type, symbol_table);

  struct SymbolTable *new_symbol_table =
      &module_definition->module_definition->symbol_table;

  return lookup_scoped_symbol_in(scoped_symbol->next, new_symbol_table);
}

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

struct ModuleDefinitionType *
get_or_resolve_mod_definition_from_type(struct DataType *type,
                                        struct SymbolTable *symbol_table) {
  assert(type != NULL);

  assert(type->kind == DTK_MODULE || type->kind == DTK_MODULE_DEF);

  if (type->kind == DTK_MODULE_DEF) {
    return type->value.module_definition;
  }

  if (type->kind == DTK_MODULE) {
    if (type->value.module->module_definition != NULL) {
      return type->value.module->module_definition;
    }

    struct SymbolTableEntry *entry =
        lookup_scoped_symbol_in(type->value.module->name, symbol_table);

    assert(entry != NULL);
    return get_or_resolve_mod_definition_from_type(entry->type, symbol_table);
  }

  assert(0);
  return NULL;
}

struct StructDefinitionExpressionNode *
get_or_resolve_struct_definition_from_type(struct DataType *type,
                                           struct Annotator *annotator) {
  assert(type != NULL);

  puts("TYPE IS:\n\t");
  print_data_type(type);
  puts("");

  if (type->kind == DTK_POINTER) {
    assert(type->value.pointer_inner != NULL);
    if (type->value.pointer_inner->kind == DTK_RESOLVABLE) {
      return get_or_resolve_struct_definition_from_type(
          type->value.pointer_inner, annotator);
    } else {
      printf("Pointer inner is not to a resolvable.");
      assert(0);
    }
  } else if (type->kind == DTK_RESOLVABLE) {
    if (type->value.resolvable.resolved_type != NULL) {
      struct DataType *resolved_type = type->value.resolvable.resolved_type;
      assert(resolved_type->kind == DTK_STRUCTURE_DEF);

      return resolved_type->value.structure_definition.definition;
    } else {
      mstb_resolve_types(annotator, type);
      assert(type->value.resolvable.resolved_type != NULL);
      assert(type->value.resolvable.resolved_type->kind == DTK_STRUCTURE_DEF);
      assert(type->value.resolvable.resolved_type->value.structure_definition
                 .definition != NULL);
      return type->value.resolvable.resolved_type->value.structure_definition
          .definition;
    }
  }

  printf("Found type that is neither a structure nor a resolvable.");
  assert(0);

  return NULL;

  // assert(type->kind == DTK_STRUCTURE ||
  //        (type->kind == DTK_POINTER && type->value.pointer_inner != NULL &&
  //         type->value.pointer_inner->kind == DTK_RESOLVABLE &&
  //         type->value.pointer_inner->value.resolvable.resolved_type !=
  //         NULL));
  //
  // struct StructType *struct_type = NULL;
  // if (type->kind == DTK_STRUCTURE) {
  //   struct_type = &type->value.structure;
  // } else {
  //   struct_type = type->value.pointer_inner->value.resolvable.resolved_type;
  // }
  //
  // if (struct_type->definition != NULL) {
  //   printf("get_or_resolve_struct_definition_from_type: found struct def, "
  //          "entry type is:");
  //   print_struct_def_data_type(struct_type->definition);
  //   puts("");
  //   return struct_type->definition;
  // }
  //
  // struct SymbolTableEntry *entry =
  //     lookup_scoped_symbol_in(struct_type->name, symbol_table);
  // assert(entry != NULL);
  //
  // assert(entry->type != NULL);
  // assert(entry->type->kind == DTK_STRUCTURE_DEF);
  // assert(entry->type->value.structure_definition.definition != NULL);
  //
  // struct_type->definition =
  // entry->type->value.structure_definition.definition;
  //
  // printf("get_or_resolve_struct_definition_from_type: resolved struct def, "
  //        "entry type is:");
  // print_data_type(entry->type);
  // puts("");
  //
  // return struct_type->definition;
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
    assert(expr->node.scoped_symbol != NULL);

    printf("symbol is: ");
    print_scoped_symbol(expr->node.scoped_symbol);
    puts("");

    struct SymbolTableEntry *entry = lookup_scoped_symbol_in(
        expr->node.scoped_symbol, annotator->current_symbol_table);
    assert(entry != NULL);
    assert(entry->type != NULL);

    puts("resolved type:");
    print_data_type(entry->type);
    puts("");

    return entry->type;

    // struct DataType *resolvable = ast_promote(
    //     annotator->allocator,
    //     &(struct DataType){.kind = DTK_RESOLVABLE,
    //                        .value.resolvable =
    //                            (struct ResolvableType){
    //                                .scoped_symbol = expr->node.scoped_symbol,
    //                                .resolved_type = NULL}},
    //     sizeof(struct DataType));

    // puts("resolvable type created.");
    // print_data_type(resolvable);
    // puts("");
    // return resolvable;
  }
  case EXPR_STRING_LITERAL: {
    puts("infered string");
    return make_pointer_data_type(
        annotator->allocator,
        make_integer_primitive_data_type(annotator->allocator, 8, 0));
  }
  case EXPR_BINARY: {
    puts("Infering on binary..");
    struct DataType *left = infer_type(annotator, expr->node.binary->left);
    struct DataType *right = infer_type(annotator, expr->node.binary->right);
    printf("Left is: ");
    print_data_type(left);
    printf("\nRight is: ");
    print_data_type(right);
    puts("");
    assert(can_operate_data_types(left, right, expr->node.binary->type));
    // TODO: return the "greater" of the two types. e.g. between i32 & *i32 ->
    // *i32, or i8 & i64 -> i64

    return get_common_type(annotator->allocator, left, right);
  }
  case EXPR_FN_DEF:
    printf("get expr fn def. is null: %d. ret is null: %d\n",
           expr->node.fn_def->function_type == NULL,
           expr->node.fn_def->function_type->value.function.return_type ==
               NULL);
    return expr->node.fn_def->function_type;
  case EXPR_FN_CALL: {
    puts("About to find symbol..");
    struct SymbolTableEntry *entry = lookup_scoped_symbol_in(
        expr->node.fn_call->name, annotator->current_symbol_table);
    puts("Found entry.");
    assert(entry != NULL);
    assert(entry->type->kind == DTK_FUNCTION);

    printf("type infer on function call: '%s'\n", entry->symbol.data);
    puts("");
    return entry->type->value.function.return_type;
  }
  case EXPR_REF: {
    return make_pointer_data_type(annotator->allocator,
                                  infer_type(annotator, expr->node.ref));
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
        // TODO: maybe this dtk_sructure should be a resolvable type?
        struct SymbolTableEntry *struct_def_symbol = lookup_scoped_symbol_in(
            field->type->value.structure.name, annotator->current_symbol_table);
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
    struct SymbolTableEntry *entry = lookup_scoped_symbol_in(
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

    struct DataType *data_type =
        ast_promote(annotator->allocator,
                    &(struct DataType){
                        .kind = DTK_RESOLVABLE,
                        .next = NULL,
                        .value.resolvable =
                            (struct ResolvableType){
                                .scoped_symbol = expr->node.struct_init->name,
                                .resolved_type = NULL,
                            },
                    },
                    sizeof(struct DataType));

    printf("for struct init, resolved type: ");
    print_data_type(data_type);
    puts("");

    // assert(0);

    // struct DataType *data_type = make_structure_data_type(
    //     annotator->allocator,
    //     (struct StructType){
    //         .name = expr->node.struct_init->name,
    //         .definition =
    //         entry->type->value.structure_definition.definition});
    // printf("created data type:\n\t");
    // print_data_type(data_type);
    // puts("");
    return data_type;
  case EXPR_FIELD_ACCESS:
    puts("expr field access");

    assert(expr->node.struct_field_access != NULL);

    struct DataType *struct_type =
        infer_type(annotator, expr->node.struct_field_access->expression);

    assert(struct_type != NULL);

    struct StructDefinitionExpressionNode *struct_def =
        get_or_resolve_struct_definition_from_type(struct_type, annotator);

    struct SymbolTableEntry *field_entry = lookup_symbol_in(
        expr->node.struct_field_access->symbol, &struct_def->symbol_table);

    assert(field_entry != NULL);

    printf("found field accessor of type: ");
    print_data_type(field_entry->type);
    puts("");
    assert(field_entry->type != NULL);
    return field_entry->type;
  case EXPR_CAST:
    assert(expr->node.cast != NULL);
    return expr->node.cast->type;
  case EXPR_ARRAY_INITIALIZER:
    puts("Found array initializer");
    struct DataType *dt = make_array_data_type(
        annotator->allocator,
        infer_type(annotator, expr->node.array_initializers->initializer),
        count_array_initializer_length(expr->node.array_initializers));
    printf("Array type infered: ");
    print_data_type(dt);
    puts("");
    return dt;
  case EXPR_MOD_DEF:
    assert(expr->node.module_definition != NULL);
    return ast_promote(
        annotator->allocator,
        &(struct DataType){
            .kind = DTK_MODULE_DEF,
            .next = NULL,
            .value.module_definition = ast_promote(
                annotator->allocator,
                &(struct ModuleDefinitionType){
                    .module_definition = expr->node.module_definition},
                sizeof(struct ModuleDefinitionType)),

        },
        sizeof(struct DataType));
  case EXPR_VALUESIZE:
    return ast_promote(annotator->allocator,
                       &(struct DataType){.kind = DTK_PRIMITIVE,
                                          .next = NULL,
                                          .value.primitive =
                                              (struct PrimitiveType){
                                                  .kind = P_INT,
                                                  .bitwidth = 32,
                                                  .is_signed = 0,
                                              }},
                       sizeof(struct DataType));

    break;
  case EXPR_TYPESIZE:
    return ast_promote(annotator->allocator,
                       &(struct DataType){.kind = DTK_PRIMITIVE,
                                          .next = NULL,
                                          .value.primitive =
                                              (struct PrimitiveType){
                                                  .kind = P_INT,
                                                  .bitwidth = 32,
                                                  .is_signed = 0,
                                              }},
                       sizeof(struct DataType));
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
                                 // .llvm_structure_type = NULL,
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
    assert(expr->node.ref != NULL);

    switch (expr->node.ref->type) {
    case EXPR_SYMBOL_LITERAL:
    case EXPR_DEREF:
    case EXPR_STRUCT_INIT:
    case EXPR_FIELD_ACCESS:
      printf("Taking reference to expression of type (%d).\n",
             expr->node.ref->type);
      break;
    case EXPR_REF:
    case EXPR_BINARY:
    case EXPR_INTEGER_LITERAL:
    case EXPR_STRING_LITERAL:
    case EXPR_FN_DEF: // TODO: Maybe this is valid? Need to figure out syntax
                      // for getting ref of function
    case EXPR_FN_CALL:
    case EXPR_STRUCT_DEF:
    case EXPR_CAST:
    case EXPR_ARRAY_INITIALIZER:
    case EXPR_MOD_DEF:
    case EXPR_VALUESIZE:
    case EXPR_TYPESIZE:
      printf("Can't take reference to expression of type (%d).\n",
             expr->node.ref->type);
      assert(0);
      break;
    }
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
                                 // .llvm_structure_type = NULL,
                                 // TODO: do we need new symbol location?
                                 .symbol_location = SL_LOCAL,
                                 .next = NULL,

                             });
      field = field->next;
    }

    annotator->current_symbol_table = old_symbol_table;
    break;
  }
  case EXPR_ARRAY_INITIALIZER:
    puts("array initializer");

    struct ArrayInitializerExpressionNode *initializer =
        expr->node.array_initializers;
    if (initializer == NULL) {
      puts("Illegal empty array initializer.");
      assert(0);
      break;
    }
    struct DataType *first_element_type =
        infer_type(annotator, initializer->initializer);
    while (initializer != NULL) {
      assert(can_store_data_type_in(
          annotator, infer_type(annotator, initializer->initializer),
          first_element_type));
      initializer = initializer->next;
    }
    break;
  case EXPR_STRUCT_INIT:
    puts("[visit expr for struct initializer] not sure if anything should "
         "happen here..");

    struct SymbolTableEntry *struct_def_symbol = lookup_scoped_symbol_in(
        expr->node.struct_init->name, annotator->current_symbol_table);
    assert(struct_def_symbol != NULL);
    assert(struct_def_symbol->type != NULL);
    assert(struct_def_symbol->type->kind == DTK_STRUCTURE_DEF);

    struct StructDefinitionExpressionNode *def =
        struct_def_symbol->type->value.structure_definition.definition;

    assert(def != NULL);

    if (def->fields != NULL) {
      struct StructFieldDefinitionNode *root_field_def = def->fields;
      struct StructFieldInitializerExpressionNode *field_init =
          expr->node.struct_init->fields;

      while (field_init != NULL) {
        struct StructFieldDefinitionNode *field_def =
            find_field_definition(root_field_def, field_init->name)
                .field_definition;
        assert(field_def != NULL);
        printf("[def]: %s\n", field_def->name.data);

        struct DataType *inferred_type =
            infer_type(annotator, field_init->expression);

        puts("\n\n");
        printf("inferred type: ");
        print_data_type(inferred_type);
        puts("");
        printf("field type: ");
        print_data_type(field_def->type);
        puts("");

        bool can_store =
            can_store_data_type_in(annotator, inferred_type, field_def->type);

        printf("can store: %d\n", can_store);
        puts("");

        assert(can_store);

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
  case EXPR_MOD_DEF:
    puts("[visit submodule]");

    // expr->node.module_definition->symbol_table = (struct SymbolTable){
    //     .head = NULL,
    //     .type = STT_MOD,
    //     .parent = find_parent_table(annotator->current_symbol_table,
    //     STT_MOD),
    // };

    struct SymbolTable *old_symbol_table = annotator->current_symbol_table;

    annotator->current_symbol_table =
        &expr->node.module_definition->symbol_table;

    annotator_visit_module_statements(annotator,
                                      expr->node.module_definition->statements);

    annotator->current_symbol_table = old_symbol_table;
    break;
  case EXPR_VALUESIZE:
    puts("[visit @valuesize]");
    annotator_visit_expr(annotator, expr->node.valuesize);
    break;
  case EXPR_TYPESIZE:
    puts("[visit @typesize]");
    break;
  }
}

void annotator_visit_decl(struct Annotator *annotator,
                          struct DeclarationStatementNode *decl,
                          bool is_module) {
  assert(decl != NULL);
  printf("symbol is: '%s'\n", decl->symbol.data);
  if (is_module) {
    assert(lookup_symbol(annotator, decl->symbol) != NULL);

    annotator_visit_expr(annotator, decl->expression);
    return;
  } else {
    assert(lookup_symbol(annotator, decl->symbol) == NULL);
  }
  puts("precheck");
  struct DataType *type = infer_type(annotator, decl->expression);
  puts("postcheck");

  assert(type != NULL);

  if (decl->data_type != NULL && decl->data_type->kind == DTK_STRUCTURE) {
    if (decl->data_type->value.structure.definition == NULL) {
      puts("structure type hasnt been resolved yet, resolving.");
      struct SymbolTableEntry *struct_def_symbol =
          lookup_scoped_symbol_in(decl->data_type->value.structure.name,
                                  annotator->current_symbol_table);
      assert(struct_def_symbol != NULL);
      assert(struct_def_symbol->type != NULL);
      assert(struct_def_symbol->type->kind == DTK_STRUCTURE_DEF);

      puts("should remove this.");
      assert(0);

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

  if (decl->has_type) {
    puts("has type, about to check equality");
    printf("is TYPE null?: %d : %d\n", type != NULL, decl->data_type != NULL);
    assert(type != NULL);

    printf("1. type: %p :: %d\n", (void *)type, type == NULL);
    assert(can_store_data_type_in(annotator, type, decl->data_type));
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
                          // .llvm_structure_type = NULL,
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
    puts("Annotating new function line..");
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
  printf("Statement type: ");
  printf("%d\n", statement->type);

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

    assert(can_store_data_type_in(annotator, dest_type, source_type));

    // NOTE: check inner expressions for validity
    annotator_visit_expr(annotator, statement->node.assign->source_expression);
    annotator_visit_expr(annotator, statement->node.assign->result_expression);
    break;
  }
  case FN_STMT_RETURN: {
    printf("Found return.\n");
    puts("");
    assert(statement->node.ret != NULL);
    assert(annotator->current_function != NULL);
    assert(annotator->current_function->return_type != NULL);
    if (statement->node.ret->expression == NULL) {
      assert(annotator->current_function->return_type->kind == DTK_VOID);
      break;
    }
    assert(statement->node.ret->expression != NULL);
    struct DataType *value_type =
        infer_type(annotator, statement->node.ret->expression);
    assert(value_type != NULL);
    assert(can_store_data_type_in(annotator, value_type,
                                  annotator->current_function->return_type));
    break;
  }
  case FN_STMT_FN_CALL: {
    printf("\n\n\t\tFN_STMT_FN_CALL\n\n");
    struct FunctionCallExpressionNode *fn_call = statement->node.fn_call;
    assert(fn_call != NULL);

    printf("\n\tname is:");
    print_scoped_symbol(fn_call->name);
    printf("\n\n");

    struct SymbolTableEntry *entry =
        lookup_scoped_symbol_in(fn_call->name, annotator->current_symbol_table);
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

      assert(can_store_data_type_in(annotator, type, def_arg->data_type));

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
    puts("Annotating new module line..");
    annotator_visit_module_statement(annotator, curr);
    print_symbols(annotator);
    print_data_types(annotator);
    curr = curr->next;
  }
  puts("Finished module annotation.");
}

bool can_store_data_type_in(struct Annotator *annotator,
                            struct DataType *value_type,
                            struct DataType *storage_type) {
  assert(value_type != NULL);
  assert(storage_type != NULL);

  printf("[can_store_data_type_in]: Value Type: <");
  print_data_type(value_type);
  printf(">, Storage Type: <");
  print_data_type(storage_type);
  puts(">");
  puts("");

  if (value_type->kind == DTK_RESOLVABLE) {
    puts("Found resolvable type for value type.");
    mstb_resolve_types(annotator, value_type);

    return can_store_data_type_in(
        annotator, value_type->value.resolvable.resolved_type, storage_type);
  }
  if (storage_type->kind == DTK_RESOLVABLE) {
    puts("Found resolvable for storag type.");
    mstb_resolve_types(annotator, storage_type);
    printf("After resolving storage type: ");
    print_data_type(storage_type);
    puts("");
    assert(storage_type->value.resolvable.resolved_type != NULL);
    return can_store_data_type_in(annotator, value_type,
                                  storage_type->value.resolvable.resolved_type);
  }

  if (value_type->kind == DTK_PRIMITIVE &&
      value_type->value.primitive.kind == P_INT &&
      storage_type->kind == DTK_POINTER) {
    // TODO: Handle proper pointer math stuff
    puts("Storing int directly into pointer is no longer legal, you must cast "
         "manually.");
    assert(0);
    return false;
  }

  if (value_type->kind == DTK_ARRAY && storage_type->kind == DTK_POINTER) {
    puts("Found array -> pointer, checking if array element is of coereceable "
         "type as pointer inner.");
    return can_store_data_type_in(annotator,
                                  value_type->value.array.element_type,
                                  storage_type->value.pointer_inner);
  }

  if (value_type->kind == DTK_STRUCTURE) {
    assert(storage_type->kind == DTK_STRUCTURE_DEF ||
           storage_type->kind == DTK_STRUCTURE);

    printf("storage type: ");
    print_data_type(storage_type);
    puts("");

    puts("[NOTE] we should check for more than just symbol equality, but for "
         "now lets just do this.");

    // TODO: implement structural equality check.

    if (storage_type->kind == DTK_STRUCTURE_DEF) {
      puts("storage type is struct def");
      assert(value_type->value.structure.definition ==
             storage_type->value.structure_definition.definition);
    } else if (storage_type->kind == DTK_STRUCTURE) {
      puts("storage type is structure");
      assert(value_type->value.structure.definition ==
             storage_type->value.structure.definition);

      if (!scoped_symbols_equal(value_type->value.structure.name,
                                storage_type->value.structure.name)) {
        printf("value type name: ");
        print_scoped_symbol(value_type->value.structure.name);
        printf("storage type name: ");
        print_scoped_symbol(storage_type->value.structure.name);
        puts("");
        assert(0);
        return false;
      }
    } else {
      printf("storage type is: ");
      print_data_type(storage_type);
      puts("");
      assert(0);
    }

    return true;
  }

  if (value_type->kind != storage_type->kind) {
    return false;
  }

  switch (value_type->kind) {
  case DTK_PRIMITIVE:
    if (value_type->value.primitive.kind == P_INT &&
        storage_type->value.primitive.kind == P_INT) {
      puts("Allowing any int to go into any other int.");
      return true;
    } else if (value_type->value.primitive.kind == P_FLOAT &&
               storage_type->value.primitive.kind == P_INT) {
      puts("Illegal to store float directly into int, must convert manually.");
      return false;
    }
    puts("Other prim prim is valid.");
    return true;
  case DTK_POINTER:
    return can_store_data_type_in(annotator, value_type->value.pointer_inner,
                                  storage_type->value.pointer_inner);
  case DTK_FUNCTION:
    puts("recurse case");
    printf("left: %d, %d\n", value_type->value.function.return_type == NULL,
           storage_type->value.function.return_type == NULL);
    if (value_type->value.function.extern_name != NULL ||
        storage_type->value.function.extern_name != NULL) {
      if (value_type->value.function.extern_name == NULL ||
          storage_type->value.function.extern_name == NULL) {
        return false;
      } else {
        if (value_type->value.function.extern_name->length !=
            storage_type->value.function.extern_name->length) {
          return false;
        }
        return strncmp(value_type->value.function.extern_name->data,
                       storage_type->value.function.extern_name->data,
                       value_type->value.function.extern_name->length) == 0;
      }
    }
    return can_store_data_type_in(annotator,
                                  value_type->value.function.return_type,
                                  storage_type->value.function.return_type);
  case DTK_VOID:
    return true;
  case DTK_STRUCTURE:
    puts("unreachable");
    assert(0);
    break;
  case DTK_STRUCTURE_DEF:
    puts("unimplemented behavior for struct defs.");
    if (value_type->value.structure_definition.definition ==
        storage_type->value.structure_definition.definition) {
      puts("HAD THE SAME STRUCT DEF");
      return true;
    }
    puts("DIFFERENT STRUCT DEFS");
    return false;
  case DTK_ARRAY:
    puts("Found case of array -> array");
    if (value_type->value.array.length > storage_type->value.array.length) {
      puts("Value array had more elements than storage.");
      return false;
    }
    return can_store_data_type_in(annotator,
                                  value_type->value.array.element_type,
                                  storage_type->value.array.element_type);
  case DTK_MODULE_DEF:
    return true;
  case DTK_MODULE:
    return false;
  case DTK_RESOLVABLE:
    puts("Found resolvable type. Should not be able to reach this?");
    assert(0);
    return false;
  };
  return false;
}

// Given two types find the common type to do intermediary results in, e.g.
// i8 + i16 -> i16
// i32 + *i16 -> *i16
// *i64 - i8 -> *i64
// f32 + i64 -> f32
struct DataType *get_common_type(struct ArenaAllocator *allocator,
                                 struct DataType *left,
                                 struct DataType *right) {
  assert(left != NULL);
  assert(right != NULL);

  printf("[get_common_type]: Left Type: <");
  print_data_type(left);
  printf(">, Right Type: <");
  print_data_type(right);
  puts(">");

  switch (left->kind) {
  case DTK_PRIMITIVE:
    switch (right->kind) {
    case DTK_PRIMITIVE:
      assert(left->value.primitive.kind == right->value.primitive.kind);
      bool is_signed = false;
      if (left->value.primitive.is_signed || right->value.primitive.is_signed) {
        is_signed = true;
      }
      uint8_t bitwidth = left->value.primitive.bitwidth;

      if (right->value.primitive.bitwidth > left->value.primitive.bitwidth) {
        bitwidth = right->value.primitive.bitwidth;
      }
      return make_integer_primitive_data_type(allocator, bitwidth, is_signed);
    case DTK_FUNCTION:
    case DTK_VOID:
    case DTK_POINTER:
      return right;
    case DTK_ARRAY:
      return make_pointer_data_type(allocator, right->value.array.element_type);
    case DTK_STRUCTURE:
    case DTK_STRUCTURE_DEF:
    case DTK_MODULE_DEF:
    case DTK_MODULE:
      break;
    case DTK_RESOLVABLE:
      return get_common_type(allocator, left,
                             right->value.resolvable.resolved_type);
    }
    break;
  case DTK_POINTER:
    return left;
  case DTK_ARRAY:
    return make_pointer_data_type(allocator, left->value.array.element_type);
  case DTK_STRUCTURE:
  case DTK_VOID:
  case DTK_FUNCTION:
  case DTK_STRUCTURE_DEF:
  case DTK_MODULE_DEF:
  case DTK_MODULE:
    assert(0);
    break;
  case DTK_RESOLVABLE:
    return get_common_type(allocator, left->value.resolvable.resolved_type,
                           right);
  }

  assert(0);
  return left;
}

bool can_operate_data_types(struct DataType *left, struct DataType *right,
                            enum BinaryExpressionType operation) {
  assert(left != NULL);
  assert(right != NULL);

  printf("[can_add_data_types]: Left Type: <");
  print_data_type(left);
  printf(">, Right Type: <");
  print_data_type(right);
  puts(">");

  bool operation_is_add_sub =
      operation == BIN_EXPR_ADD || operation == BIN_EXPR_SUB;

  bool operation_is_logical =
      operation == BIN_EXPR_GT || operation == BIN_EXPR_LT ||
      operation == BIN_EXPR_EQ || operation == BIN_EXPR_NEQ ||
      operation == BIN_EXPR_LEQ || operation == BIN_EXPR_GEQ;

  switch (left->kind) {
  case DTK_PRIMITIVE:
    switch (right->kind) {
    case DTK_PRIMITIVE:
      return left->value.primitive.kind == right->value.primitive.kind;
    case DTK_POINTER:
      return left->value.primitive.kind == P_INT && operation_is_add_sub;
    case DTK_ARRAY:
      return left->value.primitive.kind == P_INT && operation_is_add_sub;
    case DTK_FUNCTION:
    case DTK_VOID:
    case DTK_STRUCTURE:
    case DTK_STRUCTURE_DEF:
    case DTK_MODULE_DEF:
    case DTK_MODULE:
      assert(0);
      return false;
    case DTK_RESOLVABLE:
      return can_operate_data_types(left, right->value.resolvable.resolved_type,
                                    operation);
    }
    break;
  case DTK_POINTER:
    if (operation_is_logical) {
      return right->kind == DTK_POINTER;
    }
    return right->kind == DTK_PRIMITIVE &&
           right->value.primitive.kind == P_INT && operation_is_add_sub;
  case DTK_ARRAY:
    return right->kind == DTK_PRIMITIVE &&
           right->value.primitive.kind == P_INT && operation_is_add_sub;
  case DTK_STRUCTURE:
  case DTK_FUNCTION:
  case DTK_STRUCTURE_DEF:
  case DTK_VOID:
  case DTK_MODULE_DEF:
  case DTK_MODULE:
    assert(0);
    return false;
  case DTK_RESOLVABLE:
    return can_operate_data_types(left->value.resolvable.resolved_type, right,
                                  operation);
  }

  return false;
}

// NOTE: This really means, can i store left into right.
// e.g. i8 -> i32 is safe but i32 -> i8 is not safe
// TODO: rewrite this
// bool data_types_equal(struct DataType *value_type,
//                       struct DataType *storage_type) {
//   assert(value_type != NULL);
//   assert(storage_type != NULL);
//
//   printf("Value Type: <");
//   print_data_type(value_type);
//   printf(">, Storage Type: <");
//   print_data_type(storage_type);
//   puts(">");
//
//   if (value_type->kind == DTK_PRIMITIVE &&
//       value_type->value.primitive.kind == P_INT &&
//       storage_type->kind == DTK_POINTER) {
//     // TODO: Handle proper pointer math stuff
//     return true;
//   }
//
//   if (value_type->kind == DTK_ARRAY && storage_type->kind == DTK_POINTER) {
//     puts("Found array -> pointer, checking if array element is of coereceable
//     "
//          "type as pointer inner.");
//     return data_types_equal(value_type->value.array.element_type,
//                             storage_type->value.pointer_inner);
//   }
//
//   if (value_type->kind == DTK_ARRAY && storage_type->kind != DTK_ARRAY) {
//     struct DataType new_value_type = (struct DataType){
//         .kind = DTK_POINTER,
//         .value.pointer_inner = value_type->value.array.element_type};
//     puts("Value type is array, decaying into pointer.");
//     return data_types_equal(&new_value_type, storage_type);
//   }
//
//   if (value_type->kind != storage_type->kind) {
//     return false;
//   }
//
//   switch (value_type->kind) {
//   case DTK_PRIMITIVE:
//     if (value_type->value.primitive.kind == P_INT &&
//         storage_type->value.primitive.kind == P_INT) {
//       puts("Allowing any int to go into any other int.");
//       return true;
//     } else if (value_type->value.primitive.kind == P_FLOAT &&
//                storage_type->value.primitive.kind == P_INT) {
//       puts("Illegal to store float directly into int, must convert
//       manually."); return false;
//     }
//     puts("Other prim prim is valid.");
//     return true;
//   case DTK_POINTER:
//     return data_types_equal(value_type->value.pointer_inner,
//                             storage_type->value.pointer_inner);
//   case DTK_FUNCTION:
//     puts("recurse case");
//     printf("left: %d, %d\n", value_type->value.function.return_type == NULL,
//            storage_type->value.function.return_type == NULL);
//     if (value_type->value.function.extern_name != NULL ||
//         storage_type->value.function.extern_name != NULL) {
//       if (value_type->value.function.extern_name == NULL ||
//           storage_type->value.function.extern_name == NULL) {
//         return false;
//       } else {
//         if (value_type->value.function.extern_name->length !=
//             storage_type->value.function.extern_name->length) {
//           return false;
//         }
//         return strncmp(value_type->value.function.extern_name->data,
//                        storage_type->value.function.extern_name->data,
//                        value_type->value.function.extern_name->length) == 0;
//       }
//     }
//     return data_types_equal(value_type->value.function.return_type,
//                             storage_type->value.function.return_type);
//   case DTK_VOID:
//     return true;
//   case DTK_STRUCTURE:
//     puts("unimplemented behavior for structs.");
//     if (!strings_equal(value_type->value.structure.name,
//                        storage_type->value.structure.name)) {
//       return false;
//     }
//
//     puts("[NOTE] we should check for more than just symbol equality, but for
//     "
//          "now lets just do this.");
//
//     // TODO: implement structural equality check.
//
//     return true;
//     assert(0);
//     break;
//   case DTK_STRUCTURE_DEF:
//     puts("unimplemented behavior for struct defs.");
//     assert(0);
//     break;
//   case DTK_ARRAY:
//     puts("Found case of array -> array");
//     if (value_type->value.array.length > storage_type->value.array.length) {
//       puts("Value array had more elements than storage.");
//       return false;
//     }
//     return data_types_equal(value_type->value.array.element_type,
//                             storage_type->value.array.element_type);
//   };
//   return false;
// }

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
    printf("(");
    print_scoped_symbol(data_type->value.structure.name);
    printf(")");
    print_struct_def_data_type(data_type->value.structure.definition);
    break;
  case DTK_STRUCTURE_DEF: {
    print_struct_def_data_type(
        data_type->value.structure_definition.definition);
    break;
  }
  case DTK_ARRAY:
    printf("[");
    print_data_type(data_type->value.array.element_type);
    printf(";%d]", (int)data_type->value.array.length);
    break;
  case DTK_MODULE_DEF:
    printf("mod");
    break;
  case DTK_MODULE:
    printf("mod_ref");
    break;
  case DTK_RESOLVABLE:
    printf("resolvable(");
    if (data_type->value.resolvable.resolved_type != NULL) {
      print_scoped_symbol(data_type->value.resolvable.scoped_symbol);
      printf(":");
      print_data_type(data_type->value.resolvable.resolved_type);
    } else {
      printf("?");
      print_scoped_symbol(data_type->value.resolvable.scoped_symbol);
    }
    printf(")");
    break;
  }
}

bool scoped_symbols_equal(struct ScopedSymbolLiteralNode *left,
                          struct ScopedSymbolLiteralNode *right) {
  assert(left != NULL);
  assert(right != NULL);

  if (left->remaining != right->remaining) {
    return false;
  }

  if (!strings_equal(left->symbol->value, right->symbol->value)) {
    return false;
  }

  if (left->next == NULL) {
    return true;
  }

  return scoped_symbols_equal(left->next, right->next);
}

struct StructFieldInitializerExpressionNode *
find_field_initializer(struct StructFieldInitializerExpressionNode *root,
                       struct LunaString name) {
  struct StructFieldInitializerExpressionNode *field_init = root;
  while (field_init != NULL) {
    if (strings_equal(name, field_init->name)) {
      printf("found %s\n", field_init->name.data);
      return field_init;
    } else {
      printf("checked %s..\n", field_init->name.data);
    }
    field_init = field_init->next;
  }

  return NULL;
}

struct FindFieldDefinitionResult
find_field_definition(struct StructFieldDefinitionNode *root,
                      struct LunaString name) {
  size_t field_index = 0;
  struct StructFieldDefinitionNode *field_def = root;
  while (field_def != NULL) {
    if (strings_equal(name, field_def->name)) {
      return (struct FindFieldDefinitionResult){.index = field_index,
                                                .field_definition = field_def};
    }
    field_index++;
    field_def = field_def->next;
  }

  return (struct FindFieldDefinitionResult){
      .field_definition = NULL,
      .index = 0,
  };
}
