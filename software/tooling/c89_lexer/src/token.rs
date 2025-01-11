#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Token {
    Keyword,
    Identifier,
    Constant,
    StringLiteral,
    Operator,
    Punctuator,
}
