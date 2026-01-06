# AGENTS.md - Guide for AI Assistants Working on Luna

This document provides guidance for AI assistants working on the Luna compiler codebase.

## Project Overview

Luna is a compiler for a custom programming language that targets LLVM. The project is written in C99 and consists of several phases:

1. **Lexing** - Tokenizes source code into lexical tokens
2. **Parsing** - Builds an Abstract Syntax Tree (AST) from tokens
3. **Symbol Resolution** - Builds module symbol tables
4. **Type Inference** - Annotates expressions with types
5. **Code Generation** - Generates LLVM IR from the typed AST
6. **Execution** - Can interpret or compile the generated code

## Build System

**Build Command:**
```bash
make
```

**Compilation Flags:**
```make
CFLAGS := -Wall -Wextra -Wpedantic -Werror -std=c99 -Iinclude $(LLVM_CFLAGS)
```

**Key Requirements:**
- C99 compliant compiler (GCC or Clang recommended)
- LLVM development libraries
- All warnings treated as errors (`-Werror`)

**Output:** Compiled binary at `bin/lunac`

## Coding Conventions

### Struct Naming
- Use `CamelCase` for all struct names
- Do NOT use `typedef` for structs
- Always use the `struct` keyword: `struct ArenaAllocator`, `struct JsonValue`

```c
struct ArenaAllocator {
    void *arena;
    size_t length;
    size_t capacity;
};
```

### Enum Naming
- Use `ALL_CAPS_WITH_UNDERSCORES` for enum names
- Prefix with relevant scope: `enum TokenType`, `enum ExpressionType`

```c
enum TokenType {
    TOKEN_EOF,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_STRING,
    // ...
};
```

### Function Naming
- Use `snake_case` for all function names
- Use descriptive names that indicate action
- Constructor-style functions: `module_make()`, `lexer_make()`
- Predicate functions: `strings_equal()`, `lexer_next()`

```c
struct ArenaAllocator arena_make(void *arena, size_t capacity);
void *arena_alloc(struct ArenaAllocator *allocator, size_t size);
struct ExpressionNode *ast_make_integer_literal(struct ArenaAllocator *allocator, int64_t value);
```

### Compound Literals and Designated Initializers

Use C99 compound literals for struct creation and designated initializers:

```c
return (struct ArenaAllocator){
    .arena = arena,
    .capacity = capacity,
    .length = 0,
};

return (struct ExpressionNode){
    .type = EXPR_INTEGER_LITERAL,
    .node.integer_value = value,
};
```

### Header Guards

Standard preprocessor header guards:

```c
#ifndef ARENA_ALLOCATOR_H
#define ARENA_ALLOCATOR_H
// ... content ...
#endif
```

### Include Order

Organize includes as follows:
1. Standard library headers (alphabetically)
2. Project headers (alphabetically)
3. LLVM headers with `llvm-c/` prefix

```c
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "arena_allocator.h"
#include "lexer.h"
#include "parser.h"

#include "llvm-c/Core.h"
```

### Integer Types

Always use `<stdint.h>` types:
- `uint8_t`, `uint16_t`, `uint32_t`, `uint64_t` for unsigned
- `int8_t`, `int16_t`, `int32_t`, `int64_t` for signed
- `size_t` for sizes and counts

### Error Handling

Use `assert()` for unrecoverable errors (programming errors, invariant violations):

```c
assert(allocator != NULL);
assert(length < capacity);
```

For recoverable errors (user input, parsing), return error codes and populate error structs.

## Memory Management

### Arena Allocator Pattern

The codebase uses a bump allocator pattern. All allocations flow through `arena_alloc()`:

```c
struct ArenaAllocator arena_make(void *arena, size_t capacity);
void *arena_alloc(struct ArenaAllocator *allocator, size_t size);
```

**Usage Pattern:**
```c
uint8_t arena_buffer[UINT16_MAX * 16];
struct ArenaAllocator allocator = arena_make(arena_buffer, sizeof(arena_buffer));

struct JsonArray *array = arena_alloc(allocator, sizeof(struct JsonArray));
```

**Important:** No `malloc()`, `free()`, or `calloc()` should be used. All memory comes from the arena.

**Consequences:**
- No memory leaks (arena is stack-allocated)
- Single allocation point simplifies debugging
- All allocations freed when function returns

## Common Patterns

### Linked Lists in AST

AST nodes commonly use `next` pointers for sibling lists:

```c
struct ModuleStatementNode {
    enum ModuleStatementType type;
    union { /* ... */ } node;
    struct ModuleStatementNode *next;
};
```

### Variant Types with Unions

Use unions inside structs for variant types:

```c
struct ExpressionNode {
    enum ExpressionType type;
    union {
        struct IntegerLiteralNode *integer;
        struct StringLiteralNode *string;
        struct BinaryOpNode *binary_op;
        struct FunctionCallNode *function_call;
        // ...
    } node;
};
```

Access with:
```c
if (expr->type == EXPR_BINARY_OP) {
    struct BinaryOpNode *op = expr->node.binary_op;
    // ...
}
```

### Component Initialization Pattern

Each major component (lexer, parser, etc.) follows this pattern:

