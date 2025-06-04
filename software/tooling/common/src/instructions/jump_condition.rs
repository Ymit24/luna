#[derive(Debug, Clone, PartialEq, Eq)]
pub enum JumpCondition {
    Never,
    Greaterthan,
    Equal,
    Greaterthanequals,
    Lessthan,
    Notequal,
    Lessthanequal,
    Unconditional,
}
