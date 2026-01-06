# AGENTS.md

Reference guide for AI agents working on the Luna compiler codebase.

## Project Overview

The Luna compiler is a complete toolchain for the Luna programming language. It compiles `.luna` source files to native executables using LLVM as the backend.

**Version**: 0.1.3-dev  
**Language**: C99 (ISO/IEC 9899:1999)  
**Backend**: LLVM (LLVM-C API)

### Executables
- `lunac` - Main compiler
- `luna-lsp` - Language Server Protocol server for IDE/editor support

### Build Command
```bash
make                    # Build both executables
./luna_build.sh file.luna  # Compile and run Luna programs
```

## Architecture

### Compilation Pipeline

```
.luna source code
    ↓
[Frontend]
    Lexer → Tokens
    Parser → AST (Abstract Syntax Tree)
    ↓
[Middle-end]
    Module Symbol Table Builder → Symbol tables
    Annotator → Type inference, Semantic analysis
    ↓
[Backend]
    LLVM Code Generator → LLVM IR (.ll)
    llc → Assembly (.s)
    as → Object file (.o)
    clang → Executable
```

### Key Components

| Component | Files | Purpose |
|-----------|-------|---------|
| **Lexer** | `lexer.c/h` | Tokenizes Luna source code, tracks source locations |
| **Parser** | `parser.c/h` | Recursive descent parser, constructs AST |
| **AST** | `ast.c/h` | AST node definitions and utilities |
| **Symbol Tables** | `module_symbol_table_builder.c/h` | Builds hierarchical symbol tables |
| **Annotator** | `annotator.c/h` | Type inference, semantic analysis |
| **Code Generator** | `llvm-backend/code_generator.c/h` | Converts AST to LLVM IR |
| **Arena Allocator** | `arena_allocator.c/h` | Region-based memory allocation |
| **LSP Server** | `lsp.c`, `lsp_query.c` | Language Server Protocol implementation |

## Directory Structure

```
/Users/chris/dev/luna/software/luna/
├── include/                    # Public header files
│   ├── llvm-backend/          # LLVM-specific headers
│   ├── ast.h                   # AST definitions
│   ├── lexer.h                 # Lexer interface
│   ├── parser.h                # Parser interface
│   ├── annotator.h             # Type annotation and semantic analysis
│   ├── interpreter.h           # Interpreter engine
│   ├── instructions.h          # IR instruction definitions
│   ├── token.h                 # Token definitions
│   ├── luna_string.h           # String handling
│   ├── arena_allocator.h      # Arena allocator
│   └── lsp_query.h             # LSP query support
├── src/                        # Implementation files
│   ├── llvm-backend/          # LLVM code generation
│   ├── ast.c
│   ├── lexer.c
│   ├── parser.c
│   ├── annotator.c
│   ├── interpreter.c
│   ├── instruction_builder.c   # IR instruction builder
│   ├── module_symbol_table_builder.c
│   ├── luna_string.c
│   ├── arena_allocator.c
│   └── lsp_query.c
├── examples/                   # Example Luna programs
├── bin/                        # Compiled binaries
├── out/                        # Generated files (.ll, .s, executables)
├── main.c                      # Compiler entry point (debug version)
├── lsp.c                       # LSP server entry point
├── lib.c                       # Runtime library
├── Makefile                    # Build configuration
└── luna_build.sh              # Build and run script
```

## Coding Style & Conventions

### Language and Compiler
- **C99 standard** (`-std=c99`)
- **Strict compilation flags**: `-Wall -Wextra -Wpedantic -Werror`
- **No warnings allowed** - code must compile cleanly

### Naming Conventions

**Types/Structs**:
```c
struct ExpressionNode;          // PascalCase with "Node" suffix
struct BinaryExpressionNode;    // PascalCase with "Node" suffix
struct ArenaAllocator;          // PascalCase (no suffix for non-AST)
struct DataType;                // PascalCase
```

**Functions**:
```c
ast_make_integer_literal();     // snake_case
parse_expression();             // snake_case
lexer_next();                   // snake_case
annotator_visit_module_statements();  // module_prefix_verb_noun
```

**Enums**:
```c
enum ExpressionType {            // PascalCase with "Type" suffix
  EXPR_BINARY,                  // EXPR_ prefix for expression types
  EXPR_INTEGER_LITERAL,
};

enum TokenType {                // PascalCase with "Type" suffix
  T_LPAREN,                     // T_ prefix for tokens
  T_RPAREN,
};
```

**Variables**:
```c
struct ArenaAllocator *allocator;  // snake_case
struct ExpressionNode *expr;      // snake_case, abbreviations ok
struct Token *toks;                // abbreviations ok (tokens -> toks)
```

