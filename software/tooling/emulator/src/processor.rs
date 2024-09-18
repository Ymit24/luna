use std::collections::HashMap;

use common::instructions::{self, AddressInstruction, ComputeInstruction, Instruction, Register};

pub struct Processor {
    pub reg_map: HashMap<Register, i16>,
    pub memory: [i16; 16 * 1024],
    pub pc: i16,
    pub program: Vec<Instruction>,
}

impl Processor {
    pub fn new(program: &[Instruction]) -> Processor {
        Processor {
            pc: 0,
            reg_map: HashMap::new(),
            memory: [0; 16 * 1024],
            program: Vec::from(program),
        }
    }

    pub fn reset(&mut self) {
        self.pc = 0;
        self.reg_map.insert(Register::Address, 0);
        self.reg_map.insert(Register::Data, 0);
        self.reg_map.insert(Register::Memory, 0);
        self.reg_map.insert(Register::One, 1);
    }

    pub fn run(&mut self) {
        self.reset();

        let mut cycle = 0;
        while (self.pc as usize) < self.program.len() {
            let instruction = self.program[self.pc as usize].clone();

            match instruction {
                Instruction::Address(AddressInstruction { addr }) => {
                    let addr: u16 = addr.into();
                    self.reg_map.insert(Register::Address, addr as i16);
                }
                Instruction::Compute(ComputeInstruction {
                    operand1,
                    operand2,
                    opcode,
                    jump_condition,
                    destination,
                    alu_flags,
                }) => {
                    let result = self.alu(alu_flags, operand1, operand2, opcode);
                    self.store_result(destination, result);
                    self.pc = self.handle_jump(jump_condition, result);
                }
            }

            cycle += 1;
            println!(
                "[{}] A: {} D: {} M: {} PC: {}",
                cycle,
                self.reg_map[&Register::Address],
                self.reg_map[&Register::Data],
                self.reg_map[&Register::Memory],
                self.pc
            );
        }
    }

    fn store_result(&mut self, destination: instructions::Destination, result: i16) {
        match destination {
            instructions::Destination::Address => {
                self.reg_map.insert(Register::Address, result);
            }
            instructions::Destination::Data => {
                self.reg_map.insert(Register::Data, result);
            }
            instructions::Destination::Memory => {
                self.memory[self.reg_map[&Register::Address] as usize] = result;
            }
        }
    }

    fn alu(
        &mut self,
        alu_flags: instructions::AluFlags,
        operand1: Register,
        operand2: Register,
        opcode: instructions::Opcode,
    ) -> i16 {
        let op1 = if alu_flags.zero_x {
            0i16
        } else {
            self.reg_map[&operand1]
        };
        let op2 = if alu_flags.zero_y {
            0i16
        } else {
            self.reg_map[&operand2]
        };

        let result: i16 = match opcode {
            instructions::Opcode::Add => op1 + op2,
            instructions::Opcode::Sub => op1 - op2,
            instructions::Opcode::And => op1 & op2,
            instructions::Opcode::Or => op1 | op2,
        };

        let result = if alu_flags.negate_output {
            !result
        } else {
            result
        };
        result
    }

    fn handle_jump(
        &mut self,
        jump_condition: instructions::jump_condition::JumpCondition,
        result: i16,
    ) -> i16 {
        let should_jump = match jump_condition {
            instructions::jump_condition::JumpCondition::Never => false,
            instructions::jump_condition::JumpCondition::Greaterthan => result > 0,
            instructions::jump_condition::JumpCondition::Equal => result == 0,
            instructions::jump_condition::JumpCondition::Greaterthanequals => result >= 0,
            instructions::jump_condition::JumpCondition::Lessthan => result < 0,
            instructions::jump_condition::JumpCondition::Notequal => result != 0,
            instructions::jump_condition::JumpCondition::Lessthanequal => result <= 0,
            instructions::jump_condition::JumpCondition::Unconditional => true,
        };
        if should_jump {
            self.reg_map[&Register::Address]
        } else {
            self.pc + 1
        }
    }
}
