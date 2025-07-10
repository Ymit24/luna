#include "instruction_executor.h"
#include "arena_allocator.h"
#include "ast.h"
#include "instruction_builder.h"
#include "instructions.h"
#include "luna_string.h"
#include "pretty_print.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

struct VMState vm_init(struct ArenaAllocator *allocator) {
  struct VMState vm = {
      .allocator = allocator,
      .halted = false,
      .debug_mode = true,
      .function_count = 0,
      .call_depth = 0,
      .has_pending_call = false,
  };

  // Initialize all memory to zero
  memset(vm.memory, 0, sizeof(vm.memory));

  // Initialize stack pointer to start of stack segment
  vm_set_sp(&vm, VM_STACK_BASE);
  
  // Initialize local base pointer 
  vm_set_lcl(&vm, VM_STACK_BASE);

  printf(BOLD CYAN "🚀 Luna VM initialized\n" RESET);
  printf(DIM "Total memory: %d words (64K)\n" RESET, VM_MEMORY_SIZE);
  printf(DIM "Stack: RAM[%d-%d]\n" RESET, VM_STACK_BASE, VM_STACK_MAX);
  printf(DIM "Static: RAM[%d-%d]\n" RESET, VM_STATIC_BASE, VM_STATIC_MAX);
  printf(DIM "Heap: RAM[%d-%d]\n" RESET, VM_HEAP_BASE, VM_HEAP_MAX);
  printf("\n");

  return vm;
}

void vm_set_debug(struct VMState *vm, bool debug) { 
  vm->debug_mode = debug; 
}

// Direct memory access functions
uint16_t vm_read_memory(struct VMState *vm, uint16_t address) {
  // uint16_t naturally bounds address to 0-65535, so no assert needed
  return vm->memory[address];
}

void vm_write_memory(struct VMState *vm, uint16_t address, uint16_t value) {
  // uint16_t naturally bounds address to 0-65535, so no assert needed
  vm->memory[address] = value;
}

// Pointer management
uint16_t vm_get_sp(struct VMState *vm) {
  return vm->memory[VM_SP_ADDR];
}

void vm_set_sp(struct VMState *vm, uint16_t value) {
  assert(value >= VM_STACK_BASE && value <= VM_STACK_MAX + 1);
  vm->memory[VM_SP_ADDR] = value;
}

uint16_t vm_get_lcl(struct VMState *vm) {
  return vm->memory[VM_LCL_ADDR];
}

void vm_set_lcl(struct VMState *vm, uint16_t value) {
  assert(value >= VM_STACK_BASE && value <= VM_STACK_MAX);
  vm->memory[VM_LCL_ADDR] = value;
}

// Stack operations (computation stack)
void vm_push(struct VMState *vm, uint16_t value) {
  uint16_t sp = vm_get_sp(vm);
  assert(sp <= VM_STACK_MAX);
  
  vm_write_memory(vm, sp, value);
  vm_set_sp(vm, sp + 1);

  if (vm->debug_mode) {
    printf(GREEN "  📤 Push: %d (SP: %d -> %d)\n" RESET, value, sp, sp + 1);
  }
}

uint16_t vm_pop(struct VMState *vm) {
  uint16_t sp = vm_get_sp(vm);
  assert(sp > VM_STACK_BASE);
  
  sp--;
  vm_set_sp(vm, sp);
  uint16_t value = vm_read_memory(vm, sp);

  if (vm->debug_mode) {
    printf(YELLOW "  📥 Pop: %d (SP: %d -> %d)\n" RESET, value, sp + 1, sp);
  }

  return value;
}

uint16_t vm_peek(struct VMState *vm) {
  uint16_t sp = vm_get_sp(vm);
  assert(sp > VM_STACK_BASE);
  return vm_read_memory(vm, sp - 1);
}

uint16_t vm_get_memory(struct VMState *vm, enum MemorySegment segment, uint16_t index) {
  uint16_t address;
  uint16_t value;

  switch (segment) {
  case MS_LOCAL: {
    uint16_t lcl = vm_get_lcl(vm);
    address = lcl + index;
    assert(address <= VM_STACK_MAX);
    value = vm_read_memory(vm, address);
    break;
  }
  case MS_STATIC: {
    address = VM_STATIC_BASE + index;
    assert(address <= VM_STATIC_MAX);
    value = vm_read_memory(vm, address);
    break;
  }
  case MS_CONST:
    // Constants are handled differently - they're embedded in instructions
    assert(false && "Constants should not be accessed via get_memory");
    value = 0;
    break;
  default:
    assert(false && "Unknown memory segment");
    value = 0;
  }

  if (vm->debug_mode) {
    const char *seg_name = (segment == MS_LOCAL)   ? "LOCAL"
                           : (segment == MS_STATIC) ? "STATIC"
                                                    : "CONST";
    printf(BLUE "  🔍 Read %s[%d] @ RAM[%d] = %d\n" RESET, seg_name, index, address, value);
  }

  return value;
}

void vm_set_memory(struct VMState *vm, enum MemorySegment segment, uint16_t index, uint16_t value) {
  uint16_t address;

  switch (segment) {
  case MS_LOCAL: {
    uint16_t lcl = vm_get_lcl(vm);
    address = lcl + index;
    assert(address <= VM_STACK_MAX);
    vm_write_memory(vm, address, value);
    break;
  }
  case MS_STATIC: {
    address = VM_STATIC_BASE + index;
    assert(address <= VM_STATIC_MAX);
    vm_write_memory(vm, address, value);
    break;
  }
  case MS_CONST:
    assert(false && "Cannot write to constant segment");
    break;
  default:
    assert(false && "Unknown memory segment");
  }

  if (vm->debug_mode) {
    const char *seg_name = (segment == MS_LOCAL)   ? "LOCAL"
                           : (segment == MS_STATIC) ? "STATIC"
                                                    : "CONST";
    printf(MAGENTA "  ✏️  Write %s[%d] @ RAM[%d] = %d\n" RESET, seg_name, index, address, value);
  }
}

void vm_print_instruction(struct Instruction *instruction) {
  printf(WHITE "  💻 " RESET);

  switch (instruction->type) {
  case IT_PUSH: {
    struct PushPopLeaInstruction *inst = &instruction->value.pushpoplea;
    const char *seg_name = (inst->memory_segment == MS_LOCAL)   ? "LOCAL"
                           : (inst->memory_segment == MS_STATIC) ? "STATIC"
                                                                : "CONST";
    if (inst->is_index) {
      printf("PUSH %s %d", seg_name, inst->value.index);
    } else {
      printf("PUSH %s %.*s", seg_name, inst->value.label.length,
             inst->value.label.data);
    }
    break;
  }
  case IT_POP: {
    struct PushPopLeaInstruction *inst = &instruction->value.pushpoplea;
    const char *seg_name = (inst->memory_segment == MS_LOCAL)   ? "LOCAL"
                           : (inst->memory_segment == MS_STATIC) ? "STATIC"
                                                                : "CONST";
    printf("POP %s %d", seg_name, inst->value.index);
    break;
  }
  case IT_LEA: {
    struct PushPopLeaInstruction *inst = &instruction->value.pushpoplea;
    const char *seg_name = (inst->memory_segment == MS_LOCAL)   ? "LOCAL"
                           : (inst->memory_segment == MS_STATIC) ? "STATIC"
                                                                : "CONST";
    printf("LEA %s %d", seg_name, inst->value.index);
    break;
  }
  case IT_LOAD:
    printf("LOAD");
    break;
  case IT_STORE:
    printf("STORE");
    break;
  case IT_ADD:
    printf("ADD");
    break;
  case IT_SUB:
    printf("SUB");
    break;
  case IT_CALL:
    printf("CALL");
    break;
  case IT_RETURN:
    printf("RETURN");
    break;
  case IT_LABEL:
    printf(CYAN "🏷️  %.*s:" RESET, instruction->value.label.length,
           instruction->value.label.data);
    printf("\n");
    return; // Labels don't need the newline below
  }
  printf("\n");
}