### Code Style

- **No comments** in production code (minimal inline comments)
- **Brace style**: Allman style (opening brace on new line)
- **Indentation**: 2 spaces
- **No trailing whitespace**
- **Line length**: No strict limit, but keep reasonable (< 120 chars)

```c
struct ExpressionNode *
ast_make_integer_literal(struct ArenaAllocator *allocator, uint16_t value)
{
  return ast_promote(allocator, &(struct IntegerLiteralNode){.value = value},
                     sizeof(struct IntegerLiteralNode));
}
```

### Memory Management

**CRITICAL**: All allocation uses the arena allocator. **Never call free()**.

```c
// Correct - allocate from arena
struct ExpressionNode *node = ast_promote(allocator, &temp_struct, sizeof(struct ExpressionNode));
void *data = arena_alloc(allocator, size);

// WRONG - never do this
void *data = malloc(size);
free(data);
```

**Arena allocation pattern**:
```c
// Create temporary struct on stack
struct DataType temp_type = {
  .kind = DTK_PRIMITIVE,
  .value.primitive = {P_INT, 32, true}
};

// Promote to arena memory
struct DataType *type = ast_promote(allocator, &temp_type, sizeof(struct DataType));
```

### AST Structure Pattern

All AST nodes use a consistent pattern:
```c
struct ExpressionNode {
  enum ExpressionType type;              // Discriminator
  union {
    struct IntegerLiteralNode *integer;
    struct BinaryExpressionNode *binary;
    struct FunctionDefinitionExpressionNode *fn_def;
    // ... more variants
  } node;
  struct SourceLocation location;        // Always present
};
```

**Type checking pattern**:
```c
if (expr->type == EXPR_BINARY) {
  struct BinaryExpressionNode *bin = expr->node.binary;
  // Access bin->left, bin->right, bin->type
}
```

### Linked Lists

Extensive use of linked lists for AST nodes:
```c
struct ModuleStatementNode *stmt = statements;
while (stmt) {
  // Process stmt
  stmt = stmt->next;
}
```

**Building linked lists**:
```c
struct FunctionStatementNode *current = NULL;
struct FunctionStatementNode *root = NULL;

while (has_more_statements()) {
  struct FunctionStatementNode *new_stmt = parse_statement();
  
  if (root == NULL) {
    root = new_stmt;
    current = root;
  } else {
    current->next = new_stmt;
    current = new_stmt;
  }
}
```

## Key Data Structures

### SourceLocation
All AST nodes track source position:
```c
struct SourceLocation {
  struct LunaString filename;
  uint16_t start;              // Byte offset
  uint16_t end;                // Byte offset
};
```

### DataType (Type System)
Union-based type representation:
```c
enum DataTypeKind {
  DTK_PRIMITIVE,       // int, float
  DTK_FUNCTION,        // Function types
  DTK_VOID,            // void
  DTK_POINTER,         // *T
  DTK_ARRAY,           // [N]T
  DTK_STRUCTURE,       // Struct instances
  DTK_STRUCTURE_DEF,   // Struct definitions
  DTK_MODULE_DEF,      // Module definitions
  DTK_MODULE,          // Module instances
  DTK_RESOLVABLE,      // Forward-declared types
};

struct DataType {
  enum DataTypeKind kind;
  union {
    struct FunctionType function;
    struct PrimitiveType primitive;
    struct DataType *pointer_inner;
    struct StructType structure;
    struct ArrayType array;
    // ...
  } value;
  struct DataType *next;      // Linked list in type table
};
```

### SymbolTable
Hierarchical symbol tables for scoping:
```c
enum SymbolTableType {
  STT_MOD,    // Module level
  STT_FN,     // Function level
  STT_SCOPE,  // Control flow (if, while)
  STT_STRUCT, // Struct scope
};

struct SymbolTable {
  struct SymbolTableEntry *head;
  struct SymbolTable *parent;     // Parent scope
  uint16_t current_index;         // Stack frame index
  enum SymbolTableType type;
};

struct SymbolTableEntry {
  struct LunaString symbol;
  struct DataType *type;
  LLVMValueRef llvm_value;        // For code generation
  struct SymbolTableEntry *next;
  enum MemorySegment memory_segment;
  enum SymbolLocation symbol_location;
  uint16_t index;
};
```

### LunaString
Custom string type (pointer + length):
```c
struct LunaString {
  char *data;
  uint16_t length;
};

struct LunaString string_make(const char *str);
```

## The Luna Language

### Basic Syntax

```luna
// Module-level declarations
const puts = fn@extern["puts"](str: *i8)

// Function definition
const sum = fn(x: i32, y: i32): i32 {
    return x + y
}

// Main entry point
const main = fn(): i32 {
    let a: i32 = 10
    const b: i32 = 20
    const result = sum(a, b)
    return result
}
```

