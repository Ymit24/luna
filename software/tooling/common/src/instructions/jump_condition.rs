#[derive(Debug, Clone)]
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
