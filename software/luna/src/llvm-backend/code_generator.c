#include "llvm-backend/code_generator.h"
#include "annotator.h"
#include "arena_allocator.h"
#include "ast.h"
#include "luna_string.h"
#include "llvm-c/Core.h"
#include "llvm-c/Types.h"
#include <assert.h>
#include <stdio.h>

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
  default:
    puts("Unknown data type kind.");
    assert(0);
    return NULL;
  }
}

LLVMValueRef cg_visit_function_call(struct CodeGenerator *code_generator,
                                    struct FunctionCallExpressionNode *expr) {
  puts("\n\t\t\t\t\t\tCode genning function call.");
  struct SymbolTableEntry *symbol =
      lookup_symbol_in(expr->name, code_generator->current_symbol_table);
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
    case BIN_EXPR_LT:
      puts("got lt");
      return LLVMBuildICmp(code_generator->builder, LLVMIntSLT, left, right,
                           "");
    case BIN_EXPR_GT:
      puts("got gt");
      return LLVMBuildICmp(code_generator->builder, LLVMIntSGT, left, right,
                           "");
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

    LLVMTypeRef type = cg_get_type(code_generator, symbol->type);
    // if (symbol->type->kind == DTK_FUNCTION) {
    //   type = LLVMPointerType(type, 0);
    // }

    switch (symbol->symbol_location) {
    case SL_MODULE:
      puts("its a module symbol");
      assert(0);
      break;
    case SL_ARGUMENT:
      puts("its an argument symbol");
      return symbol->llvm_value;
    case SL_LOCAL:
      puts("its local so build a load2 once.");
      if (symbol->type->kind == DTK_FUNCTION) {
        return symbol->llvm_value;
      }
      return LLVMBuildLoad2(code_generator->builder, type, symbol->llvm_value,
                            "");
    };

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
        symbol->llvm_value = value;
        index++;
        argument = argument->next;
      }

      cg_visit_function_statements(code_generator, expr->node.fn_def->body);

      code_generator->current_symbol_table = old_current;
      code_generator->current_block = previous_block;

      LLVMPositionBuilderAtEnd(code_generator->builder, previous_block);
    }

    printf("\n\t\t\t++++++++++++++++POPPING FUNCTION+++++++++\n\n\n");

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

    if (LLVMGetTypeKind(type) == LLVMPointerTypeKind) {
      puts("type was pointer type so adding extra load.");
      value = LLVMBuildLoad2(code_generator->builder, type, inner, "");
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

  if (source_kind == LLVMIntegerTypeKind && dest_kind == LLVMIntegerTypeKind) {
    puts("doing integer to integer coercsion.");
    uint16_t source_width = LLVMGetIntTypeWidth(source_type);
    uint16_t dest_width = LLVMGetIntTypeWidth(dest_type);
    if (source_width < dest_width) {
      puts("need to extend source");
      return LLVMBuildSExt(cg->builder, val, dest_type, "");
    } else if (source_width > dest_width) {
      puts("need to trunc source");
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

void cg_visit_module_decl(struct CodeGenerator *code_generator,
                          struct DeclarationStatementNode *decl) {
  struct SymbolTableEntry *symbol =
      lookup_symbol_in(decl->symbol, code_generator->current_symbol_table);
  assert(symbol != NULL);

  printf("Code genning MODULE decl %s.\n", symbol->symbol.data);

  LLVMTypeRef type = cg_get_type(code_generator, decl->data_type);
  // if (decl->data_type->kind == DTK_FUNCTION) {
  //   type = LLVMPointerType(type, 0);
  // }

  LLVMValueRef variable = LLVMAddGlobal(code_generator->module, type, "");
  if (decl->data_type->kind == DTK_FUNCTION ||
      decl->data_type->kind == DTK_POINTER) {
    LLVMSetInitializer(variable, LLVMConstPointerNull(type));
  } else {
    LLVMSetInitializer(variable, LLVMConstInt(LLVMInt32Type(), 0, 0));
  }

  symbol->llvm_value = variable;

  cg_gen_assignment(code_generator, variable, type, decl->expression);
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
                   LLVMBasicBlockRef prev_merge_block) {
  {

    LLVMValueRef fn = LLVMGetBasicBlockParent(code_generator->current_block);
    LLVMBasicBlockRef then_block = LLVMAppendBasicBlock(fn, "if.then");
    LLVMBasicBlockRef merge_block = prev_merge_block != NULL
                                        ? prev_merge_block
                                        : LLVMAppendBasicBlock(fn, "if.end");
    LLVMBuildBr(code_generator->builder, then_block);

    LLVMPositionBuilderAtEnd(code_generator->builder, then_block);
    struct SymbolTable *old_current = code_generator->current_symbol_table;
    code_generator->current_symbol_table = &if_stmt->symbol_table;

    LLVMBasicBlockRef old_block = code_generator->current_block;
    code_generator->current_block = then_block;

    cg_visit_function_statements(code_generator, if_stmt->body);

    code_generator->current_symbol_table = old_current;
    code_generator->current_block = old_block;

    if (LLVMGetBasicBlockTerminator(then_block) == NULL) {
      LLVMBuildBr(code_generator->builder, merge_block);
    }

    LLVMPositionBuilderAtEnd(code_generator->builder, merge_block);
  }
}

void cg_visit_if(struct CodeGenerator *code_generator,
                 struct IfStatementNode *if_stmt,
                 LLVMBasicBlockRef prev_merge_block) {
  LLVMValueRef conditional = cg_visit_expr(code_generator, if_stmt->condition);
  conditional = cg_coerce(code_generator, conditional, LLVMInt1Type());

  LLVMValueRef fn = LLVMGetBasicBlockParent(code_generator->current_block);
  LLVMBasicBlockRef then_block = LLVMAppendBasicBlock(fn, "if.then");
  LLVMBasicBlockRef else_block = LLVMAppendBasicBlock(fn, "if.else");
  LLVMBasicBlockRef merge_block = prev_merge_block != NULL
                                      ? prev_merge_block
                                      : LLVMAppendBasicBlock(fn, "if.end");

  LLVMBuildCondBr(code_generator->builder, conditional, then_block, else_block);

  LLVMPositionBuilderAtEnd(code_generator->builder, then_block);
  // TODO: visit if body
  struct SymbolTable *old_current = code_generator->current_symbol_table;
  code_generator->current_symbol_table = &if_stmt->symbol_table;

  LLVMBasicBlockRef old_block = code_generator->current_block;
  code_generator->current_block = then_block;

  cg_visit_function_statements(code_generator, if_stmt->body);

  if (LLVMGetBasicBlockTerminator(code_generator->current_block) == NULL) {
    LLVMBuildBr(code_generator->builder, merge_block);
  }

  code_generator->current_symbol_table = old_current;
  code_generator->current_block = old_block;

  if (LLVMGetBasicBlockTerminator(then_block) == NULL) {
    LLVMBuildBr(code_generator->builder, merge_block);
  }

  if (if_stmt->next != NULL) {
    LLVMPositionBuilderAtEnd(code_generator->builder, else_block);
    code_generator->current_block = else_block;

    LLVMBasicBlockRef old_block = code_generator->current_block;
    code_generator->current_block = else_block;

    if (if_stmt->next->condition == NULL) {
      cg_visit_else(code_generator, if_stmt->next, merge_block);
    } else {

      cg_visit_if(code_generator, if_stmt->next, merge_block);
    }

    code_generator->current_block = old_block;

    if (LLVMGetBasicBlockTerminator(else_block) == NULL) {
      LLVMBuildBr(code_generator->builder, merge_block);
    }
  } else {
    LLVMPositionBuilderAtEnd(code_generator->builder, else_block);
    code_generator->current_block = else_block;
    LLVMBuildBr(code_generator->builder, merge_block);
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
          lookup_symbol_in(node->source_expression->node.symbol->value,
                           code_generator->current_symbol_table);

      LLVMTypeRef type = cg_get_type(code_generator, symbol->type);

      // TODO: standardize this
      // if (symbol->type->kind == DTK_FUNCTION) {
      //   type = LLVMPointerType(type, 0);
      // }

      cg_gen_assignment(code_generator, symbol->llvm_value, type,
                        node->result_expression);
      break;

      // cg_gen_assignment(code_generator,
      //                   cg_visit_expr(code_generator,
      //                   node->source_expression), cg_get_type(code_generator,
      //                   source_type), node->result_expression);
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
  case FN_STMT_IF:
    puts("generating if..");

    cg_visit_if(code_generator, stmt->node.if_stmt, NULL);

    // assert(0);
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
