#include "llvm-backend/code_generator.h"
#include "annotator.h"
#include "arena_allocator.h"
#include "ast.h"
#include "luna_string.h"
#include "llvm-c/Core.h"
#include "llvm-c/Types.h"
#include <assert.h>
#include <stdio.h>

void cg_visit_function_statements(struct CodeGenerator *code_generator,
                                  struct FunctionStatementNode *stmt);

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

LLVMTypeRef cg_get_type(struct DataType *data_type) {
  switch (data_type->kind) {
  case DTK_PRIMITIVE:
    switch (data_type->value.primitive) {
    case P_INT:
      return LLVMInt32Type();
    case P_BOOL:
      return LLVMInt1Type();
    case P_STRING:
      return LLVMPointerType(LLVMInt8Type(), 0);
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
      return_type = cg_get_type(data_type->value.function.return_type);
    }

    LLVMTypeRef function_type = LLVMFunctionType(return_type, NULL, 0, 0);
    return function_type;
    // return LLVMPointerType(function_type, 0);
  }
  case DTK_VOID: {
    puts("cg_get_type: void");
    return LLVMVoidType();
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

  LLVMTypeRef type = cg_get_type(symbol->type);
  LLVMTypeRef ptr_type = LLVMPointerType(type, 0);

  LLVMValueRef fn_pointer =
      LLVMBuildLoad2(code_generator->builder, ptr_type, symbol->llvm_value, "");

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
    return LLVMBuildLoad2(code_generator->builder, cg_get_type(symbol->type),
                          symbol->llvm_value, "");
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

    if (expr->node.fn_def->function_type->value.function.extern_name != NULL) {
      puts("code genning extern function.");

      function = LLVMAddFunction(
          code_generator->module,
          expr->node.fn_def->function_type->value.function.extern_name->data,
          cg_get_type(expr->node.fn_def->function_type));
    } else {
      function = LLVMAddFunction(code_generator->module, "",
                                 cg_get_type(expr->node.fn_def->function_type));
      LLVMBasicBlockRef block = LLVMAppendBasicBlock(function, "entry");
      LLVMPositionBuilderAtEnd(code_generator->builder, block);

      code_generator->current_block = block;
      code_generator->current_symbol_table = &expr->node.fn_def->symbol_table;

      cg_visit_function_statements(code_generator, expr->node.fn_def->body);

      code_generator->current_symbol_table = old_current;
      code_generator->current_block = previous_block;

      LLVMPositionBuilderAtEnd(code_generator->builder, previous_block);
    }

    puts("popping function");

    return function;
  case EXPR_FN_CALL:
    return cg_visit_function_call(code_generator, expr->node.fn_call);
  }
}

void cg_visit_decl(struct CodeGenerator *code_generator,
                   struct DeclarationStatementNode *decl) {

  struct SymbolTableEntry *symbol =
      lookup_symbol_in(decl->symbol, code_generator->current_symbol_table);
  assert(symbol != NULL);
  printf("Code genning decl %s.\n", symbol->symbol.data);

  LLVMTypeRef type = cg_get_type(decl->data_type);
  if (decl->data_type->kind == DTK_FUNCTION) {
    type = LLVMPointerType(type, 0);
  }

  LLVMValueRef variable =
      LLVMBuildAlloca(code_generator->builder, type, decl->symbol.data);

  symbol->llvm_value = variable;

  LLVMValueRef result = cg_visit_expr(code_generator, decl->expression);

  LLVMBuildStore(code_generator->builder, result, variable);
}

void cg_visit_module_decl(struct CodeGenerator *code_generator,
                          struct DeclarationStatementNode *decl) {

  struct SymbolTableEntry *symbol =
      lookup_symbol_in(decl->symbol, code_generator->current_symbol_table);
  assert(symbol != NULL);
  printf("Code genning MODULE decl %s.\n", symbol->symbol.data);

  LLVMTypeRef type = cg_get_type(decl->data_type);
  if (decl->data_type->kind == DTK_FUNCTION) {
    type = LLVMPointerType(type, 0);
  }

  LLVMValueRef variable = LLVMAddGlobal(code_generator->module, type, "");
  LLVMSetInitializer(variable, LLVMConstPointerNull(type));

  symbol->llvm_value = variable;

  LLVMValueRef result = cg_visit_expr(code_generator, decl->expression);

  LLVMBuildStore(code_generator->builder, result, variable);
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

void cg_visit_function_statement(struct CodeGenerator *code_generator,
                                 struct FunctionStatementNode *stmt) {
  switch (stmt->type) {
  case FN_STMT_LET:
    cg_visit_decl(code_generator, stmt->node.decl);
    break;
  case FN_STMT_CONST:
    cg_visit_decl(code_generator, stmt->node.decl);
    break;
  case FN_STMT_ASSIGN:
    assert(lookup_symbol_in(stmt->node.assign->symbol,
                            code_generator->current_symbol_table) != NULL);
    puts("assign safe.");
    printf("assign type: %s -> %d (bin is %d)\n",
           stmt->node.assign->symbol.data, stmt->node.assign->expression->type,
           EXPR_BINARY);
    cg_visit_expr(code_generator, stmt->node.assign->expression);
    // TODO: Do we need something here?
    break;
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

    LLVMTypeRef func_type = LLVMFunctionType(LLVMVoidType(), NULL, 0, 0);
    LLVMTypeRef ptr_type = LLVMPointerType(func_type, 0);

    LLVMValueRef fn_pointer = LLVMBuildLoad2(code_generator->builder, ptr_type,
                                             main_symbol->llvm_value, "");

    if (main_symbol->type->value.function.return_type->kind == DTK_VOID) {
      LLVMBuildCall2(code_generator->builder, func_type, fn_pointer, 0, 0, "");
      LLVMBuildRet(code_generator->builder,
                   LLVMConstInt(LLVMInt32Type(), 0, 0));
    } else {
      LLVMValueRef main_ret = LLVMBuildCall2(code_generator->builder, func_type,
                                             fn_pointer, 0, 0, "");
      LLVMBuildRet(code_generator->builder, main_ret);
    }

  } else {
    LLVMBuildRetVoid(code_generator->builder);
  }

  return module_initialization_function;
}
