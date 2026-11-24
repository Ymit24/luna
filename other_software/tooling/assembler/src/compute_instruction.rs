use ux::{u3, u7};

use crate::operation::Operation;
use crate::parse_error::ParseError;
use crate::{jump_condition::JumpCondition, Destination};

#[derive(Debug, Clone, Copy)]
pub struct ComputeInstruction {
    destination: Destination, // for now just single destination
    operation: Operation,
    jump_condition: JumpCondition,
}

impl ComputeInstruction {
    pub fn new(
        destination: Destination,
        operation: Operation,
        jump_condition: JumpCondition,
    ) -> Self {
        Self {
            destination,
            operation,
            jump_condition,
        }
    }
}

impl From<ComputeInstruction> for u16 {
    fn from(value: ComputeInstruction) -> Self {
        let top_mask = 0b1110_0000_0000_0000;
        let dest_mask: u16 = u16::from(u3::from(value.destination)) << 3;
        let jmp_mask: u16 = u16::from(u3::from(value.jump_condition));
        let op_mask: u16 = u16::from(u7::from(value.operation)) << 6;
        top_mask | op_mask | dest_mask | jmp_mask
    }
}

impl TryFrom<String> for ComputeInstruction {
    type Error = ParseError;

    fn try_from(value: String) -> Result<Self, Self::Error> {
        if value.len() == 0 {
            return Err(ParseError::NotInstruction);
        }
        let (reg_str, rem) = match value.split_once('=') {
            None => return Err(ParseError::ComputeMissingEquals),
            Some(reg) => reg,
        };

        let dest = match reg_str.trim().to_uppercase().as_str() {
            "A" => Destination::A,
            "D" => Destination::D,
            "M" => Destination::M,
            "_" => Destination::Null,
            _ => return Err(ParseError::IllegalDestination),
        };

        let (op_str, rem) = match rem.split_once(';') {
            None => (rem, ""),
            Some(x) => x,
        };

        let opcode = match remove_all_whitespace(op_str.trim().to_uppercase()).as_str() {
            "0" => Operation::Zero,
            "1" => Operation::One,
            "-1" => Operation::NegOne,
            "D" => Operation::D,
            "A" => Operation::A,
            "!D" => Operation::NotD,
            "!A" => Operation::NotA,
            "D+1" => Operation::DPlus1,
            "A+1" => Operation::APlus1,
            "D-1" => Operation::DMin1,
            "A-1" => Operation::AMin1,
            "D+A" => Operation::DPlusA,
            "D-A" => Operation::DMinA,
            "A-D" => Operation::AMinD,
            "D&A" => Operation::DAndA,
            "D|A" => Operation::DOrA,

            "M" => Operation::M,
            "!M" => Operation::NotM,
            "-M" => Operation::NegM,
            "M+1" => Operation::MPlus1,
            "M-1" => Operation::MMin1,
            "D+M" => Operation::DPlusM,
            "D-M" => Operation::DMinM,
            "M-D" => Operation::MMinD,
            "D&M" => Operation::DAndM,
            "D|M" => Operation::DOrM,
            _ => return Err(ParseError::IllegalOperation),
        };

        let jmp = match remove_all_whitespace(rem.to_uppercase()).as_str() {
            "" => JumpCondition::Null,
            "JGT" => JumpCondition::JGT,
            "JEQ" => JumpCondition::JEQ,
            "JGE" => JumpCondition::JGE,
            "JLT" => JumpCondition::JLT,
            "JNE" => JumpCondition::JNE,
            "JLE" => JumpCondition::JLE,
            "JMP" => JumpCondition::JMP,
            _ => return Err(ParseError::IllegalJump),
        };

        Ok(ComputeInstruction {
            destination: dest,
            operation: opcode,
            jump_condition: jmp,
        })
    }
}

fn remove_all_whitespace<T: Into<String>>(s: T) -> String {
    s.into().chars().filter(|c| !c.is_whitespace()).collect()
}
