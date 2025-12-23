#include "llvm-backend/code_generator.h"
#include "annotator.h"
#include "arena_allocator.h"
#include "ast.h"
#include "luna_string.h"
#include "module_symbol_table_builder.h"
#include "llvm-c/Core.h"
#include "llvm-c/Target.h"
#include "llvm-c/Types.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

LLVMTypeRef cg_get_type(struct CodeGenerator *code_generator,
                        struct DataType *data_type);
LLVMValueRef cg_visit_expr(struct CodeGenerator *code_generator,
                           struct ExpressionNode *expr);
LLVMValueRef cg_coerce(struct CodeGenerator *cg, LLVMValueRef val,
                       LLVMTypeRef dest_type);

void cg_gen_assignment(struct CodeGenerator *code_generator,
                       LLVMValueRef source_value, LLVMTypeRef source_type,
                       struct ExpressionNode *expression);

void cg_visit_function_statements(struct CodeGenerator *code_generator,
                                  struct FunctionStatementNode *stmt);
size_t
cg_count_structure_definition_fields(struct StructFieldDefinitionNode *field);

size_t
cg_count_field_access_depth(struct StructFieldAccessExpressionNode *field);

void cg_visit_struct_definition(
    struct CodeGenerator *code_generator,
    struct StructDefinitionExpressionNode *definition);

size_t count_function_arguments(struct FunctionArgumentNode *argument) {
  if (argument == NULL) {
    return 0;
  }
  size_t count = 0;
  struct FunctionArgumentNode *current = argument;
  while (current != NULL) {
    count++;
    current = current->next;
  }
  return count;
}

size_t cg_count_field_inner_access_depth(
    struct StructFieldAccessInnerExpressionNode *field) {
  if (field == NULL) {
    return 0;
  }

  if (field->next) {
    return 1 + cg_count_field_inner_access_depth(field->next);
  }

  return 1;
}

size_t
cg_count_field_access_depth(struct StructFieldAccessExpressionNode *field) {
  if (field == NULL) {
    return 0;
  }

  if (field->next) {
    return 1 + cg_count_field_inner_access_depth(field->next);
  }

  return 1;
}

size_t count_function_call_arguments(
    struct FunctionCallArgumentExpressionsNode *argument) {
  if (argument == NULL) {
    return 0;
  }
  size_t count = 0;
  struct FunctionCallArgumentExpressionsNode *current = argument;
  while (current != NULL) {
    count++;
    current = current->next;
  }
  return count;
}

struct CodeGenerator cg_make(struct ArenaAllocator *allocator,
                             struct Annotator *annotator) {
  LLVMModuleRef module = LLVMModuleCreateWithName("m");
  LLVMModuleCreateWithName("global");
  LLVMBuilderRef builder = LLVMCreateBuilder();

  LLVMInitializeAllTargetInfos();
  LLVMInitializeAllTargets();
  LLVMInitializeAllTargetMCs();

  char *triple = LLVMGetDefaultTargetTriple();

  LLVMTargetRef target;
  char *err = NULL;
  if (LLVMGetTargetFromTriple(triple, &target, &err) != 0) {
    printf("Failed to get target from triple: %s\n", err);
    LLVMDisposeMessage(err);
    LLVMDisposeMessage(triple);
    assert(0);
  }

  LLVMTargetMachineRef target_machine =
      LLVMCreateTargetMachine(target, triple, "", "", LLVMCodeGenLevelDefault,
                              LLVMRelocDefault, LLVMCodeModelDefault);

  LLVMTargetDataRef target_data = LLVMCreateTargetDataLayout(target_machine);

  LLVMSetTarget(module, triple);
  char *data_layout = LLVMCopyStringRepOfTargetData(target_data);
  LLVMSetDataLayout(module, data_layout);
  LLVMDisposeMessage(data_layout);
  LLVMDisposeMessage(triple);

  return (struct CodeGenerator){
      .allocator = allocator,
      .annotator = annotator,
      .current_symbol_table = &annotator->root_symbol_table,
      .module = module,
      .builder = builder,
      .target_data = target_data,
      .current_block = NULL,
  };
}

LLVMTypeRef cg_get_func_type(struct CodeGenerator *code_generator,
                             struct DataType *data_type) {

  puts("about to check type for function.");
  LLVMTypeRef return_type;
  if (data_type->value.function.return_type == NULL) {
    return_type = LLVMVoidType();
  } else {
    return_type =
        cg_get_type(code_generator, data_type->value.function.return_type);
  }

  size_t argument_count =
      count_function_arguments(data_type->value.function.arguments);

  LLVMTypeRef function_type = NULL;
  if (argument_count != 0) {
    assert(data_type->value.function.arguments != NULL);

    LLVMTypeRef *argument_types = (LLVMTypeRef *)arena_alloc(
        code_generator->allocator, argument_count * sizeof(LLVMTypeRef));

    struct FunctionArgumentNode *current = data_type->value.function.arguments;

    size_t index = 0;
    while (current != NULL) {
      LLVMTypeRef type = cg_get_type(code_generator, current->data_type);
      argument_types[index++] = type;
      current = current->next;
    }

    function_type =
        LLVMFunctionType(return_type, argument_types, argument_count,
                         data_type->value.function.is_variadic);
  } else {
    function_type = LLVMFunctionType(return_type, NULL, 0,
                                     data_type->value.function.is_variadic);
  }
  return function_type;
}

LLVMTypeRef cg_get_type(struct CodeGenerator *code_generator,
                        struct DataType *data_type) {
  printf("[cg_get_type] checking type: ");
  print_data_type(data_type);
  printf("\n");
  switch (data_type->kind) {
  case DTK_PRIMITIVE:
    switch (data_type->value.primitive.kind) {
    case P_INT:
      return LLVMIntType(data_type->value.primitive.bitwidth);
    case P_FLOAT:
      puts("Floats not impl yet.");
      assert(0);
      break;
    }
    break;
  case DTK_FUNCTION:
    return LLVMPointerType(cg_get_func_type(code_generator, data_type), 0);
  case DTK_VOID: {
    puts("cg_get_type: void");
    return LLVMVoidType();
  }
  case DTK_POINTER: {
    LLVMTypeRef inner =
        cg_get_type(code_generator, data_type->value.pointer_inner);
    return LLVMPointerType(inner, 0);
  }
  case DTK_STRUCTURE_DEF:
    puts("unimplemented for struct def");
    printf("data type: ");
    print_data_type(data_type);
    puts("");
    assert(data_type->value.structure_definition.definition != NULL);
    if (data_type->value.structure_definition.definition->llvm_structure_type ==
        NULL) {
      cg_visit_struct_definition(
          code_generator, data_type->value.structure_definition.definition);
    }
    assert(
        data_type->value.structure_definition.definition->llvm_structure_type !=
        NULL);
    return data_type->value.structure_definition.definition
        ->llvm_structure_type;
  case DTK_STRUCTURE: {
    puts("Unreachable");
    assert(0);
    return NULL;
    // struct SymbolTableEntry *entry = lookup_scoped_symbol_in(
    //     data_type->value.structure.name,
    //     code_generator->current_symbol_table);
    // assert(entry != NULL);
    // printf("entry: '%s' of type: ", entry->symbol.data);
    // print_data_type(entry->type);
    // puts("");
    // assert(entry->type->value.structure_definition.definition != NULL);
    // if (entry->llvm_structure_type == NULL) {
    //   cg_visit_struct_definition(
    //       code_generator, entry->type->value.structure_definition.definition,
    //       entry);
    // }
    // assert(entry->llvm_structure_type != NULL);
    //
    // return entry->llvm_structure_type;
  }
  case DTK_ARRAY:
    puts("Found array in type inference.");
    printf("Length is: %d\n", (int)data_type->value.array.length);
    return LLVMArrayType2(
        cg_get_type(code_generator, data_type->value.array.element_type),
        data_type->value.array.length);
  case DTK_MODULE_DEF:
  case DTK_MODULE:
    puts("Illegal use of module (/def) type.");
    assert(0);
    return NULL;
  case DTK_RESOLVABLE:
    puts("Found resolvable type.");
    if (data_type->value.resolvable.resolved_type == NULL) {
      puts("resolving type.");

      struct SymbolTable *old_symbol_table =
          code_generator->annotator->current_symbol_table;
      code_generator->annotator->current_symbol_table =
          code_generator->current_symbol_table;
      mstb_resolve_types(code_generator->annotator, data_type);
      code_generator->annotator->current_symbol_table = old_symbol_table;

      printf("resolved type: ");
      print_data_type(data_type);
      puts("");
    }
    assert(data_type->value.resolvable.resolved_type != NULL);
    return cg_get_type(code_generator,
                       data_type->value.resolvable.resolved_type);
  default:
    puts("Unknown data type kind.");
    assert(0);
    return NULL;
  }

  assert(0);
  return NULL;
}

