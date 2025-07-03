#include "instruction_builder.h"
#include "luna_string.h"
#include <assert.h>

uint16_t IB_SP_ADDR = 1, IB_BSP_ADDR = 2, IB_NP_ADDR = 0;

struct InstructionBuilder instruction_builder_make(FILE *output_file) {
  return (struct InstructionBuilder){
      .output_file = output_file,
  };
}

void instr_write_line(struct InstructionBuilder *instruction_builder,
                      struct LunaString line) {
  if (fputs(line.data, instruction_builder->output_file) == EOF) {
    fclose(instruction_builder->output_file);
    puts("Failed to write to output file.");
    assert(0);
  }
}

void instr_push_push(struct InstructionBuilder *instruction_builder,
                     struct LunaString memory_segment, uint16_t location) {
  char buff[64] = {0};
  uint16_t length =
      sprintf(buff, "push %s %d\n", memory_segment.data, location);
  struct LunaString line = string_make(buff);
  assert(line.length == length);
  instr_write_line(instruction_builder, line);
}

void instr_push_pop(struct InstructionBuilder *instruction_builder,
                    struct LunaString memory_segment, uint16_t location) {
  char buff[64] = {0};
  uint16_t length = sprintf(buff, "pop %s %d\n", memory_segment.data, location);
  struct LunaString line = string_make(buff);
  assert(line.length == length);
  instr_write_line(instruction_builder, line);
}

void instr_push_add(struct InstructionBuilder *instruction_builder) {
  instr_write_line(instruction_builder, string_make("add"));
}

void instr_push_sub(struct InstructionBuilder *instruction_builder) {
  instr_write_line(instruction_builder, string_make("sub"));
}

void instr_push_label(struct InstructionBuilder *instruction_builder,
                      struct LunaString label) {
  char buff[64] = {0};
  uint16_t length = sprintf(buff, "$%s\n", label.data);
  struct LunaString line = string_make(buff);
  assert(line.length == length);
  instr_write_line(instruction_builder, line);
}