void vm_print_memory_segments(struct VMState *vm) {
  printf(BOLD "\n🗂️  Memory Segments:\n" RESET);
  
  // Print stack pointer info
  uint16_t sp = vm_get_sp(vm);
  uint16_t lcl = vm_get_lcl(vm);
  printf(CYAN "SP: %d, LCL: %d\n" RESET, sp, lcl);
  
  // Print computation stack
  printf(YELLOW "Stack (SP=%d): [" RESET, sp);
  for (uint16_t i = VM_STACK_BASE; i < sp && i < VM_STACK_BASE + 10; i++) {
    if (i > VM_STACK_BASE) printf(", ");
    printf("%d", vm->memory[i]);
  }
  if (sp > VM_STACK_BASE + 10) printf("...");
  printf(YELLOW "]\n" RESET);

  // Print locals (relative to LCL)
  printf(BLUE "Local (LCL+offset): [" RESET);
  bool first = true;
  for (int i = 0; i < 10; i++) {
    uint16_t addr = lcl + i;
    if (addr <= VM_STACK_MAX && vm->memory[addr] != 0) {
      if (!first) printf(", ");
      printf("%d=%d", i, vm->memory[addr]);
      first = false;
    }
  }
  printf(BLUE "]\n" RESET);

  // Print statics
  printf(MAGENTA "Static: [" RESET);
  first = true;
  for (uint16_t i = VM_STATIC_BASE; i <= VM_STATIC_MAX && i < VM_STATIC_BASE + 10; i++) {
    if (vm->memory[i] != 0) {
      if (!first) printf(", ");
      printf("%d=%d", i - VM_STATIC_BASE, vm->memory[i]);
      first = false;
    }
  }
  printf(MAGENTA "]\n" RESET);
}

void vm_print_state(struct VMState *vm) {
  vm_print_memory_segments(vm);
  printf(DIM "────────────────────────────────────────\n" RESET);
}

// Function management functions
void vm_register_function(struct VMState *vm, struct LunaString label, struct Instruction *instruction) {
  if (vm->function_count >= 256) {
    printf(RED "  ❌ Function table full\n" RESET);
    vm->halted = true;
    return;
  }
  
  vm->function_table[vm->function_count].label = label;
  vm->function_table[vm->function_count].instruction = instruction;
  vm->function_count++;
  
  if (vm->debug_mode) {
    printf(CYAN "  📝 Registered function: %.*s\n" RESET, label.length, label.data);
  }
}

struct Instruction *vm_find_function(struct VMState *vm, struct LunaString label) {
  for (uint16_t i = 0; i < vm->function_count; i++) {
    if (vm->function_table[i].label.length == label.length &&
        strncmp(vm->function_table[i].label.data, label.data, label.length) == 0) {
      return vm->function_table[i].instruction;
    }
  }
  return NULL;
}

