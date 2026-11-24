use ux::u3;

#[derive(Debug, Clone, Copy)]
pub enum JumpCondition {
    Null,
    JGT,
    JEQ,
    JGE,
    JLT,
    JNE,
    JLE,
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
