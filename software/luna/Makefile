# Detect LLVM flags via llvm-config
LLVM_CONFIG ?= llvm-config

LLVM_CFLAGS := $(shell $(LLVM_CONFIG) --cflags)
LLVM_LIBS   := $(shell $(LLVM_CONFIG) --libs core bitreader bitwriter irreader \
                                 analysis scalaropts \
                                 support)
LLVM_LDFLAGS := $(shell $(LLVM_CONFIG) --ldflags) -Wl,-rpath,$(shell $(LLVM_CONFIG) --libdir)

CFLAGS := -Wall -Wextra -Wpedantic -Werror -std=c99 -Iinclude $(LLVM_CFLAGS)
LDFLAGS := $(LLVM_LDFLAGS) $(LLVM_LIBS)

BIN_DIR := bin
OUT_DIR := out

CORE_SRC := $(wildcard src/*.c src/**/*.c)

SRC := main.c $(filter-out main.c,$(CORE_SRC))

TEST_SRC := tests/unit.test.c tests/test.c $(CORE_SRC)
INT_TEST_SRC := tests/integration.test.c tests/test.c $(CORE_SRC)

LIB := lib.c $(CORE_SRC)

CFLAGS += -MMD -MP
DEPENDS := $(SRC:.c=.d) $(TEST_SRC:.c=.d) $(INT_TEST_SRC:.c=.d)

.PHONY: all clean help test unit integration lib run

all: $(BIN_DIR)/lunac

$(BIN_DIR)/lunac: $(SRC) | $(BIN_DIR)
	@gcc -o $@ $(SRC) $(CFLAGS) $(LDFLAGS)

$(BIN_DIR)/unit: $(TEST_SRC) | $(BIN_DIR)
	@gcc -o $@ $(TEST_SRC) $(CFLAGS) $(LDFLAGS) -I tests/include

$(BIN_DIR)/int: $(INT_TEST_SRC) | $(BIN_DIR)
	@gcc -o $@ $(INT_TEST_SRC) $(CFLAGS) $(LDFLAGS) -I tests/include

$(BIN_DIR):
	@mkdir -p $@

unit: $(BIN_DIR)/unit
	@./$(BIN_DIR)/unit

integration: $(BIN_DIR)/int
	@./$(BIN_DIR)/int

test: unit integration
	@echo "All tests passed"

lib: $(OUT_DIR)/final
	@echo "Library built"

$(OUT_DIR)/final: $(OUT_DIR)/combined.ll
	@clang $(OUT_DIR)/combined.ll -o $@ $(LDFLAGS)

$(OUT_DIR)/combined.ll: $(OUT_DIR)/source.ll
	@llvm-link $(OUT_DIR)/compiled.ll $< -S -o $@

$(OUT_DIR)/source.ll: lib.c
	@mkdir -p $(OUT_DIR)
	@clang -S -emit-llvm -std=c99 $< -o $@ -Iinclude

clean:
	@rm -rf $(BIN_DIR) $(OUT_DIR)

help:
	@echo "Usage: make [target]"
	@echo ""
	@echo "Targets:"
	@echo "  all          Build lunac (default)"
	@echo "  unit         Build and run unit tests"
	@echo "  integration  Build and run integration tests"
	@echo "  test         Run all tests"
	@echo "  lib          Build library"
	@echo "  clean        Remove all generated files"
	@echo "  help         Show this message"
	@echo ""
	@echo "Variables:"
	@echo "  LLVM_CONFIG  Path to llvm-config (default: llvm-config)"
	@echo "  CFLAGS       Additional compiler flags"

run: all
	@./$(BIN_DIR)/lunac

-include $(DEPENDS)

.DELETE_ON_ERROR: