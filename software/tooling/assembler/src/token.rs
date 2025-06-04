#[derive(Debug, PartialEq, Eq)]
pub enum Token {
    Dollar,
    LabelIdent(String),
    Register(Register),
    Colon,
    At,
    Literal(u16),
    Equal,
    Semicolon,
    Add,
    Sub,
    And,
    Or,
    Jump(Jump),
    EOF,
    Illegal(char),
}

#[derive(Debug, PartialEq, Eq)]
pub enum Register {
    A,
    D,
    M,
    Void,
    One,
}

#[derive(Debug, PartialEq, Eq)]
pub enum Jump {
    JMP,
    JNE,
    JEQ,
    JNQ,
    JGT,
    JLT,
    JGE,
    JLE,
}
