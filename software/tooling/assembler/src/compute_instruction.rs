use ux::{u3, u7};

use crate::operation::Operation;
use crate::parse_error::ParseError;
use crate::jump_condition::JumpCondition;
use crate::destination::Destination;

/// Represents a compute instruction in the assembly language
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct ComputeInstruction {
    /// The destination register(s) for the computation result
    pub destination: Destination,
    /// The operation to perform
    pub operation: Operation,
    /// The jump condition (if any)
    pub jump_condition: JumpCondition,
}

impl ComputeInstruction {
    /// Creates a new compute instruction
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
        if value.is_empty() {
            return Err(ParseError::NotInstruction);
        }

        // Split into destination and operation/jump parts
        let (dest_str, rem) = value.split_once('=')
            .ok_or(ParseError::ComputeMissingEquals)?;

        // Parse destination
        let dest = Destination::try_from(dest_str.trim())?;

        // Split into operation and jump parts
        let (op_str, jump_str) = match rem.split_once(';') {
            None => (rem, ""),
            Some(x) => x,
        };

        // Parse operation
        let op_str = remove_all_whitespace(op_str.trim());
        let opcode = Operation::try_from(op_str.as_str())?;

        // Parse jump condition
        let jump_str = remove_all_whitespace(jump_str);
        let jmp = JumpCondition::try_from(jump_str.as_str())?;

        Ok(ComputeInstruction::new(dest, opcode, jmp))
    }
}

/// Removes all whitespace characters from a string
fn remove_all_whitespace<T: Into<String>>(s: T) -> String {
    s.into().chars().filter(|c| !c.is_whitespace()).collect()
}
