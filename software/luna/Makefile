# Detect LLVM flags via llvm-config
LLVM_CONFIG ?= llvm-config

LLVM_CFLAGS := $(shell $(LLVM_CONFIG) --cflags)
# Core libs sufficient for llvm-c usage; add more if you call additional APIs
LLVM_LIBS   := $(shell $(LLVM_CONFIG) --libs core bitreader bitwriter irreader \
                                 analysis scalaropts \
                                 support)
LLVM_LDFLAGS := $(shell $(LLVM_CONFIG) --ldflags) -Wl,-rpath,$(shell $(LLVM_CONFIG) --libdir)

CFLAGS := -Wall -Wextra -Wpedantic -Werror -std=c99 -Iinclude $(LLVM_CFLAGS)
LDFLAGS := $(LLVM_LDFLAGS) $(LLVM_LIBS)

SRC := main.c \
       src/luna_string.c src/lexer.c src/parser.c src/ast.c \
       src/arena_allocator.c src/interpreter.c src/annotator.c \
       src/instruction_builder.c src/llvm-backend/code_generator.c src/module_symbol_table_builder.c

LIB := lib.c \
       src/luna_string.c src/lexer.c src/parser.c src/ast.c \
       src/arena_allocator.c src/interpreter.c src/annotator.c \
       src/instruction_builder.c src/llvm-backend/code_generator.c src/module_symbol_table_builder.c

.PHONY: all clean

all: lunac

lunac: $(SRC)
	gcc -o bin/$@ $(SRC) $(CFLAGS) $(LDFLAGS)

lib: $(LIB)
	clang -S -emit-llvm -std=c99 lib.c -o out/source.ll -Iinclude
	llvm-link out/compiled.ll out/source.ll -S -o out/combined.ll
	clang out/combined.ll -o out/final $(LDFLAGS)

clean:
	rm -f lunac
