#[derive(Debug, Clone, PartialEq, Eq)]
pub enum Token {
    Literal(u16),
    Plus,
    Minus,
    LeftParen,
    RightParen,
    Int,
    Ident(String),
    Assign,
    Illegal,
    EOF,
}
