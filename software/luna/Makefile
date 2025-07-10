# Luna Compiler Makefile

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Werror -std=c99
INCLUDES = -Iinclude
DEBUG_FLAGS = -g -DDEBUG
RELEASE_FLAGS = -O2 -DNDEBUG

# Directories
SRC_DIR = src
INCLUDE_DIR = include
OBJ_DIR = obj
TARGET = lunac

# Source files
SOURCES = main.c \
          src/luna_string.c \
          src/lexer.c \
          src/parser.c \
          src/ast.c \
          src/arena_allocator.c \
          src/annotator.c \
          src/instruction_builder.c \
          src/instruction_executor.c \
          src/pretty_print.c \
          src/ast_printer.c \
          src/code_gen.c \
          src/linker.c

# Object files
OBJECTS = $(SOURCES:%.c=$(OBJ_DIR)/%.o)

# Default target
.PHONY: all clean debug debug-build release run help

all: $(TARGET)

# Release build (default)
$(TARGET): CFLAGS += $(RELEASE_FLAGS)
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

# Debug build (clean rebuild)
debug: CFLAGS += $(DEBUG_FLAGS)
debug: clean $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

# Debug build (incremental)
debug-build: CFLAGS += $(DEBUG_FLAGS)
debug-build: $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

# Release build (explicit)
release: CFLAGS += $(RELEASE_FLAGS)
release: clean $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

# Build and run (incremental debug build)
run: debug-build
	./$(TARGET)

# Object file compilation
$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Create object directory
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR) $(OBJ_DIR)/$(SRC_DIR)

# Automatic dependency generation
-include $(OBJECTS:.o=.d)

$(OBJ_DIR)/%.d: %.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -MM -MT $(@:.d=.o) $< > $@

# Clean build artifacts
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# Clean and rebuild
rebuild: clean all

# Help target
help:
	@echo "Luna Compiler Makefile"
	@echo "Available targets:"
	@echo "  all         - Build the compiler (release mode)"
	@echo "  debug       - Clean and build with debug flags"
	@echo "  debug-build - Incremental build with debug flags"
	@echo "  release     - Clean and build with optimization"
	@echo "  run         - Incremental debug build and run"
	@echo "  clean       - Remove build artifacts"
	@echo "  rebuild     - Clean and build"
	@echo "  help        - Show this help message"

# Header dependencies (manual for key files)
$(OBJ_DIR)/main.o: $(wildcard $(INCLUDE_DIR)/*.h)
$(OBJ_DIR)/$(SRC_DIR)/%.o: $(wildcard $(INCLUDE_DIR)/*.h)