```c
// Header
struct Lexer {
    char *source;
    size_t source_length;
    size_t cursor;
    struct Token current_token;
};

struct Lexer lexer_make(char *source, size_t source_length);

// Implementation
struct Lexer lexer_make(char *source, size_t source_length) {
    return (struct Lexer){
        .source = source,
        .source_length = source_length,
        .cursor = 0,
    };
}
```

## Project Structure

```
/home/chris/dev/luna/software/luna/
├── include/              # Public API headers
│   ├── arena_allocator.h # Memory management
│   ├── ast.h            # AST node definitions
│   ├── lexer.h          # Lexer interface
│   ├── parser.h         # Parser interface
│   ├── token.h          # Token type definitions
│   ├── json.h           # JSON parsing library
│   └── llvm-backend/
│       └── code_generator.h
├── src/                 # Implementation
│   ├── arena_allocator.c
│   ├── ast.c
│   ├── lexer.c
│   ├── parser.c
│   ├── json.c           # JSON library implementation
│   └── llvm-backend/
│       └── code_generator.c
├── main.c              # Compiler driver
├── lib.c               # Library entry
├── json_main.c         # JSON library tests
├── Makefile            # Build configuration
└── bin/                # Compiled binaries
```

## Adding New Features

### Adding a New JSON Test

1. Create test in `json_main.c`
2. Use assert-based testing
3. Follow existing test patterns
4. Run: `clang -std=c99 -Iinclude -o json_test json_main.c src/json.c src/arena_allocator.c && ./json_test`

### Adding a New Compiler Phase

1. Create header in `include/` with struct definition
2. Create implementation in `src/`
3. Update `Makefile` to include new source file
4. Update `main.c` to call new phase

Example:
```bash
# Create files
touch include/my_phase.h src/my_phase.c

# Update Makefile SRC variable
SRC := main.c \
       src/luna_string.c src/lexer.c src/parser.c src/ast.c \
       src/arena_allocator.c src/interpreter.c src/annotator.c \
       src/instruction_builder.c src/llvm-backend/code_generator.c \
       src/module_symbol_table_builder.c \
       src/my_phase.c  # Add this line
```

### Extending the AST

When adding new node types:

1. Add enum value to appropriate `enum` in header
2. Add union member to struct in header
3. Implement creation/handling functions in implementation file
4. Update parser to construct new nodes
5. Update code generator to emit code for new nodes

## Testing Approach

### Running Tests

**JSON Library:**
```bash
clang -std=c99 -Iinclude -o json_test json_main.c src/json.c src/arena_allocator.c && ./json_test
```

**Full Compiler:**
```bash
make clean && make
```

### Adding Tests

Tests should:
- Use `assert()` for verification
- Have descriptive names: `test_feature_description()`
- Print "TEST_NAME: PASSED" on success
- Return non-zero exit code on failure

## Common Tasks

### Finding Where Something is Defined

**Search for struct definitions:**
```bash
grep -n "struct.* {" include/*.h
```

**Search for function implementations:**
```bash
grep -n "^function_name(" src/*.c
```

**Search for enum values:**
```bash
grep -n "TOKEN_" include/token.h
```

### Understanding a Module

1. Read the header first (public API)
2. Read the implementation (internal details)
3. Look for initialization functions (`*_make`)
4. Look for main processing functions
5. Check main.c for usage pattern

### Debugging Memory Issues

Since all allocation is through arena:
- Check arena capacity and length
- Verify all allocations fit in arena
- Ensure no pointer arithmetic exceeds bounds
- Use `assert(length < capacity)` patterns

## LLVM Integration

### Including LLVM Headers

Use `llvm-c/` prefix:
```c
#include "llvm-c/Core.h"
#include "llvm-c/IRReader.h"
#include "llvm-c/Target.h"
```

### LLVM Flags (from Makefile)

Typical flags:
```make
LLVM_CFLAGS := $(shell llvm-config --cflags)
LLVM_LDFLAGS := $(shell llvm-config --ldflags)
LLVM_LIBS := $(shell llvm-config --libs core)
```

### Code Generation Pattern

```c
LLVMValueRef codegen_expression(struct ArenaAllocator *allocator,
                                 struct LLVMBuilderRef builder,
                                 struct ExpressionNode *expr) {
    switch (expr->type) {
        case EXPR_INTEGER_LITERAL:
            return LLVMConstInt(LLVMInt64Type(),
                               expr->node.integer_value->value,
                               false);
        // ... other cases ...
    }
}
```

## Style Checklist

Before submitting code:

- [ ] Struct names use CamelCase
- [ ] Enum names use ALL_CAPS
- [ ] Function names use snake_case
- [ ] No typedef for structs
- [ ] Compound literals used for struct creation
- [ ] Designated initializers used where appropriate
- [ ] Standard includes come before project includes
- [ ] No `malloc()`/`free()` - use arena allocator
- [ ] `-Wall -Wextra -Wpedantic -Werror` clean
- [ ] Comments explain "why", not "what"

## Getting Help

- See `README.md` for project overview
- Check existing implementations for patterns
- Use grep to find similar functionality
- Review main.c to understand workflow