### Language Features

**Variables**:
- `let` - mutable
- `const` - immutable
- Type annotation optional: `let x: i32 = 5` vs `let x = 5`

**Functions**:
```luna
const fn_name = fn(param: type): return_type {
    // body
}

// Extern C functions
const puts = fn@extern["puts"](str: *i8)
const printf = fn@extern["printf"](fmt: *i8, ...): i32
```

**Structs**:
```luna
const Point = struct {
    x: i32,
    y: i32
}

let p: Point = .Point { x: 10, y: 20 }
```

**Pointers**:
```luna
let a: i32 = 10
let ptr: *i32 = &a
let val: i32 = *ptr
```

**Arrays**:
```luna
let arr = [1, 2, 3, 4, 5]
let first: i32 = arr[0]
```

**Control Flow**:
```luna
if condition {
    // body
} else {
    // else body
}

while condition {
    // body
}
```

**Built-in Operations**:
- `@value(expr)` - Get size in bytes
- `@type(T)` - Get type size in bytes

## Build System

### Makefile Structure

The Makefile uses `llvm-config` to detect LLVM:

```makefile
LLVM_CONFIG ?= llvm-config

LLVM_CFLAGS := $(shell $(LLVM_CONFIG) --cflags)
LLVM_LIBS   := $(shell $(LLVM_CONFIG) --libs core bitreader bitwriter \
                                   irreader analysis scalaropts support)
LLVM_LDFLAGS := $(shell $(LLVM_CONFIG) --ldflags) -Wl,-rpath,$(shell $(LLVM_CONFIG) --libdir)

CFLAGS := -Wall -Wextra -Wpedantic -Werror -std=c99 -Iinclude $(LLVM_CFLAGS)
LDFLAGS := $(LLVM_LDFLAGS) $(LLVM_LIBS)
```

### Building

```bash
make                    # Build lunac and luna-lsp
make clean              # Remove binaries
./luna_build.sh file.luna  # Compile and run
```

### LLVM Integration

**Required LLVM libraries**:
- `core` - Core LLVM infrastructure
- `bitreader` - Read LLVM bitcode
- `bitwriter` - Write LLVM bitcode
- `irreader` - Parse LLVM IR text
- `analysis` - Analysis passes
- `scalaropts` - Scalar optimization passes

**LLVM-C API usage**:
- All code generation uses LLVM-C API (not C++ API)
- Types: `LLVMTypeRef`, `LLVMValueRef`, `LLVMModuleRef`, etc.
- Pattern: `LLVMBuild*()` functions for building IR

## Common Tasks

### Adding a New AST Node Type

1. **Update `ast.h`**:
   ```c
   enum ExpressionType {
     // ... existing types
     EXPR_NEW_TYPE,
   };

   struct ExpressionNode {
     // ... existing fields
     union {
       // ... existing fields
       struct NewTypeNode *new_type;
     } node;
   };

   struct NewTypeNode {
     // node-specific fields
   };
   ```

2. **Update parser (`parser.c`)**:
   ```c
   struct NewTypeNode *parse_new_type(struct Parser *parser) {
     // Parse logic
     // Use ast_promote() for allocation
   }
   ```

3. **Update annotator (`annotator.c`)**:
   ```c
   struct DataType *infer_type(struct Annotator *annotator,
                                struct ExpressionNode *expr) {
     // ... existing cases
     case EXPR_NEW_TYPE:
       // Type inference logic
       break;
   }
   ```

4. **Update code generator (`llvm-backend/code_generator.c`)**:
   ```c
   LLVMValueRef cg_visit_expr(struct CodeGenerator *cg,
                              struct ExpressionNode *expr) {
     // ... existing cases
     case EXPR_NEW_TYPE:
       // Code generation logic
       break;
   }
   ```

### Adding a New Token

1. **Update `token.h`**:
   ```c
   enum TokenType {
     // ... existing tokens
     T_NEW_TOKEN,
   };
   ```

2. **Update lexer (`lexer.c`)**:
   ```c
   bool lexer_next(struct Lexer *lexer, struct Token *out_token) {
     // ... existing logic
     // Add pattern matching for new token
   }
   ```

3. **Update parser** to handle the new token

### Working with Symbol Tables

**Lookup symbol**:
```c
struct SymbolTableEntry *entry = lookup_symbol(annotator, symbol_name);
if (entry) {
  struct DataType *type = entry->type;
}
```

**Insert symbol**:
```c
struct SymbolTableEntry entry = {
  .symbol = symbol_name,
  .type = type,
  .llvm_value = llvm_value,
  .symbol_location = SL_LOCAL,
  .index = current_index,
};
insert_symbol_entry(annotator, entry);
```