void vm_execute_instruction(struct VMState *vm, struct Instruction *instruction) {
  if (vm->debug_mode) {
    vm_print_instruction(instruction);
  }

  switch (instruction->type) {
  case IT_PUSH: {
    struct PushPopLeaInstruction *inst = &instruction->value.pushpoplea;
    if (inst->memory_segment == MS_CONST) {
      // For constants, the value is embedded in the instruction
      if (inst->is_index) {
        vm_push(vm, inst->value.index);
      } else {
        // For label constants, we could implement a symbol table lookup
        // For now, just push 0 as a placeholder
        printf(RED "  ⚠️  Label constants not yet implemented\n" RESET);
        vm_push(vm, 0);
      }
    } else {
      // Push value from memory segment
      uint16_t value = vm_get_memory(vm, inst->memory_segment, inst->value.index);
      vm_push(vm, value);
    }
    break;
  }
  case IT_POP: {
    struct PushPopLeaInstruction *inst = &instruction->value.pushpoplea;
    uint16_t value = vm_pop(vm);
    vm_set_memory(vm, inst->memory_segment, inst->value.index, value);
    break;
  }
  case IT_LEA: {
    struct PushPopLeaInstruction *inst = &instruction->value.pushpoplea;
    // Load Effective Address - pushes the address, not the value
    uint16_t address;
    switch (inst->memory_segment) {
    case MS_LOCAL: {
      uint16_t lcl = vm_get_lcl(vm);
      address = lcl + inst->value.index;
      break;
    }
    case MS_STATIC: {
      address = VM_STATIC_BASE + inst->value.index;
      break;
    }
    default:
      assert(false && "LEA not supported for this segment");
      address = 0;
    }
    vm_push(vm, address);
    if (vm->debug_mode) {
      printf(CYAN "  📍 LEA: pushed address %d\n" RESET, address);
    }
    break;
  }
  case IT_LOAD: {
    // Pop address, push value at that address
    uint16_t address = vm_pop(vm);
    uint16_t value = vm_read_memory(vm, address);
    vm_push(vm, value);
    if (vm->debug_mode) {
      printf(CYAN "  🔄 LOAD: RAM[%d] = %d\n" RESET, address, value);
    }
    break;
  }
  case IT_STORE: {
    // Pop value, pop address, store value at address
    uint16_t value = vm_pop(vm);
    uint16_t address = vm_pop(vm);
    vm_write_memory(vm, address, value);
    if (vm->debug_mode) {
      printf(CYAN "  💾 STORE: %d -> RAM[%d]\n" RESET, value, address);
    }
    break;
  }
  case IT_ADD: {
    uint16_t b = vm_pop(vm);
    uint16_t a = vm_pop(vm);
    uint16_t result = a + b;
    vm_push(vm, result);
    if (vm->debug_mode) {
      printf(GREEN "  ➕ ADD: %d + %d = %d\n" RESET, a, b, result);
    }
    break;
  }
  case IT_SUB: {
    uint16_t b = vm_pop(vm);
    uint16_t a = vm_pop(vm);
    uint16_t result = a - b;
    vm_push(vm, result);
    if (vm->debug_mode) {
      printf(GREEN "  ➖ SUB: %d - %d = %d\n" RESET, a, b, result);
    }
    break;
  }
  case IT_CALL: {
    // Pop the target address from the stack
    uint16_t target_address = vm_pop(vm);
    
    if (vm->call_depth >= VM_MAX_CALL_DEPTH) {
      printf(RED "  ❌ Call stack overflow\n" RESET);
      vm->halted = true;
      break;
    }
    
    // Save current frame - store the ACTUAL return instruction
    vm->call_stack[vm->call_depth].return_lcl = vm_get_lcl(vm);
    vm->call_stack[vm->call_depth].return_sp = vm_get_sp(vm);
    vm->call_stack[vm->call_depth].return_instruction = instruction->next; // Where to return to
    vm->call_depth++;
    
    // Set up new frame
    uint16_t new_lcl = vm_get_sp(vm);
    vm_set_lcl(vm, new_lcl);
    
    if (vm->debug_mode) {
      printf(CYAN "  📞 CALL: address=%d (depth: %d, new LCL: %d)\n" RESET,
             target_address, vm->call_depth, new_lcl);
    }
    
    // Store target address for execution loop to use
    vm->pending_call_target = target_address;
    vm->has_pending_call = true;
    
    break;
  }
  case IT_RETURN: {
    if (vm->call_depth == 0) {
      if (vm->debug_mode) {
        printf(CYAN "  🔙 RETURN: program exit\n" RESET);
      }
      vm->halted = true;
      break;
    }
    
    // Return value should be on top of stack - leave it there
    uint16_t return_value = 0;
    if (vm_get_sp(vm) > vm_get_lcl(vm)) {
      return_value = vm_pop(vm);
    }
    
    // Restore previous frame
    vm->call_depth--;
    vm_set_lcl(vm, vm->call_stack[vm->call_depth].return_lcl);
    vm_set_sp(vm, vm->call_stack[vm->call_depth].return_sp);
    
    // Push return value
    vm_push(vm, return_value);
    
    if (vm->debug_mode) {
      printf(CYAN "  🔙 RETURN: value=%d, restored LCL=%d, SP=%d\n" RESET,
             return_value, vm_get_lcl(vm), vm_get_sp(vm));
    }
    
    // Note: The actual jump back will be handled in vm_execute_group
    break;
  }
  case IT_LABEL:
    // Labels are just markers, no execution needed
    break;
  default:
    printf(RED "  ❌ Unknown instruction type: %d\n" RESET, instruction->type);
    vm->halted = true;
  }
}

void vm_execute_group(struct VMState *vm, struct InstructionGroup *group) {
  if (group == NULL || vm->halted) {
    return;
  }

  printf(BOLD YELLOW "\n🏃 Executing instruction group\n" RESET);

  struct Instruction *current = group->head;
  while (current != NULL && !vm->halted) {
    vm_execute_instruction(vm, current);
    current = current->next;

    if (vm->debug_mode) {
      vm_print_state(vm);
    }
  }
}