LLVMValueRef cg_visit_function_call(struct CodeGenerator *code_generator,
                                    struct FunctionCallExpressionNode *expr) {
  puts("\n\t\t\t\t\t\tCode genning function call.");
  struct SymbolTableEntry *symbol =
      lookup_scoped_symbol_in(expr->name, code_generator->current_symbol_table);
  assert(symbol != NULL);

  assert(symbol->type != NULL);
  assert(symbol->type->kind == DTK_FUNCTION);

  struct FunctionType *fn_definition = &symbol->type->value.function;

  assert(symbol->llvm_value != NULL);

  LLVMTypeRef type = cg_get_func_type(code_generator, symbol->type);
  puts("about to load type");
  LLVMValueRef value = LLVMBuildLoad2(code_generator->builder,
                                      cg_get_type(code_generator, symbol->type),
                                      symbol->llvm_value, "");
  puts("did load type.");
  // LLVMTypeRef ptr_type = LLVMPointerType(type, 0);

  // LLVMValueRef fn_pointer =
  //     LLVMBuildLoad2(code_generator->builder, ptr_type, symbol->llvm_value,
  //     "");

  size_t argument_count = count_function_call_arguments(expr->arguments);
  if (argument_count != 0) {
    assert(expr->arguments != NULL);
    puts("cg: function call has arguments.");

    LLVMValueRef *arguments = (LLVMValueRef *)arena_alloc(
        code_generator->allocator, argument_count * sizeof(LLVMValueRef));

    struct FunctionCallArgumentExpressionsNode *current_call_argument =
        expr->arguments;
    struct FunctionArgumentNode *current_definition_argument =
        fn_definition->arguments;

    size_t index = 0;
    while (current_call_argument != NULL) {
      // TODO: consider variadic
      if (!fn_definition->is_variadic) {
        assert(current_definition_argument != NULL);
      } else {
        puts("skipping definition type check match for variadic");
      }

      LLVMValueRef value =
          cg_visit_expr(code_generator, current_call_argument->argument);

      // TODO: Get actual function argument definitions from symbol->..->fn_call
      // so we can coerce correectly.

      if (!fn_definition->is_variadic) {
        LLVMValueRef coerced =
            cg_coerce(code_generator, value,
                      cg_get_type(code_generator,
                                  current_definition_argument->data_type));
        arguments[index++] = coerced;
        current_definition_argument = current_definition_argument->next;
      } else {
        arguments[index++] = value;
      }
      current_call_argument = current_call_argument->next;
    }

    puts("about to build call");

    printf("symbol value: %s : \n", symbol->symbol.data);
    print_data_type(symbol->type);
    puts("");

    return LLVMBuildCall2(code_generator->builder, type, value, arguments,
                          argument_count, "");
  }

  return LLVMBuildCall2(code_generator->builder, type, value, NULL, 0, "");
}

// TODO/NOTE: this is an awful hack because the annotator infer_type uses the
// annotators current symbol table. Given code gen runs after all annotation,
// the annotator will likely have some root module as its current symbol table.
// we should likely update the infer_type to take a table but its used so many
// places that would be a bit annoying.
struct DataType *cg_infer_type(struct CodeGenerator *code_generator,
                               struct ExpressionNode *expression) {
  struct SymbolTable *old_current =
      code_generator->annotator->current_symbol_table;
  code_generator->annotator->current_symbol_table =
      code_generator->current_symbol_table;

  struct DataType *type = infer_type(code_generator->annotator, expression);

  code_generator->annotator->current_symbol_table = old_current;
  return type;
}

