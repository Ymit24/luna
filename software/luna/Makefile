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
          $(SRC_DIR)/luna_string.c \
          $(SRC_DIR)/lexer.c \
          $(SRC_DIR)/parser.c \
          $(SRC_DIR)/ast.c \
          $(SRC_DIR)/arena_allocator.c \
          $(SRC_DIR)/annotator.c \
          $(SRC_DIR)/instruction_builder.c \
          $(SRC_DIR)/instruction_executor.c \
          $(SRC_DIR)/code_gen.c

# Object files
OBJECTS = $(SOURCES:%.c=$(OBJ_DIR)/%.o)

# Default target
.PHONY: all clean debug release help

all: $(TARGET)

# Release build (default)
$(TARGET): CFLAGS += $(RELEASE_FLAGS)
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

# Debug build
debug: CFLAGS += $(DEBUG_FLAGS)
debug: clean $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

# Release build (explicit)
release: CFLAGS += $(RELEASE_FLAGS)
release: clean $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

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
	@echo "  all       - Build the compiler (release mode)"
	@echo "  debug     - Build with debug flags"
	@echo "  release   - Build with optimization (explicit)"
	@echo "  clean     - Remove build artifacts"
	@echo "  rebuild   - Clean and build"
	@echo "  help      - Show this help message"

# Header dependencies (manual for key files)
$(OBJ_DIR)/main.o: $(wildcard $(INCLUDE_DIR)/*.h)
$(OBJ_DIR)/$(SRC_DIR)/%.o: $(wildcard $(INCLUDE_DIR)/*.h)
