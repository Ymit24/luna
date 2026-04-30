# Project Settings
MODULE = vga
V_SRC = vga.v
CPP_SRC = vga_sim.cpp
EXE = vga_sim

# Library Flags
# We use pkg-config for Raylib to ensure paths are correct
RAYLIB_LDFLAGS = $(shell pkg-config --libs raylib 2>/dev/null || echo "-lraylib")
RAYLIB_CFLAGS  = $(shell pkg-config --cflags raylib 2>/dev/null || echo "")

.PHONY: all clean run compile_commands

all: $(EXE)

# We use --build so Verilator handles the C++ compilation internally.
# This structure is much more robust against argument order errors.
$(EXE): $(V_SRC) $(CPP_SRC)
	verilator -Wall -cc --trace --exe --build \
		-Wno-fatal \
		$(RAYLIB_CFLAGS) \
		-LDFLAGS "$(RAYLIB_LDFLAGS)" \
		-o $(EXE) \
		$(V_SRC) $(CPP_SRC)

# For bear: we must clean first so it sees the full build process
compile_commands: clean
	bear -- make all

run: $(EXE)
	./$(EXE)

clean:
	rm -rf obj_dir $(EXE) compile_commands.json