LLVMValueRef cg_visit_struct_field_access_expr(
    struct CodeGenerator *cg,
    struct StructFieldAccessExpressionNode *field_access_expr) {
  assert(field_access_expr != NULL);

  struct SymbolTableEntry *entry = lookup_scoped_symbol_in(
      field_access_expr->root_symbol, cg->current_symbol_table);
  assert(entry != NULL);

  if (field_access_expr->next == NULL) {
    return entry->llvm_value;
  }

  assert(0);
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

void cg_visit_field_inner_access_expr(
    struct StructFieldAccessInnerExpressionNode *field_access_expr,
    LLVMValueRef **field_indicies, size_t *index,
    struct StructFieldDefinitionNode *field_defs) {
  // field_access_expr = field_access_expr->next;
  while (field_access_expr != NULL) {
    printf("looking for field %s (%zu)..\n", field_access_expr->symbol.data,
           *index);
    struct StructFieldDefinitionNode *field_def = field_defs;

    struct FindFieldDefinitionResult result =
        find_field_definition(field_def, field_access_expr->symbol);
    assert(result.field_definition != NULL);

    puts("Found field.");
    printf("\tindex: %zu\n", result.index);

    (*field_indicies)[*index] = LLVMConstInt(LLVMInt32Type(), result.index, 0);
    *index += 1;

    puts("did weird stuff.");

    if (field_access_expr->next != NULL) {
      puts("still have further stuff.");
      field_def = result.field_definition;
      printf("Field def kind: %d\n", field_def->type->kind);
      puts("");
      assert(field_def->type->kind == DTK_RESOLVABLE);
      assert(field_def->type->value.resolvable.resolved_type != NULL);
      assert(field_def->type->value.resolvable.resolved_type->kind ==
             DTK_STRUCTURE_DEF);
      field_defs = field_def->type->value.resolvable.resolved_type->value
                       .structure_definition.definition->fields;

      // NOTE: If field is pointer, add extra deref for the pointer. We dont
      // do this for last field though.
      if (result.field_definition->type->kind == DTK_POINTER) {
        puts("symbol is pointer, adding extra deref.");
        *field_indicies[*index] = LLVMConstInt(LLVMInt32Type(), 0, 0);
        *index += 1;
      }
    } else {
      puts("has no next.");
    }

    field_access_expr = field_access_expr->next;
  }
}

LLVMValueRef cg_visit_field_access_expr(
    struct CodeGenerator *code_generator,
    struct StructFieldAccessExpressionNode *field_access_expr) {

  struct SymbolTableEntry *entry = lookup_scoped_symbol_in(
      field_access_expr->root_symbol, code_generator->current_symbol_table);

  assert(entry != NULL);
  puts("Found symbol.");
  assert(entry->type != NULL);

  printf("next: %d\n", field_access_expr->next == NULL);
  puts("..");

  if (field_access_expr->next == NULL) {
    puts("Returning field access.");
    return entry->llvm_value;
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  struct SymbolTable *old_symbol_table = code_generator->current_symbol_table;

  code_generator->annotator->current_symbol_table =
      code_generator->current_symbol_table;

  struct StructDefinitionExpressionNode *definition =
      get_or_resolve_struct_definition_from_type(entry->type,
                                                 code_generator->annotator);

  code_generator->annotator->current_symbol_table = old_symbol_table;

  assert(definition != NULL);

  size_t field_depth = cg_count_field_access_depth(field_access_expr);

  printf("field depth: %zu\n", field_depth);

  // NOTE: the * 2 is so each field could be a pointer so we may need up to
  // double the derefs
  LLVMValueRef *field_indicies = arena_alloc(
      code_generator->allocator, sizeof(LLVMValueRef) * field_depth * 2);

  field_indicies[0] = LLVMConstInt(LLVMInt32Type(), 0, 0);

  size_t index = 1;

  LLVMValueRef source = entry->llvm_value;
  LLVMTypeRef source_type = cg_get_type(code_generator, entry->type);

  // NOTE: if base field is a struct pointer we need an initial extra deref.
  if (entry->type->kind == DTK_POINTER) {
    puts("root symbol is pointer, adding extra deref.");
    source =
        LLVMBuildLoad2(code_generator->builder,
                       cg_get_type(code_generator, entry->type), source, "");
    source_type = cg_get_type(code_generator, entry->type->value.pointer_inner);
  }

  ////////////////

  cg_visit_field_inner_access_expr(field_access_expr->next, &field_indicies,
                                   &index, definition->fields);

  LLVMValueRef field_ptr = LLVMBuildGEP2(code_generator->builder, source_type,
                                         source, field_indicies, index, "");

  return field_ptr;
}

LLVMValueRef cg_visit_expr(struct CodeGenerator *code_generator,
                           struct ExpressionNode *expr) {
  printf("[cg_visit_expr] %d\n", expr->type);
  switch (expr->type) {
  case EXPR_BINARY:
    printf("[cg_visit_expr] in binary of type %d\n", expr->node.binary->type);
    struct DataType *left_type =
        cg_infer_type(code_generator, expr->node.binary->left);
    struct DataType *right_type =
        cg_infer_type(code_generator, expr->node.binary->right);
    struct DataType *common_type =
        get_common_type(code_generator->allocator, left_type, right_type);
    LLVMTypeRef common = cg_get_type(code_generator, common_type);
    LLVMValueRef left = cg_visit_expr(code_generator, expr->node.binary->left);
    LLVMValueRef right =
        cg_visit_expr(code_generator, expr->node.binary->right);
    switch (expr->node.binary->type) {
    case BIN_EXPR_ADD:
      puts("got add");
      LLVMValueRef pointer = NULL;
      LLVMValueRef non_pointer = NULL;
      LLVMTypeRef pointer_type = NULL;
      if (left_type->kind == DTK_ARRAY) {
        struct DataType *pointer_data_type = make_pointer_data_type(
            code_generator->allocator, left_type->value.array.element_type);
        pointer_type = cg_get_type(code_generator, pointer_data_type);
        pointer =
            LLVMBuildBitCast(code_generator->builder, left, pointer_type, "");
        non_pointer = right;
      } else if (left_type->kind == DTK_POINTER) {
        pointer = left;
        pointer_type =
            cg_get_type(code_generator, left_type->value.pointer_inner);
        non_pointer = right;
      } else if (right_type->kind == DTK_ARRAY) {
        struct DataType *pointer_data_type = make_pointer_data_type(
            code_generator->allocator, right_type->value.array.element_type);
        pointer_type = cg_get_type(code_generator, pointer_data_type);
        pointer =
            LLVMBuildBitCast(code_generator->builder, right, pointer_type, "");
        non_pointer = left;
      } else if (right_type->kind == DTK_POINTER) {
        pointer = right;
        pointer_type =
            cg_get_type(code_generator, right_type->value.pointer_inner);
        non_pointer = left;
      }

      if (pointer == NULL) {
        return LLVMBuildAdd(code_generator->builder,
                            cg_coerce(code_generator, left, common),
                            cg_coerce(code_generator, right, common), "");
      } else {
        // LLVMValueRef indices[] = {non_pointer};
        // uint64_t size =
        //     LLVMABISizeOfType(code_generator->target_data, pointer_type);
        // LLVMValueRef new_non_pointer =
        //     LLVMBuildMul(code_generator->builder, non_pointer,
        //                  LLVMConstInt(LLVMInt32Type(), size, 0), "");
        //
        // return LLVMBuildAdd(code_generator->builder, pointer,
        //                     LLVMBuildIntToPtr(code_generator->builder,
        //                                       new_non_pointer, common_type,
        //                                       ""),
        //                     "");
        assert(common_type->kind == DTK_POINTER);
        return LLVMBuildGEP2(
            code_generator->builder,
            cg_get_type(code_generator, common_type->value.pointer_inner),
            pointer, &non_pointer, 1, "");
      }
    case BIN_EXPR_SUB:
      puts("got sub");
      return LLVMBuildSub(code_generator->builder, left, right, "");
      break;
    case BIN_EXPR_MUL:
      puts("got mul");
      return LLVMBuildMul(code_generator->builder, left, right, "");
    case BIN_EXPR_DIV:
      puts("got div");
      return LLVMBuildSDiv(code_generator->builder, left, right, "");
    case BIN_EXPR_LT:
      puts("got lt");
      return LLVMBuildICmp(code_generator->builder, LLVMIntSLT,
                           cg_coerce(code_generator, left, common),
                           cg_coerce(code_generator, right, common), "");
    case BIN_EXPR_GT:
      puts("got gt");
      return LLVMBuildICmp(code_generator->builder, LLVMIntSGT,
                           cg_coerce(code_generator, left, common),
                           cg_coerce(code_generator, right, common), "");
    case BIN_EXPR_LEQ:
      puts("got le");
      return LLVMBuildICmp(code_generator->builder, LLVMIntSLE,
                           cg_coerce(code_generator, left, common),
                           cg_coerce(code_generator, right, common), "");
    case BIN_EXPR_GEQ:
      puts("got ge");
      return LLVMBuildICmp(code_generator->builder, LLVMIntSGE,
                           cg_coerce(code_generator, left, common),
                           cg_coerce(code_generator, right, common), "");
    case BIN_EXPR_EQ:
      puts("got eq");
      return LLVMBuildICmp(code_generator->builder, LLVMIntEQ,
                           cg_coerce(code_generator, left, common),
                           cg_coerce(code_generator, right, common), "");
    case BIN_EXPR_NEQ:
      puts("got neq");
      return LLVMBuildICmp(code_generator->builder, LLVMIntNE,
                           cg_coerce(code_generator, left, common),
                           cg_coerce(code_generator, right, common), "");
    }
    break;
  case EXPR_INTEGER_LITERAL:
    return LLVMConstInt(LLVMInt32Type(), expr->node.integer->value, false);
  case EXPR_SYMBOL_LITERAL: {
    puts("getting variable");
    struct SymbolTableEntry *symbol = lookup_scoped_symbol_in(
        expr->node.scoped_symbol, code_generator->current_symbol_table);
    assert(symbol != NULL);
    assert(symbol->llvm_value != NULL);

    if (symbol->type->kind == DTK_ARRAY) {
      return symbol->llvm_value;
    }

    LLVMTypeRef type = cg_get_type(code_generator, symbol->type);

    switch (symbol->symbol_location) {
    case SL_MODULE:
    case SL_ARGUMENT:
    case SL_LOCAL:
      puts("its local");
      return LLVMBuildLoad2(code_generator->builder, type, symbol->llvm_value,
                            "");
    };
    assert(0);
    return symbol->llvm_value;
  }
  case EXPR_STRING_LITERAL: {
    return LLVMBuildGlobalStringPtr(code_generator->builder,
                                    expr->node.string->value.data, "");
    break;
  }
  case EXPR_FN_DEF:
    printf("\n\t\t\t++++++++++++++++PUSHING FUNCTION+++++++++\n\n\n");

    struct SymbolTable *old_current = code_generator->current_symbol_table;
    LLVMBasicBlockRef previous_block = code_generator->current_block;

    LLVMValueRef function = NULL;

    // NOTE: we explicitly want the function type not pointer
    printf("expr: %d, fn_def %d\n", expr != NULL, expr->node.fn_def != NULL);
    LLVMTypeRef function_type =
        cg_get_func_type(code_generator, expr->node.fn_def->function_type);

    char *msg = LLVMPrintTypeToString(function_type);
    printf("the type is: %s\n", msg);
    LLVMDisposeMessage(msg);

    if (expr->node.fn_def->function_type->value.function.extern_name != NULL) {
      puts("code genning extern function.");

      puts("pre add func");
      printf(
          "name: %s\n",
          expr->node.fn_def->function_type->value.function.extern_name->data);
      char *msg = LLVMPrintTypeToString(function_type);
      printf("type is: %s\n", msg);
      LLVMDisposeMessage(msg);

      function = LLVMAddFunction(
          code_generator->module,
          expr->node.fn_def->function_type->value.function.extern_name->data,
          function_type);
      puts("did the add func");
    } else {
      function = LLVMAddFunction(code_generator->module, "", function_type);
      LLVMBasicBlockRef block = LLVMAppendBasicBlock(function, "entry");
      LLVMPositionBuilderAtEnd(code_generator->builder, block);

      code_generator->current_block = block;
      code_generator->current_symbol_table = &expr->node.fn_def->symbol_table;

      printf("about to hop into function. here is its symbol table:\n");
      print_symbol_table(string_make("some_func"),
                         &expr->node.fn_def->symbol_table);

      struct FunctionArgumentNode *argument =
          expr->node.fn_def->function_type->value.function.arguments;
      size_t index = 0;
      while (argument != NULL) {
        struct SymbolTableEntry *symbol = lookup_symbol_in(
            argument->symbol, code_generator->current_symbol_table);
        assert(symbol != NULL);
        LLVMValueRef value = LLVMGetParam(function, index);

        symbol->llvm_value = LLVMBuildAlloca(
            code_generator->builder,
            cg_get_type(code_generator, argument->data_type), "");
        LLVMBuildStore(code_generator->builder, value, symbol->llvm_value);

        // symbol->llvm_value = value;
        index++;
        argument = argument->next;
      }

      cg_visit_function_statements(code_generator, expr->node.fn_def->body);

      if (LLVMGetBasicBlockTerminator(code_generator->current_block) == NULL) {
        assert(expr->node.fn_def->function_type->value.function.return_type
                   ->kind == DTK_VOID);

        LLVMBuildRetVoid(code_generator->builder);
      }

      code_generator->current_symbol_table = old_current;
      code_generator->current_block = previous_block;

      LLVMPositionBuilderAtEnd(code_generator->builder, previous_block);

      puts("non extern.");
      return function;
    }

    printf("\n\t\t\t++++++++++++++++POPPING FUNCTION+++++++++\n\n\n");

    return function;
  case EXPR_FN_CALL:
    return cg_visit_function_call(code_generator, expr->node.fn_call);
  case EXPR_REF: {
    // struct SymbolTableEntry *symbol = lookup_symbol_in(
    //     expr->node.ref_symbol->value, code_generator->current_symbol_table);

    return cg_visit_field_access_expr(code_generator, expr->node.ref_symbol);
    // assert(symbol != NULL);
    // assert(symbol->llvm_value != NULL);
    //
    // return symbol->llvm_value;
  }
  case EXPR_DEREF: {
    LLVMValueRef inner = cg_visit_expr(code_generator, expr->node.deref);
    assert(inner != NULL);

    struct ExpressionNode *deref_expr = expr->node.deref;
    assert(deref_expr != NULL);

    struct DataType *deref_expr_type =
        cg_infer_type(code_generator, deref_expr);

    assert(deref_expr_type != NULL);
    assert(deref_expr_type->kind == DTK_POINTER);

    LLVMTypeRef type =
        cg_get_type(code_generator, deref_expr_type->value.pointer_inner);

    LLVMValueRef value = inner;

    // if (LLVMGetTypeKind(type) == LLVMPointerTypeKind) {
    //   puts("type was pointer type so adding extra load.");
    //   value = LLVMBuildLoad2(code_generator->builder, type, inner, "");
    // }

    return LLVMBuildLoad2(code_generator->builder, type, value, "");
  }
  case EXPR_STRUCT_DEF:
    puts("unimplemented behavior for struct def.");
    assert(0);
    break;
  case EXPR_STRUCT_INIT: {
    struct SymbolTableEntry *entry = lookup_scoped_symbol_in(
        expr->node.struct_init->name, code_generator->current_symbol_table);

    assert(entry->type != NULL);
    assert(entry->type->kind == DTK_STRUCTURE_DEF);
    assert(entry->type->value.structure_definition.definition != NULL);
    assert(entry->type->value.structure_definition.definition
               ->llvm_structure_type != NULL);

    LLVMTypeRef struct_type =
        entry->type->value.structure_definition.definition->llvm_structure_type;

    // assert(0);

    assert(entry != NULL);
    assert(struct_type != NULL);

    struct StructFieldDefinitionNode *field_def =
        entry->type->value.structure_definition.definition->fields;

    struct StructFieldInitializerExpressionNode *field_init =
        expr->node.struct_init->fields;

    puts("counting fields..");
    size_t field_count = cg_count_structure_definition_fields(field_def);
    printf("found %zu fields\n", field_count);

    LLVMValueRef local_struct =
        LLVMBuildAlloca(code_generator->builder, struct_type, "");

    puts("Generating field initializers.");
    size_t field_index = 0;
    while (field_def != NULL) {
      LLVMValueRef indicies[2] = {
          LLVMConstInt(LLVMInt32Type(), 0, 0),
          LLVMConstInt(LLVMInt32Type(), field_index++, 0)};

      puts("Building field ptr..");
      LLVMValueRef field_ptr = LLVMBuildGEP2(
          code_generator->builder, struct_type, local_struct, indicies, 2, "");

      LLVMValueRef coereced = cg_coerce(
          code_generator, cg_visit_expr(code_generator, field_init->expression),
          cg_get_type(code_generator, field_def->type));

      puts("Building store..");
      LLVMBuildStore(code_generator->builder, coereced, field_ptr);

      puts("Advancing pointers..");
      field_def = field_def->next;
      field_init = field_init->next;
    }
    puts("Done.");

    return LLVMBuildLoad2(code_generator->builder, struct_type, local_struct,
                          "");
  }
  case EXPR_FIELD_ACCESS: {
    puts("cg visit expr for struct field access.");

    return LLVMBuildLoad2(
        code_generator->builder,
        cg_get_type(code_generator, cg_infer_type(code_generator, expr)),
        cg_visit_field_access_expr(code_generator,
                                   expr->node.struct_field_access),
        "");
  }
  case EXPR_CAST:
    puts("cg expr cast");
    return cg_visit_expr(code_generator, expr->node.cast->expr);
  case EXPR_ARRAY_INITIALIZER:
    puts("cg array initializer.");
    LLVMTypeRef element_type =
        cg_get_type(code_generator,
                    cg_infer_type(code_generator,
                                  expr->node.array_initializers->initializer));
    size_t element_count =
        count_array_initializer_length(expr->node.array_initializers);
    LLVMValueRef array = LLVMBuildArrayAlloca(
        code_generator->builder, element_type,
        LLVMConstInt(LLVMInt32Type(), element_count, 0), "");
    struct ArrayInitializerExpressionNode *initializer =
        expr->node.array_initializers;
    size_t index = 0;
    while (initializer != NULL) {
      // %p = getelementptr i32, i32* %a, i32 5
      LLVMValueRef indices[] = {LLVMConstInt(LLVMInt32Type(), index, 0)};
      LLVMValueRef gep = LLVMBuildGEP2(code_generator->builder, element_type,
                                       array, indices, 1, "");
      LLVMValueRef result =
          cg_visit_expr(code_generator, initializer->initializer);
      LLVMValueRef coerced = cg_coerce(code_generator, result, element_type);
      LLVMBuildStore(code_generator->builder, coerced, gep);
      initializer = initializer->next;
      index++;
    }
    // return array;

    return LLVMBuildLoad2(code_generator->builder,
                          LLVMArrayType2(element_type, element_count), array,
                          "");
  case EXPR_VALUESIZE: {
    uint64_t size = LLVMABISizeOfType(
        code_generator->target_data,
        cg_get_type(code_generator,
                    cg_infer_type(code_generator, expr->node.valuesize)));
    return LLVMConstInt(LLVMInt32Type(), size, 0);
  }
  case EXPR_TYPESIZE: {
    uint64_t size =
        LLVMABISizeOfType(code_generator->target_data,
                          cg_get_type(code_generator, expr->node.typesize));
    return LLVMConstInt(LLVMInt32Type(), size, 0);
  }
  case EXPR_MOD_DEF:
    puts("Visit mod def.");
    assert(0);
    break;
  }
  assert(0);
  return NULL;
}

LLVMValueRef cg_coerce(struct CodeGenerator *cg, LLVMValueRef val,
                       LLVMTypeRef dest_type) {
  LLVMTypeRef source_type = LLVMTypeOf(val);

  LLVMTypeKind source_kind = LLVMGetTypeKind(source_type);
  LLVMTypeKind dest_kind = LLVMGetTypeKind(dest_type);

  char *source_type_str = LLVMPrintTypeToString(source_type);
  char *dest_type_str = LLVMPrintTypeToString(dest_type);

  printf("\n\n\t\nsource type: '%s' -> '%s'\n", source_type_str, dest_type_str);

  LLVMDisposeMessage(source_type_str);
  LLVMDisposeMessage(dest_type_str);

  if (dest_kind == LLVMPointerTypeKind && source_kind == LLVMIntegerTypeKind) {
    puts("\n\t\tcasting int to pointer\n");
    return LLVMBuildIntToPtr(cg->builder, val, dest_type, "");
  } else {
    puts("\n\t\tnot casting to pointer.\n");
  }

  if (source_kind == LLVMIntegerTypeKind && dest_kind == LLVMIntegerTypeKind) {
    puts("doing integer to integer coercsion.");
    uint16_t source_width = LLVMGetIntTypeWidth(source_type);
    uint16_t dest_width = LLVMGetIntTypeWidth(dest_type);
    if (source_width < dest_width) {
      puts("need to extend source");
      return LLVMBuildSExt(cg->builder, val, dest_type, "");
    } else if (source_width > dest_width) {
      puts("need to trunc source");
      if (dest_width == 1) {
        puts("Converting to truthy i1");
        return LLVMBuildICmp(cg->builder, LLVMIntSGT, val,
                             LLVMConstInt(source_type, 1, 0), "");
      }
      return LLVMBuildTrunc(cg->builder, val, dest_type, "");
    } else {
      puts("types are same width");
      return val;
    }
  }

  return val;
}

void cg_visit_decl(struct CodeGenerator *code_generator,
                   struct DeclarationStatementNode *decl) {
  struct SymbolTableEntry *symbol =
      lookup_symbol_in(decl->symbol, code_generator->current_symbol_table);
  assert(symbol != NULL);

  printf("Code genning decl %s.\n", symbol->symbol.data);

  LLVMTypeRef type = cg_get_type(code_generator, decl->data_type);

  // if (decl->data_type->kind == DTK_FUNCTION) {
  //   type = LLVMPointerType(type, 0);
  // } else if (decl->data_type->kind == DTK_POINTER) {
  //   puts("kind is pointer.");
  // }

  LLVMValueRef variable =
      LLVMBuildAlloca(code_generator->builder, type, decl->symbol.data);

  symbol->llvm_value = variable;

  cg_gen_assignment(code_generator, variable, type, decl->expression);
}

size_t
cg_count_structure_definition_fields(struct StructFieldDefinitionNode *field) {
  size_t field_count = 0;
  while (field != NULL) {
    field_count++;
    field = field->next;
  }
  return field_count;
}

void cg_visit_struct_definition(
    struct CodeGenerator *code_generator,
    struct StructDefinitionExpressionNode *definition) {
  puts("code genning struct definition");
  struct StructFieldDefinitionNode *field = definition->fields;
  size_t field_count = 0;
  while (field != NULL) {
    field_count++;
    field = field->next;
  }
  field = definition->fields;

  LLVMTypeRef *field_types =
      arena_alloc(code_generator->allocator, sizeof(LLVMTypeRef) * field_count);

  size_t field_index = 0;
  while (field != NULL) {
    field_types[field_index++] = cg_get_type(code_generator, field->type);
    field = field->next;
  }

  LLVMTypeRef type = LLVMStructType(field_types, field_count, false);
  definition->llvm_structure_type = type;
  // entry->llvm_structure_type = type;

  assert(definition->llvm_structure_type != NULL);

  char *msg = LLVMPrintTypeToString(type);
  printf("llvm struct def type: %s\n", msg);
  LLVMDisposeMessage(msg);
}

void cg_visit_module_decl(struct CodeGenerator *code_generator,
                          struct DeclarationStatementNode *decl) {
  struct SymbolTableEntry *entry =
      lookup_symbol_in(decl->symbol, code_generator->current_symbol_table);
  assert(entry != NULL);

  printf("Code genning MODULE decl %s.\n", entry->symbol.data);

  if (decl->data_type->kind == DTK_STRUCTURE_DEF) {
    cg_visit_struct_definition(
        code_generator, decl->data_type->value.structure_definition.definition);
    puts("TODO: remove this.");
    assert(decl->data_type->value.structure_definition.definition
               ->llvm_structure_type != NULL);
    return;
  } else if (decl->data_type->kind == DTK_MODULE_DEF) {
    puts("Found module def, not code genning module decl normally.");

    struct SymbolTable *old_symbol_table = code_generator->current_symbol_table;
    code_generator->current_symbol_table =
        &decl->expression->node.module_definition->symbol_table;

    LLVMValueRef inner_module_initializer = cg_visit_module_statements(
        code_generator, decl->expression->node.module_definition->statements,
        false, decl->symbol);

    LLVMBuildCall2(code_generator->builder,
                   LLVMFunctionType(LLVMVoidType(), 0, 0, 0),
                   inner_module_initializer, NULL, 0, "");

    code_generator->current_symbol_table = old_symbol_table;

    return;
  }

  LLVMTypeRef type = cg_get_type(code_generator, decl->data_type);

  // if (decl->data_type->kind == DTK_FUNCTION) {
  //   type = LLVMPointerType(type, 0);
  // }

  if (entry->llvm_value == NULL) {
    LLVMValueRef variable = LLVMAddGlobal(code_generator->module, type, "");
    if (decl->data_type->kind == DTK_FUNCTION ||
        decl->data_type->kind == DTK_POINTER) {
      LLVMSetInitializer(variable, LLVMConstPointerNull(type));
    } else {
      LLVMSetInitializer(variable, LLVMConstInt(LLVMInt32Type(), 0, 0));
    }

    entry->llvm_value = variable;
  }

  cg_gen_assignment(code_generator, entry->llvm_value, type, decl->expression);
}

void cg_visit_module_statement(struct CodeGenerator *code_generator,
                               struct ModuleStatementNode *stmt) {
  switch (stmt->type) {
  case MOD_STMT_LET:
  case MOD_STMT_CONST:
    cg_visit_module_decl(code_generator, stmt->node.decl);
    break;
  default:
    puts("Unknown module statement type.");
    assert(0);
    break;
  }
}

void cg_visit_return(struct CodeGenerator *code_generator,
                     struct ReturnStatementNode *ret) {
  if (ret->expression == NULL) {
    LLVMBuildRetVoid(code_generator->builder);
  } else {
    LLVMBuildRet(code_generator->builder,
                 cg_visit_expr(code_generator, ret->expression));
  }
}

void cg_gen_assignment(struct CodeGenerator *code_generator,
                       LLVMValueRef source_value, LLVMTypeRef source_type,
                       struct ExpressionNode *expression) {
  LLVMValueRef result = cg_visit_expr(code_generator, expression);

  // TODO: does this belong somewhere else?
  // if (symbol->type->kind == DTK_FUNCTION) {
  //   type = LLVMPointerType(type, 0);
  // } else if (symbol->type->kind == DTK_POINTER) {
  //   puts("kind is pointer.");
  // }

  LLVMValueRef coerced = cg_coerce(code_generator, result, source_type);

  LLVMBuildStore(code_generator->builder, coerced, source_value);
}

void cg_visit_else(struct CodeGenerator *code_generator,
                   struct IfStatementNode *if_stmt,
                   LLVMBasicBlockRef else_block,
                   LLVMBasicBlockRef prev_merge_block) {
  assert(else_block != NULL);
  assert(prev_merge_block != NULL);

  LLVMBasicBlockRef body_block = else_block;
  LLVMBasicBlockRef merge_block = prev_merge_block;

  LLVMPositionBuilderAtEnd(code_generator->builder, body_block);
  struct SymbolTable *old_current = code_generator->current_symbol_table;
  code_generator->current_symbol_table = &if_stmt->symbol_table;

  code_generator->current_block = body_block;

  cg_visit_function_statements(code_generator, if_stmt->body);

  code_generator->current_symbol_table = old_current;

  if (LLVMGetBasicBlockTerminator(code_generator->current_block) == NULL) {
    LLVMBuildBr(code_generator->builder, merge_block);
  }

  LLVMPositionBuilderAtEnd(code_generator->builder, merge_block);
}

void cg_visit_while(struct CodeGenerator *code_generator,
                    struct WhileStatementNode *while_stmt) {
  LLVMValueRef fn = LLVMGetBasicBlockParent(code_generator->current_block);
  LLVMBasicBlockRef cond_block = LLVMAppendBasicBlock(fn, "while.cond");
  LLVMBasicBlockRef body_block = LLVMAppendBasicBlock(fn, "while.body");
  LLVMBasicBlockRef end_block = LLVMAppendBasicBlock(fn, "while.end");

  LLVMBuildBr(code_generator->builder, cond_block);
  LLVMPositionBuilderAtEnd(code_generator->builder, cond_block);

  LLVMValueRef conditional =
      cg_visit_expr(code_generator, while_stmt->condition);
  conditional = cg_coerce(code_generator, conditional, LLVMInt1Type());

  LLVMBuildCondBr(code_generator->builder, conditional, body_block, end_block);
  LLVMPositionBuilderAtEnd(code_generator->builder, body_block);

  struct SymbolTable *old_current = code_generator->current_symbol_table;
  code_generator->current_symbol_table = &while_stmt->symbol_table;

  LLVMBasicBlockRef old_block = code_generator->current_block;
  code_generator->current_block = body_block;

  cg_visit_function_statements(code_generator, while_stmt->body);

  LLVMBuildBr(code_generator->builder, cond_block);

  code_generator->current_symbol_table = old_current;
  code_generator->current_block = old_block;

  LLVMPositionBuilderAtEnd(code_generator->builder, end_block);
  code_generator->current_block = end_block;
}

void cg_visit_if(struct CodeGenerator *code_generator,
                 struct IfStatementNode *if_stmt,
                 LLVMBasicBlockRef prev_merge_block) {
  LLVMValueRef conditional = cg_visit_expr(code_generator, if_stmt->condition);
  conditional = cg_coerce(code_generator, conditional, LLVMInt1Type());

  LLVMValueRef fn = LLVMGetBasicBlockParent(code_generator->current_block);
  LLVMBasicBlockRef then_block = LLVMAppendBasicBlock(fn, "if.then");
  LLVMBasicBlockRef else_block = LLVMAppendBasicBlock(fn, "if.else");
  LLVMBasicBlockRef merge_block = NULL;

  if (prev_merge_block == NULL) {
    merge_block = LLVMAppendBasicBlock(fn, "if.end");
  } else {
    merge_block = prev_merge_block;
  }
  assert(merge_block != NULL);

  LLVMBuildCondBr(code_generator->builder, conditional, then_block, else_block);

  LLVMPositionBuilderAtEnd(code_generator->builder, then_block);

  struct SymbolTable *old_current = code_generator->current_symbol_table;
  code_generator->current_symbol_table = &if_stmt->symbol_table;

  code_generator->current_block = then_block;

  cg_visit_function_statements(code_generator, if_stmt->body);

  if (LLVMGetBasicBlockTerminator(code_generator->current_block) == NULL) {
    LLVMBuildBr(code_generator->builder, merge_block);
  }

  code_generator->current_symbol_table = old_current;
  code_generator->current_block = else_block;

  if (if_stmt->next != NULL) {
    LLVMPositionBuilderAtEnd(code_generator->builder, else_block);
    LLVMBasicBlockRef old_block = code_generator->current_block;

    code_generator->current_block = else_block;

    if (if_stmt->next->condition == NULL) {
      cg_visit_else(code_generator, if_stmt->next, else_block, merge_block);
    } else {
      assert(merge_block != NULL);
      cg_visit_if(code_generator, if_stmt->next, merge_block);
    }

    code_generator->current_block = old_block;

    if (LLVMGetBasicBlockTerminator(else_block) == NULL) {
      LLVMBuildBr(code_generator->builder, merge_block);
    }
  } else {
    if (LLVMGetBasicBlockTerminator(else_block) == NULL) {
      LLVMPositionBuilderAtEnd(code_generator->builder, else_block);
      LLVMBuildBr(code_generator->builder, merge_block);
    }
  }

  LLVMPositionBuilderAtEnd(code_generator->builder, merge_block);
  code_generator->current_block = merge_block;
}

void cg_visit_function_statement(struct CodeGenerator *code_generator,
                                 struct FunctionStatementNode *stmt) {
  switch (stmt->type) {
  case FN_STMT_LET:
    cg_visit_decl(code_generator, stmt->node.decl);
    break;
  case FN_STMT_CONST:
    cg_visit_decl(code_generator, stmt->node.decl);
    break;
  case FN_STMT_ASSIGN: {
    struct AssignStatementNode *node = stmt->node.assign;

    assert(node != NULL);
    assert(node->source_expression != NULL);
    assert(node->result_expression != NULL);

    if (node->source_expression->type == EXPR_SYMBOL_LITERAL) {
      struct SymbolTableEntry *symbol =
          lookup_scoped_symbol_in(node->source_expression->node.scoped_symbol,
                                  code_generator->current_symbol_table);

      LLVMTypeRef type = cg_get_type(code_generator, symbol->type);

      // TODO: standardize this
      // if (symbol->type->kind == DTK_FUNCTION) {
      //   type = LLVMPointerType(type, 0);
      // }

      cg_gen_assignment(code_generator, symbol->llvm_value, type,
                        node->result_expression);
      break;
    } else if (node->source_expression->type == EXPR_DEREF) {
      struct DataType *source_type =
          cg_infer_type(code_generator, node->source_expression->node.deref);
      assert(source_type->kind == DTK_POINTER);
      cg_gen_assignment(
          code_generator,
          cg_visit_expr(code_generator, node->source_expression->node.deref),
          cg_get_type(code_generator, source_type->value.pointer_inner),
          node->result_expression);
      break;
    } else if (node->source_expression->type == EXPR_FIELD_ACCESS) {
      puts("in field access");
      struct SymbolTable *old_symbol_table =
          code_generator->annotator->current_symbol_table;
      code_generator->annotator->current_symbol_table =
          code_generator->current_symbol_table;
      struct DataType *source_type = infer_type_of_field_access(
          node->source_expression->node.struct_field_access,
          code_generator->annotator);
      code_generator->annotator->current_symbol_table = old_symbol_table;

      puts("about to gen assignment");

      cg_gen_assignment(code_generator,
                        cg_visit_field_access_expr(
                            code_generator,
                            node->source_expression->node.struct_field_access),
                        cg_get_type(code_generator, source_type),
                        node->result_expression);
      break;
    }

    struct DataType *source_type =
        cg_infer_type(code_generator, node->source_expression);

    cg_gen_assignment(
        code_generator, cg_visit_expr(code_generator, node->source_expression),
        cg_get_type(code_generator, source_type), node->result_expression);
    break;

    // switch (node->source_expression->type) {
    // case EXPR_SYMBOL_LITERAL: {
    //   puts("symbol literal assignment");
    //   assert(node->source_expression->node.symbol != NULL);
    //   struct SymbolTableEntry *symbol =
    //       lookup_symbol_in(node->source_expression->node.symbol->value,
    //                        code_generator->current_symbol_table);
    //   assert(symbol != NULL);
    //
    //   cg_gen_assignment(code_generator, symbol, node->result_expression);
    //   break;
    // }
    // case EXPR_DEREF: {
    //   puts("deref assignment");
    //   struct ExpressionNode *deref_expr =
    //   node->source_expression->node.deref; assert(deref_expr != NULL);
    //
    //   LLVMValueRef source = cg_visit_expr(code_generator, deref_expr);
    //
    //   struct SymbolTableEntry *symbol = lookup_symbol_in(
    //       deref_expr->value, code_generator->current_symbol_table);
    //   assert(symbol != NULL);
    //
    //   cg_gen_assignment(code_generator, source, node->result_expression);
    //
    //   assert(0);
    //   break;
    // }
    // case EXPR_BINARY:
    // case EXPR_INTEGER_LITERAL:
    // case EXPR_STRING_LITERAL:
    // case EXPR_FN_DEF:
    // case EXPR_FN_CALL:
    // case EXPR_REF:
    //   puts("illegal expression in left hand side of assignment.");
    //   assert(0);
    //   break;
    // };

    break;
  }
  case FN_STMT_RETURN:
    cg_visit_return(code_generator, stmt->node.ret);
    break;
  case FN_STMT_FN_CALL:
    cg_visit_function_call(code_generator, stmt->node.fn_call);
    break;
  case FN_STMT_WHILE:
    puts("generating while..");
    cg_visit_while(code_generator, stmt->node.while_stmt);
    break;
  case FN_STMT_IF:
    cg_visit_if(code_generator, stmt->node.if_stmt, NULL);
    break;
  }
}

void cg_visit_function_statements(struct CodeGenerator *code_generator,
                                  struct FunctionStatementNode *stmt) {
  struct FunctionStatementNode *curr = stmt;
  while (curr != NULL) {
    cg_visit_function_statement(code_generator, curr);
    curr = curr->next;
  }
}

LLVMValueRef cg_visit_module_statements(struct CodeGenerator *code_generator,
                                        struct ModuleStatementNode *stmt,
                                        bool is_root,
                                        struct LunaString module_name) {
  printf("isroot: %d\n", is_root);
  LLVMBasicBlockRef previous_block = code_generator->current_block;

  char *header = "module_initializer_";
  char module_initializer_name[module_name.length + strlen(header)];
  sprintf((char *)&module_initializer_name, "%s%s", header, module_name.data);

  LLVMValueRef module_initialization_function =
      LLVMAddFunction(code_generator->module, module_initializer_name,
                      LLVMFunctionType(LLVMVoidType(), NULL, 0, 0));
  LLVMBasicBlockRef block =
      LLVMAppendBasicBlock(module_initialization_function, "entry");
  LLVMPositionBuilderAtEnd(code_generator->builder, block);

  code_generator->current_block = block;

  struct ModuleStatementNode *curr = stmt;
  while (curr != NULL) {
    cg_visit_module_statement(code_generator, curr);
    curr = curr->next;
  }

  LLVMBuildRetVoid(code_generator->builder);

  if (previous_block != NULL) {
    code_generator->current_block = previous_block;
    LLVMPositionBuilderAtEnd(code_generator->builder, previous_block);
  }

  return module_initialization_function;
}

void cg_make_entrypoint(struct CodeGenerator *code_generator,
                        LLVMValueRef global_module_initializer) {
  LLVMTypeRef param_types[] = {LLVMInt32Type(), // int argc
                               LLVMPointerType( // char **argv
                                   LLVMPointerType(LLVMInt8Type(), 0), 0)};
  LLVMValueRef entrypoint_function =
      LLVMAddFunction(code_generator->module, "main",
                      LLVMFunctionType(LLVMInt32Type(), param_types, 2, 0));

  LLVMBasicBlockRef block = LLVMAppendBasicBlock(entrypoint_function, "entry");
  LLVMPositionBuilderAtEnd(code_generator->builder, block);

  LLVMBuildCall2(code_generator->builder,
                 LLVMFunctionType(LLVMVoidType(), 0, 0, 0),
                 global_module_initializer, 0, 0, "");

  struct SymbolTableEntry *main_symbol = NULL;
  struct LunaString main_symbol_name = string_make("main");
  // NOTE: We do an extra unwrapping here because the all the root modules are
  // wrapped in a src modules.
  struct SymbolTableEntry *entry =
      code_generator->annotator->root_symbol_table.head->type->value
          .module_definition->module_definition->symbol_table.head;
  while (entry != NULL) {
    if (entry->type->kind == DTK_MODULE_DEF) {
      struct SymbolTableEntry *subentry = lookup_symbol_in(
          main_symbol_name, &entry->type->value.module_definition
                                 ->module_definition->symbol_table);
      if (subentry != NULL) {
        assert(subentry->type != NULL);
        if (subentry->type->kind == DTK_MODULE_DEF) {
          struct SymbolTableEntry *supersubentry = lookup_symbol_in(
              main_symbol_name, &subentry->type->value.module_definition
                                     ->module_definition->symbol_table);
          if (supersubentry != NULL) {
            main_symbol = supersubentry;
            break;
          }
        }
        main_symbol = subentry;
        break;
      }
    }
    entry = entry->next;
  }

  assert(main_symbol != NULL);
  assert(main_symbol->type != NULL);
  assert(main_symbol->type->kind == DTK_FUNCTION);
  assert(main_symbol->llvm_value != NULL);

  LLVMTypeRef main_func_type =
      cg_get_func_type(code_generator, main_symbol->type);
  LLVMTypeRef ptr_type = LLVMPointerType(main_func_type, 0);

  LLVMValueRef fn_pointer = LLVMBuildLoad2(code_generator->builder, ptr_type,
                                           main_symbol->llvm_value, "");

  LLVMValueRef main_ret;
  if (main_symbol->type->value.function.arguments == NULL) {
    // main has no arguments
    puts("Main takes no arguments");
    main_ret = LLVMBuildCall2(code_generator->builder, main_func_type,
                              fn_pointer, 0, 0, "");

  } else {
    // main must take arguments
    LLVMValueRef args[] = {LLVMGetParam(entrypoint_function, 0),
                           LLVMGetParam(entrypoint_function, 1)};
    puts("Main takes arguments, assuming they're correct");
    main_ret = LLVMBuildCall2(code_generator->builder, main_func_type,
                              fn_pointer, args, 2, "");
  }

  if (main_symbol->type->value.function.return_type->kind == DTK_VOID) {
    LLVMBuildRet(code_generator->builder, LLVMConstInt(LLVMInt32Type(), 0, 0));
  } else {
    LLVMBuildRet(code_generator->builder, main_ret);
  }
}

void cg_prepare_module(struct CodeGenerator *code_generator,
                       struct ModuleStatementNode *root) {
  struct Queue *queue = queue_make(code_generator->allocator);

  queue_push(code_generator->allocator, queue, root);
  while (queue->head != NULL) {
    struct ModuleStatementNode *head = queue_pop_head(queue);

    printf("head: %s\n", head->node.decl->symbol.data);

    if (head->node.decl->expression->type == EXPR_MOD_DEF) {
      struct ModuleStatementNode *stmt =
          head->node.decl->expression->node.module_definition->statements;

      while (stmt != NULL) {
        if (stmt->node.decl->expression->type == EXPR_MOD_DEF) {
          printf("Pushing module def %s\n", stmt->node.decl->symbol.data);
          queue_push(code_generator->allocator, queue, stmt);
        } else if (stmt->node.decl->expression->type == EXPR_STRUCT_DEF) {
          puts("Pushing struct def");
          struct SymbolTableEntry *entry = lookup_symbol_in(
              stmt->node.decl->symbol, &head->node.decl->expression->node
                                            .module_definition->symbol_table);
          assert(entry != NULL);
          cg_visit_struct_definition(
              code_generator, stmt->node.decl->expression->node.struct_def);
          assert(stmt->node.decl->expression->node.struct_def
                     ->llvm_structure_type != NULL);
        } else {
          struct SymbolTableEntry *entry = lookup_symbol_in(
              stmt->node.decl->symbol, &head->node.decl->expression->node
                                            .module_definition->symbol_table);
          assert(entry != NULL);
          assert(entry->llvm_value == NULL);
          struct SymbolTable *old_symbol_table =
              code_generator->current_symbol_table;
          code_generator->current_symbol_table =
              &head->node.decl->expression->node.module_definition
                   ->symbol_table;
          LLVMTypeRef type =
              cg_get_type(code_generator, stmt->node.decl->data_type);
          code_generator->current_symbol_table = old_symbol_table;
          LLVMValueRef variable =
              LLVMAddGlobal(code_generator->module, type, "");

          if (stmt->node.decl->data_type->kind == DTK_FUNCTION ||
              stmt->node.decl->data_type->kind == DTK_POINTER) {
            LLVMSetInitializer(variable, LLVMConstPointerNull(type));
          } else {
            LLVMSetInitializer(variable, LLVMConstInt(LLVMInt32Type(), 0, 0));
          }
          entry->llvm_value = variable;
        }
        stmt = stmt->next;
      }
    }
  }
}
