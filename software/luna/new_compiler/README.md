# Luna Bootstrap Compiler

A self-hosting compiler for the Luna programming language, written in Luna itself.

## Architecture

The compiler consists of several modules:

1. **lexer.luna** - Tokenizes Luna source code into tokens
2. **ast.luna** - Defines the Abstract Syntax Tree structures
3. **parser.luna** - Parses tokens into an AST using recursive descent
4. **typechecker.luna** - Performs static type checking and symbol resolution
5. **codegen.luna** - Generates C code from the typed AST
6. **compiler.luna** - Main driver that orchestrates the compilation pipeline

## Features Implemented

### Core Language Features
- ✅ Basic tokens (identifiers, literals, operators, keywords)
- ✅ Function definitions with parameters and return types
- ✅ Variable declarations (`let` and `const`)
- ✅ Basic expressions (arithmetic, comparison, function calls)
- ✅ Control flow (`if/else`, `while`, `return`)
- ✅ Type annotations for variables and functions
- ✅ Pointer types (`*Type`)
- ✅ Basic built-in types (`i32`, `u32`, `i1`, `u16`)

### Compiler Pipeline
- ✅ Lexical analysis with comprehensive token support
- ✅ Recursive descent parser with precedence handling
- ✅ Type checker with symbol table and scope management
- ✅ Code generator that outputs readable C code
- ✅ Main compiler driver with error handling

## Current Limitations

This is a bootstrap compiler with simplified implementations:

1. **Memory Management**: Uses basic stack allocation, no dynamic arrays
2. **Type System**: Basic types only, no full struct/enum/union support yet
3. **Generics**: Framework exists but not fully implemented
4. **Modules**: Import system not fully implemented
5. **Error Handling**: Basic error reporting, needs improvement
6. **Standard Library**: No built-in functions beyond primitives

## Usage

The compiler is designed to be self-hosting. Once a working Luna implementation exists, this compiler can compile itself and other Luna programs.

### Test Programs

- `test_simple.luna` - Basic arithmetic and function calls
- `test_fibonacci.luna` - Recursive function example

### Expected Output

The compiler generates C code that can be compiled with GCC/Clang:

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// Generated function definitions
int32_t add(int32_t a, int32_t b) {
    return a + b;
}

int main(void) {
    int32_t x = 5;
    int32_t y = 3;
    int32_t result = add(x, y);
    return result;
}
```

## Bootstrap Process

1. **Phase 1**: Implement Luna compiler in C (existing codebase)
2. **Phase 2**: Write Luna compiler in Luna (this implementation)
3. **Phase 3**: Use Phase 1 to compile Phase 2
4. **Phase 4**: Use Phase 2 to compile itself (self-hosting achieved)

## Future Enhancements

- Complete struct/enum/union support
- Full generic type system
- Module system with imports
- Memory management and garbage collection
- Standard library implementation
- Optimization passes
- Better error messages with source locations
- LLVM backend for better code generation

This bootstrap compiler demonstrates the core concepts and provides a foundation for a fully-featured Luna compiler implementation.