void vm_execute_program(struct VMState *vm, struct InstructionBuilder *builder) {
  printf(BOLD GREEN "\n🎯 Starting program execution\n" RESET);
  printf(DIM "══════════════════════════════════════════\n" RESET);

  // Build instruction address mapping for address-based calls
  struct Instruction *instruction_map[10000] = {0}; // Max 10k instructions
  uint16_t current_address = 0;
  
  struct InstructionGroup *group = builder->head;
  while (group != NULL) {
    struct Instruction *instr = group->head;
    while (instr != NULL) {
      if (instr->type != IT_LABEL) {
        instruction_map[current_address] = instr;
        current_address++;
      }
      instr = instr->next;
    }
    group = group->next;
  }

  // Find the .statics group and look for the main() call
  struct InstructionGroup *current_group = builder->head;
  struct Instruction *current_instruction = NULL;
  
  while (current_group != NULL) {
    if (current_group->head != NULL && current_group->head->type == IT_LABEL) {
      struct LunaString label = current_group->head->value.label;
      if (strncmp(label.data, ".statics", 8) == 0) {
        // Find the last CALL instruction in this group (should be main())
        struct Instruction *instr = current_group->head->next; // Skip the label
        struct Instruction *last_call = NULL;
        while (instr != NULL) {
          if (instr->type == IT_CALL) {
            last_call = instr;
          }
          instr = instr->next;
        }
        if (last_call != NULL) {
          current_instruction = last_call;
          break;
        }
      }
    }
    current_group = current_group->next;
  }

  // If no main call found, start from first non-preamble group
  if (current_instruction == NULL) {
    current_group = builder->head;
    while (current_group != NULL) {
      if (current_group->head != NULL && current_group->head->type == IT_LABEL) {
        struct LunaString label = current_group->head->value.label;
        if (strncmp(label.data, ".preamble", 9) != 0 && 
            strncmp(label.data, ".statics", 8) != 0) {
          current_instruction = current_group->head->next; // Skip the label
          break;
        }
      }
      current_group = current_group->next;
    }
  }

  if (current_instruction == NULL && current_group != NULL) {
    current_instruction = current_group->head;
  }

  // Main execution loop with address-based function call support
  while (current_instruction != NULL && !vm->halted) {
    enum InstructionType type = current_instruction->type;
    
    vm_execute_instruction(vm, current_instruction);
    
    if (vm->halted) break;
    
    // Handle control flow
    if (type == IT_CALL) {
      // Jump to address stored in pending call
      if (vm->has_pending_call) {
        uint16_t target_address = vm->pending_call_target;
        vm->has_pending_call = false; // Clear the flag
        
        if (target_address < 10000 && instruction_map[target_address] != NULL) {
          current_instruction = instruction_map[target_address];
          continue;
        } else {
          printf(RED "  ❌ Invalid function address: %d\n" RESET, target_address);
          vm->halted = true;
          break;
        }
      }
    } else if (type == IT_RETURN) {
      // Jump back to caller
      if (vm->call_depth > 0) {
        current_instruction = vm->call_stack[vm->call_depth].return_instruction;
        continue;
      } else {
        // Program exit - main function returned
        break;
      }
    }
    
    // Normal sequential execution
    current_instruction = current_instruction->next;
    
    // If we reached the end of current group, move to next
    if (current_instruction == NULL) {
      current_group = current_group->next;
      if (current_group != NULL) {
        current_instruction = current_group->head;
        // Skip labels
        while (current_instruction != NULL && current_instruction->type == IT_LABEL) {
          current_instruction = current_instruction->next;
        }
      }
    }

    if (vm->debug_mode && current_instruction != NULL) {
      vm_print_state(vm);
    }
  }

  if (vm->halted) {
    printf(BOLD RED "\n🛑 VM halted\n" RESET);
  } else {
    printf(BOLD GREEN "\n✅ Program execution completed successfully\n" RESET);
  }

  printf(DIM "══════════════════════════════════════════\n" RESET);
  vm_print_state(vm);
} 
