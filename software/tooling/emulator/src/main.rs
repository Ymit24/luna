use common::instructions::{AluFlags, Instruction, Register};
use processor::Processor;

mod processor;

fn main() {
    let program = vec![
        Instruction::Compute(common::instructions::ComputeInstruction {
            destination: common::instructions::Destination::Data,
            operand1: Register::Data,
            operand2: Register::One,
            alu_flags: AluFlags {
                zero_x: false,
                zero_y: false,
                negate_output: false,
            },
            opcode: common::instructions::Opcode::Add,
            jump_condition: common::instructions::jump_condition::JumpCondition::Never,
        }),
        Instruction::Compute(common::instructions::ComputeInstruction {
            destination: common::instructions::Destination::Data,
            operand1: Register::Data,
            operand2: Register::One,
            alu_flags: AluFlags {
                zero_x: false,
                zero_y: false,
                negate_output: false,
            },
            opcode: common::instructions::Opcode::Add,
            jump_condition: common::instructions::jump_condition::JumpCondition::Never,
        }),
    ];

    let mut processor = Processor::new(&program);

    processor.run();
}
