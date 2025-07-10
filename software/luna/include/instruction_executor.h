#ifndef INSTRUCTION_EXECUTOR_H
#define INSTRUCTION_EXECUTOR_H

#include "arena_allocator.h"
#include "instruction_builder.h"
#include "instructions.h"
#include "luna_string.h"
#include <stdbool.h>
#include <stdint.h>

// Total addressable memory (64k words)
#define VM_MEMORY_SIZE 65536

// Memory segment base addresses (following nand2tetris design)
#define VM_STACK_BASE     256     // Stack starts at RAM[256]
#define VM_STACK_MAX      2047    // Stack ends at RAM[2047]
#define VM_STATIC_BASE    16      // Static variables start at RAM[16] 
#define VM_STATIC_MAX     255     // Static variables end at RAM[255]
#define VM_HEAP_BASE      2048    // Heap starts at RAM[2048]
#define VM_HEAP_MAX       16383   // Heap ends at RAM[16383]

// Special pointer registers (RAM[0-15])
#define VM_SP_ADDR        0       // Stack pointer at RAM[0]
#define VM_LCL_ADDR       1       // Local base pointer at RAM[1] 
#define VM_ARG_ADDR       2       // Argument base pointer at RAM[2]
#define VM_THIS_ADDR      3       // This pointer at RAM[3]
#define VM_THAT_ADDR      4       // That pointer at RAM[4]

struct VMState {
  // Unified 64k memory space
  uint16_t memory[VM_MEMORY_SIZE];
  
  // Execution state
  struct ArenaAllocator *allocator;
  bool halted;
  bool debug_mode;
};

// Initialize VM state
struct VMState vm_init(struct ArenaAllocator *allocator);

// Execute all instruction groups
void vm_execute_program(struct VMState *vm, struct InstructionBuilder *builder);

// Execute a single instruction group
void vm_execute_group(struct VMState *vm, struct InstructionGroup *group);

// Execute a single instruction
void vm_execute_instruction(struct VMState *vm, struct Instruction *instruction);

// Stack operations (computation stack)
void vm_push(struct VMState *vm, uint16_t value);
uint16_t vm_pop(struct VMState *vm);
uint16_t vm_peek(struct VMState *vm);

// Memory operations
uint16_t vm_get_memory(struct VMState *vm, enum MemorySegment segment, uint16_t index);
void vm_set_memory(struct VMState *vm, enum MemorySegment segment, uint16_t index, uint16_t value);

// Direct memory access
uint16_t vm_read_memory(struct VMState *vm, uint16_t address);
void vm_write_memory(struct VMState *vm, uint16_t address, uint16_t value);

// Pointer management
uint16_t vm_get_sp(struct VMState *vm);
void vm_set_sp(struct VMState *vm, uint16_t value);
uint16_t vm_get_lcl(struct VMState *vm);
void vm_set_lcl(struct VMState *vm, uint16_t value);

// Debug and display functions
void vm_print_state(struct VMState *vm);
void vm_print_instruction(struct Instruction *instruction);
void vm_set_debug(struct VMState *vm, bool debug);
void vm_print_memory_segments(struct VMState *vm);

#endif
