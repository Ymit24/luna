#include "llvm-backend/code_generator.h"
#include "annotator.h"
#include "arena_allocator.h"
#include "ast.h"
#include "luna_string.h"
#include "llvm-c/Core.h"
#include "llvm-c/Types.h"
#include <assert.h>
#include <stdio.h>

LLVMValueRef cg_visit_expr(struct CodeGenerator *code_generator,
                           struct ExpressionNode *expr);

void cg_gen_assignment(struct CodeGenerator *code_generator,
                       struct SymbolTableEntry *symbol,
                       struct ExpressionNode *expression);

void cg_visit_function_statements(struct CodeGenerator *code_generator,
                                  struct FunctionStatementNode *stmt);

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

  return (struct CodeGenerator){
      .allocator = allocator,
      .annotator = annotator,
      .current_symbol_table = &annotator->root_symbol_table,
      .module = module,
      .builder = builder,
  };
}

LLVMTypeRef cg_get_type(struct CodeGenerator *code_generator,
                        struct DataType *data_type) {
  switch (data_type->kind) {
  case DTK_PRIMITIVE:
    switch (data_type->value.primitive) {
    case P_I8:
      return LLVMInt8Type();
    case P_I32:
      return LLVMInt32Type();
    case P_BOOL:
      return LLVMInt1Type();
    default:
      puts("Unknown primitive data type.");
      assert(0);
      return NULL;
    }
    break;
  case DTK_FUNCTION: {
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

      struct FunctionArgumentNode *current =
          data_type->value.function.arguments;

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
  case DTK_VOID: {
    puts("cg_get_type: void");
    return LLVMVoidType();
  }
  case DTK_POINTER: {
    LLVMTypeRef inner =
        cg_get_type(code_generator, data_type->value.pointer_inner);
    return LLVMPointerType(inner, 0);
  }
  default:
    puts("Unknown data type kind.");
    assert(0);
    return NULL;
  }
}

LLVMValueRef cg_visit_function_call(struct CodeGenerator *code_generator,
                                    struct FunctionCallExpressionNode *expr) {

  puts("Code genning function call.");
  struct SymbolTableEntry *symbol =
      lookup_symbol_in(expr->name, code_generator->current_symbol_table);
  assert(symbol != NULL);
  printf("type kind: %d\n", symbol->type->kind);
  assert(symbol->llvm_value != NULL);

  LLVMTypeRef type = cg_get_type(code_generator, symbol->type);
  LLVMTypeRef ptr_type = LLVMPointerType(type, 0);

  LLVMValueRef fn_pointer =
      LLVMBuildLoad2(code_generator->builder, ptr_type, symbol->llvm_value, "");

  size_t argument_count = count_function_call_arguments(expr->arguments);
  if (argument_count != 0) {
    assert(expr->arguments != NULL);
    puts("cg: function call has arguments.");

    LLVMValueRef *arguments = (LLVMValueRef *)arena_alloc(
        code_generator->allocator, argument_count * sizeof(LLVMValueRef));

    struct FunctionCallArgumentExpressionsNode *current = expr->arguments;

    size_t index = 0;
    while (current != NULL) {
      LLVMValueRef value = cg_visit_expr(code_generator, current->argument);
      arguments[index++] = value;
      current = current->next;
    }

    return LLVMBuildCall2(code_generator->builder, type, fn_pointer, arguments,
                          argument_count, "");
  }

  return LLVMBuildCall2(code_generator->builder, type, fn_pointer, NULL, 0, "");
}

LLVMValueRef cg_visit_expr(struct CodeGenerator *code_generator,
                           struct ExpressionNode *expr) {
  printf("[cg_visit_expr] %d\n", expr->type);
  switch (expr->type) {
  case EXPR_BINARY:
    printf("[cg_visit_expr] in binary of type %d\n", expr->node.binary->type);
    LLVMValueRef left = cg_visit_expr(code_generator, expr->node.binary->left);
    LLVMValueRef right =
        cg_visit_expr(code_generator, expr->node.binary->right);
    switch (expr->node.binary->type) {
    case BIN_EXPR_ADD:
      puts("got add");
      return LLVMBuildAdd(code_generator->builder, left, right, "");
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
    }
    break;
  case EXPR_INTEGER_LITERAL:
    return LLVMConstInt(LLVMInt32Type(), expr->node.integer->value, false);
  case EXPR_SYMBOL_LITERAL: {
    puts("getting variable");
    struct SymbolTableEntry *symbol = lookup_symbol_in(
        expr->node.symbol->value, code_generator->current_symbol_table);
    assert(symbol != NULL);
    assert(symbol->llvm_value != NULL);

    if (symbol->type->kind == DTK_POINTER) {
      printf("\n\n\t\tBUILDING LOAD FOR POINTER TYPE: %s\n\n\n",
             symbol->symbol.data);
      return LLVMBuildLoad2(code_generator->builder,
                            cg_get_type(code_generator, symbol->type),
                            symbol->llvm_value, "");
    } else {
      printf("grabbing value from non pointer type.\n");
      printf("\n\n\t\tBUILDING LOAD FOR (non) POINTER TYPE: %s\n\n\n",
             symbol->symbol.data);
      return LLVMBuildLoad2(code_generator->builder,
                            cg_get_type(code_generator, symbol->type),
                            symbol->llvm_value, "");
    }
    return symbol->llvm_value;
  }
  case EXPR_STRING_LITERAL: {
    return LLVMBuildGlobalStringPtr(code_generator->builder,
                                    expr->node.string->value.data, "");
    break;
  }
  case EXPR_FN_DEF:
    puts("pushing function");

    struct SymbolTable *old_current = code_generator->current_symbol_table;
    LLVMBasicBlockRef previous_block = code_generator->current_block;

    LLVMValueRef function = NULL;

    LLVMTypeRef function_type =
        cg_get_type(code_generator, expr->node.fn_def->function_type);

    if (expr->node.fn_def->function_type->value.function.extern_name != NULL) {
      puts("code genning extern function.");

      function = LLVMAddFunction(
          code_generator->module,
          expr->node.fn_def->function_type->value.function.extern_name->data,
          function_type);
    } else {
      function = LLVMAddFunction(code_generator->module, "", function_type);
      LLVMBasicBlockRef block = LLVMAppendBasicBlock(function, "entry");
      LLVMPositionBuilderAtEnd(code_generator->builder, block);

      code_generator->current_block = block;
      code_generator->current_symbol_table = &expr->node.fn_def->symbol_table;

      struct FunctionArgumentNode *argument =
          expr->node.fn_def->function_type->value.function.arguments;
      size_t index = 0;
      while (argument != NULL) {
        struct SymbolTableEntry *symbol = lookup_symbol_in(
            argument->symbol, code_generator->current_symbol_table);
        assert(symbol != NULL);
        LLVMValueRef value = LLVMGetParam(function, index);
        symbol->llvm_value = value;
        index++;
        argument = argument->next;
      }

      cg_visit_function_statements(code_generator, expr->node.fn_def->body);

      code_generator->current_symbol_table = old_current;
      code_generator->current_block = previous_block;

      LLVMPositionBuilderAtEnd(code_generator->builder, previous_block);
    }

    puts("popping function");

    return function;
  case EXPR_FN_CALL:
    return cg_visit_function_call(code_generator, expr->node.fn_call);
  case EXPR_REF: {
    struct SymbolTableEntry *symbol = lookup_symbol_in(
        expr->node.ref_symbol->value, code_generator->current_symbol_table);

    assert(symbol != NULL);
    assert(symbol->llvm_value != NULL);

    return symbol->llvm_value;
  }
  case EXPR_DEREF: {
    struct SymbolTableEntry *symbol = lookup_symbol_in(
        expr->node.deref_symbol->value, code_generator->current_symbol_table);

    assert(symbol != NULL);
    assert(symbol->llvm_value != NULL);

    LLVMTypeRef type = cg_get_type(code_generator, symbol->type);

    LLVMValueRef value = symbol->llvm_value;
    if (LLVMGetTypeKind(LLVMTypeOf(symbol->llvm_value)) ==
        LLVMPointerTypeKind) {
      puts("type was pointer type so adding extra load.");
      value = LLVMBuildLoad2(code_generator->builder, type, value, "");
    }

    return LLVMBuildLoad2(code_generator->builder, type, value, "");
  }
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

  return val;
}

void cg_visit_decl(struct CodeGenerator *code_generator,
                   struct DeclarationStatementNode *decl) {
  struct SymbolTableEntry *symbol =
      lookup_symbol_in(decl->symbol, code_generator->current_symbol_table);
  assert(symbol != NULL);
  printf("Code genning decl %s.\n", symbol->symbol.data);

  LLVMTypeRef type = cg_get_type(code_generator, decl->data_type);
  if (decl->data_type->kind == DTK_FUNCTION) {
    type = LLVMPointerType(type, 0);
  } else if (decl->data_type->kind == DTK_POINTER) {
    puts("kind is pointer.");
  }

  LLVMValueRef variable =
      LLVMBuildAlloca(code_generator->builder, type, decl->symbol.data);

  symbol->llvm_value = variable;

  cg_gen_assignment(code_generator, symbol, decl->expression);
}

void cg_visit_module_decl(struct CodeGenerator *code_generator,
                          struct DeclarationStatementNode *decl) {
  struct SymbolTableEntry *symbol =
      lookup_symbol_in(decl->symbol, code_generator->current_symbol_table);
  assert(symbol != NULL);
  printf("Code genning MODULE decl %s.\n", symbol->symbol.data);

  LLVMTypeRef type = cg_get_type(code_generator, decl->data_type);
  if (decl->data_type->kind == DTK_FUNCTION) {
    type = LLVMPointerType(type, 0);
  }

  LLVMValueRef variable = LLVMAddGlobal(code_generator->module, type, "");
  if (decl->data_type->kind == DTK_FUNCTION ||
      decl->data_type->kind == DTK_POINTER) {
    LLVMSetInitializer(variable, LLVMConstPointerNull(type));
  } else {
    LLVMSetInitializer(variable, LLVMConstInt(LLVMInt32Type(), 0, 0));
  }

  symbol->llvm_value = variable;

  cg_gen_assignment(code_generator, symbol, decl->expression);
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
                       struct SymbolTableEntry *symbol,
                       struct ExpressionNode *expression) {
  LLVMValueRef result = cg_visit_expr(code_generator, expression);

  LLVMTypeRef type = cg_get_type(code_generator, symbol->type);

  printf("[cg_gen_assignment]: ");
  print_data_type(symbol->type);
  char *type_str = LLVMPrintTypeToString(type);
  printf(" -> %s\n", type_str);
  LLVMDisposeMessage(type_str);

  if (symbol->type->kind == DTK_FUNCTION) {
    type = LLVMPointerType(type, 0);
  } else if (symbol->type->kind == DTK_POINTER) {
    puts("kind is pointer.");
  }

  LLVMValueRef coerced = cg_coerce(code_generator, result, type);

  LLVMBuildStore(code_generator->builder, coerced, symbol->llvm_value);
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
    struct SymbolTableEntry *symbol = lookup_symbol_in(
        stmt->node.assign->symbol, code_generator->current_symbol_table);
    assert(symbol != NULL);

    cg_gen_assignment(code_generator, symbol, stmt->node.assign->expression);

    // TODO: Do we need something here?
    break;
  }
  case FN_STMT_RETURN:
    cg_visit_return(code_generator, stmt->node.ret);
    break;
  case FN_STMT_FN_CALL:
    cg_visit_function_call(code_generator, stmt->node.fn_call);
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

// TODO/NOTE: return module initialization function. if root then we should add
// "main" to end of our own initiailzier
LLVMValueRef cg_visit_module_statements(struct CodeGenerator *code_generator,
                                        struct ModuleStatementNode *stmt,
                                        bool is_root) {
  LLVMBasicBlockRef previous_block = code_generator->current_block;

  LLVMValueRef module_initialization_function = NULL;
  if (is_root) {
    module_initialization_function =
        LLVMAddFunction(code_generator->module, "main",
                        LLVMFunctionType(LLVMInt32Type(), NULL, 0, 0));
  } else {
    module_initialization_function =
        LLVMAddFunction(code_generator->module, "",
                        LLVMFunctionType(LLVMVoidType(), NULL, 0, 0));
  }
  LLVMBasicBlockRef block =
      LLVMAppendBasicBlock(module_initialization_function, "entry");
  LLVMPositionBuilderAtEnd(code_generator->builder, block);

  code_generator->current_block = block;

  struct ModuleStatementNode *curr = stmt;
  while (curr != NULL) {
    cg_visit_module_statement(code_generator, curr);
    curr = curr->next;
  }

  if (previous_block != NULL) {
    LLVMPositionBuilderAtEnd(code_generator->builder, previous_block);
  }

  if (is_root) {
    struct SymbolTableEntry *main_symbol = lookup_symbol_in(
        string_make("main"), code_generator->current_symbol_table);

    assert(main_symbol != NULL);
    assert(main_symbol->llvm_value != NULL);

    LLVMTypeRef func_type = LLVMFunctionType(LLVMInt32Type(), NULL, 0, 0);
    LLVMTypeRef ptr_type = LLVMPointerType(func_type, 0);

    LLVMValueRef fn_pointer = LLVMBuildLoad2(code_generator->builder, ptr_type,
                                             main_symbol->llvm_value, "");

    if (main_symbol->type->value.function.return_type->kind == DTK_VOID) {
      puts("\n\n------\nVOID\n");
      LLVMBuildCall2(code_generator->builder, func_type, fn_pointer, 0, 0, "");
      LLVMBuildRet(code_generator->builder,
                   LLVMConstInt(LLVMInt32Type(), 0, 0));
    } else {
      puts("\n\n------\nNON VOID\n");
      LLVMValueRef main_ret = LLVMBuildCall2(code_generator->builder, func_type,
                                             fn_pointer, 0, 0, "");
      LLVMBuildRet(code_generator->builder, main_ret);
    }

  } else {
    LLVMBuildRetVoid(code_generator->builder);
  }

  return module_initialization_function;
}
