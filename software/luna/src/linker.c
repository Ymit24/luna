#include "linker.h"
#include "pretty_print.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

struct Linker linker_init(struct ArenaAllocator *allocator) {
  return (struct Linker){
    .label_count = 0,
    .allocator = allocator,
  };
}

void linker_collect_labels(struct Linker *linker, struct InstructionBuilder *builder) {
  pp_info("🔗 Collecting function labels...");
  
  uint16_t current_address = 0;
  struct InstructionGroup *group = builder->head;
  
  while (group != NULL) {
    struct Instruction *instruction = group->head;
    
    while (instruction != NULL) {
      if (instruction->type == IT_LABEL) {
        // Found a label - record its address
        if (linker->label_count >= MAX_LABELS) {
          pp_error("Too many labels in program");
          assert(false);
        }
        
        linker->labels[linker->label_count] = (struct LabelEntry){
          .label = instruction->value.label,
          .address = current_address,
          .is_resolved = true,
        };
        
        pp_debug("📍 Label %.*s -> address %d", 
                instruction->value.label.length, 
                instruction->value.label.data,
                current_address);
        
        linker->label_count++;
      } else {
        // Non-label instructions take up one address slot
        current_address++;
      }
      
      instruction = instruction->next;
    }
    
    group = group->next;
  }
  
  pp_success("Found %d labels", linker->label_count);
}

void linker_resolve_addresses(struct Linker *linker, struct InstructionBuilder *builder) {
  pp_info("🔗 Resolving label references...");
  
  struct InstructionGroup *group = builder->head;
  uint16_t resolved_count = 0;
  
  while (group != NULL) {
    struct Instruction *instruction = group->head;
    
    while (instruction != NULL) {
      if (instruction->type == IT_PUSH) {
        struct PushPopLeaInstruction *push = &instruction->value.pushpoplea;
        
        // Check if this is a label constant that needs resolving
        if (push->memory_segment == MS_CONST && !push->is_index) {
          uint16_t address = linker_get_address(linker, push->value.label);
          
          // Convert to indexed constant with the resolved address
          push->is_index = true;
          push->value.index = address;
          
          pp_debug("🔄 Resolved label %.*s -> address %d", 
                  push->value.label.length, 
                  push->value.label.data, 
                  address);
          
          resolved_count++;
        }
      }
      
      instruction = instruction->next;
    }
    
    group = group->next;
  }
  
  pp_success("Resolved %d label references", resolved_count);
}

uint16_t linker_get_address(struct Linker *linker, struct LunaString label) {
  for (uint16_t i = 0; i < linker->label_count; i++) {
    if (linker->labels[i].label.length == label.length &&
        strncmp(linker->labels[i].label.data, label.data, label.length) == 0) {
      return linker->labels[i].address;
    }
  }
  
  pp_error("Label not found: %.*s", label.length, label.data);
  assert(false);
  return 0;
} 