### Type Creation Helpers

```c
// Primitive type
struct DataType *i32_type = make_integer_primitive_data_type(allocator, 32, true);

// Pointer type
struct DataType *i32_ptr = make_pointer_data_type(allocator, i32_type);

// Function type
struct DataType *fn_type = make_function_data_type(
  allocator,
  arguments,           // FunctionArgumentNode*
  return_type,         // DataType*
  &extern_name,        // LunaString* or NULL
  false                // is_variadic
);

// Array type
struct DataType *arr_type = make_array_data_type(
  allocator,
  element_type,        // DataType*
  length               // uint64_t
);
```

## Testing and Debugging

### AST Visualization

The compiler includes AST printing functions in `main.c`:
```c
print_module_statement_list(root_ast, 0);  // Print entire AST
```

Output format uses tree visualization:
```
ModuleStatementNode (MOD_STMT_CONST) [file=file.luna, span=0-100]
  symbol: main
  expression: ExpressionNode (EXPR_FN_DEF) [file=file.luna, span=10-100]
```

### Cursor Location Query

Test AST navigation with cursor queries:
```bash
./bin/lunac --cursor line:col examples/file.luna
```

This will print the smallest AST node containing the cursor position.

### Manual Testing

```bash
# Compile a Luna file
./luna_build.sh examples/hello_world.luna

# Check generated LLVM IR
cat out/compiled.ll

# Check generated assembly
cat out/compiled.s

# Run the executable
./out/compiled
```

### No Formal Test Framework

The project does not have automated tests. Verification is done by:
- Running example programs in `examples/`
- Inspecting generated LLVM IR
- Manual testing of new features

## Important Patterns and Gotchas

### AST Node Promotion Pattern

Always use `ast_promote()` for AST nodes:
```c
struct ExpressionNode temp_expr = {
  .type = EXPR_INTEGER_LITERAL,
  .node.integer = &(struct IntegerLiteralNode){.value = 42},
  .location = source_location,
};

struct ExpressionNode *expr = ast_promote_expression_node(allocator, temp_expr);
```

### Source Location Tracking

All AST nodes must have valid source locations:
```c
struct SourceLocation loc = parser_make_span(parser, start_position);

struct ExpressionNode *node = ast_make_binary_expression(
  allocator, BIN_EXPR_ADD, left, right);
node->location = loc;
```

### Type Resolution Order

1. Build module symbol tables (module → function → scope)
2. Infer forward-declared types
3. Visit and annotate all nodes

### Memory Segment Assignment

Symbols are assigned to memory segments:
- `LOCAL` - Local variables (stack)
- `CONST` - Constants (read-only data)
- `STATIC` - Static/global data

### LLVM Value Creation

Pattern for creating LLVM values:
```c
// Create function
LLVMValueRef fn = LLVMAddFunction(module, "main", fn_type);

// Create basic block
LLVMBasicBlockRef entry = LLVMAppendBasicBlock(fn, "entry");

// Create builder
LLVMBuilderRef builder = LLVMCreateBuilder();
LLVMPositionBuilderAtEnd(builder, entry);

// Build instructions
LLVMValueRef result = LLVMBuildAdd(builder, left, right, "result");

// Clean up
LLVMDisposeBuilder(builder);
```

## Development Workflow

1. **Make changes** to source files
2. **Run `make clean && make`** to rebuild
3. **Test with example files** in `examples/`
4. **Inspect generated LLVM IR** in `out/compiled.ll`
5. **Verify code is clean** (no warnings, compiles with `-Werror`)

## Version Control

**Current version**: 0.1.3-dev

Version is defined in `main.c`:
```c
const char *VERSION = "0.1.3-dev";
```

When committing changes:
- Ensure code compiles cleanly
- Test with example programs
- Update version if making breaking changes

## LSP Server (luna-lsp)

The LSP server implements the Language Server Protocol for IDE/editor support:
- Text document synchronization
- Hover information (type/definition at cursor)
- AST queries based on cursor position
- JSON communication over stdin/stdout

Entry point: `lsp.c`  
Query implementation: `lsp_query.c`

Build separately:
```bash
make luna-lsp
```

## Notes

- The compiler is currently in development (0.1.3-dev)
- Some features may be incomplete or experimental
- Error handling is minimal (assert-based)
- No comprehensive test suite exists
- Arena allocator means no individual deallocation
- All AST nodes track source locations for error reporting

## Additional Resources

**Example programs**: See `examples/` directory for language feature demonstrations  
**LLVM documentation**: https://llvm.org/docs/  
**C99 standard**: ISO/IEC 9899:1999
