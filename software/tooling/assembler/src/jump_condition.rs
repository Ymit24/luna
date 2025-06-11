use ux::u3;

/// Represents the jump condition for a compute instruction
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum JumpCondition {
    /// No jump (null)
    Null,
    /// Jump if greater than
    JGT,
    /// Jump if equal
    JEQ,
    /// Jump if greater than or equal
    JGE,
    /// Jump if less than
    JLT,
    /// Jump if not equal
    JNE,
    /// Jump if less than or equal
    JLE,
    /// Unconditional jump
    JMP,
}

// TODO: confirm these values
impl From<JumpCondition> for u3 {
    fn from(value: JumpCondition) -> Self {
        match value {
            JumpCondition::Null => u3::new(0b000),
            JumpCondition::JGT => u3::new(0b001),
            JumpCondition::JEQ => u3::new(0b010),
            JumpCondition::JGE => u3::new(0b011),
            JumpCondition::JLT => u3::new(0b100),
            JumpCondition::JNE => u3::new(0b101),
            JumpCondition::JLE => u3::new(0b110),
            JumpCondition::JMP => u3::new(0b111),
        }
    }
}

impl TryFrom<&str> for JumpCondition {
    type Error = crate::parse_error::ParseError;

    fn try_from(value: &str) -> Result<Self, Self::Error> {
        match value {
            "" => Ok(JumpCondition::Null),
            "JGT" => Ok(JumpCondition::JGT),
            "JEQ" => Ok(JumpCondition::JEQ),
            "JGE" => Ok(JumpCondition::JGE),
            "JLT" => Ok(JumpCondition::JLT),
            "JNE" => Ok(JumpCondition::JNE),
            "JLE" => Ok(JumpCondition::JLE),
            "JMP" => Ok(JumpCondition::JMP),
            _ => Err(crate::parse_error::ParseError::IllegalJump),
        }
    }
}
