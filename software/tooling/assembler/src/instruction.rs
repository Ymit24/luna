use ux::u15;

use crate::{compute_instruction::ComputeInstruction, parse_error::ParseError};

#[derive(Debug, Clone, Copy)]
pub enum Instruction {
    Address(u15),
    Compute(ComputeInstruction),
}

impl TryFrom<String> for Instruction {
    type Error = ParseError;

    fn try_from(value: String) -> Result<Self, Self::Error> {
        if value.len() == 0 {
            return Err(ParseError::EndOfInput);
        }
        if value.starts_with("#") {
            return Err(ParseError::SkipComment);
        }
        if value.starts_with("@") {
            let imm = &value[1..];
            let imm = match imm.parse::<u16>() {
                Err(_) => return Err(ParseError::NotInstruction),
                Ok(i) => i,
            };
            let imm = match u15::try_from(imm) {
                Err(_) => return Err(ParseError::NotInstruction),
                Ok(i) => i,
            };
            return Ok(Instruction::Address(imm));
        }
        match ComputeInstruction::try_from(value.clone()) {
            Ok(ci) => return Ok(Instruction::Compute(ci)),
            Err(e @ ParseError::IllegalDestination)
            | Err(e @ ParseError::IllegalOperation)
            | Err(e @ ParseError::IllegalJump)
            | Err(e @ ParseError::ComputeMissingEquals) => return Err(e),
            Err(_) => {}
        }
        Err(ParseError::NotInstruction)
    }
}

impl From<Instruction> for u16 {
    fn from(value: Instruction) -> Self {
        match value {
            Instruction::Address(addr) => addr.into(),
            Instruction::Compute(ci) => ci.into(),
        }
    }
}
