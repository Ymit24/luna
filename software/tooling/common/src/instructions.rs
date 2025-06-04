use ux::u15;

pub mod jump_condition;
use jump_condition::JumpCondition;

#[derive(Debug, Clone)]
pub struct ComputeInstruction {
    pub destination: Destination,

    pub operand1: Register,
    pub operand2: Register,
    pub alu_flags: AluFlags,
    pub opcode: Opcode,
    pub jump_condition: JumpCondition,

    pub label: Option<Label>,
}

#[derive(Debug, Clone)]
pub struct Label {
    pub label: String,
}

#[derive(Debug, Clone)]
pub struct AluFlags {
    pub zero_x: bool,
    pub zero_y: bool,
    pub negate_output: bool,
}

#[derive(Debug, Clone)]
pub enum Opcode {
    Add,
    Sub,
    Or,
    And,
}

#[derive(Debug, Clone)]
pub enum Destination {
    Address,
    Data,
    Memory,
}

#[derive(Debug, Clone)]
pub struct AddressInstruction {
    pub addr: u15,
    pub label: Option<Label>,
}

#[derive(Debug, Clone)]
pub enum Instruction {
    Compute(ComputeInstruction),
    Address(AddressInstruction),
}

#[derive(Debug, Clone, Eq, PartialEq, Hash)]
pub enum Register {
    Address,
    Data,
    Memory,
    One,
}
