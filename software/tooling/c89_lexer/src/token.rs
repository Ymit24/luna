#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Token {
    Keyword(String),
    Identifier(String),
    Constant,
    StringLiteral,
    Operator,
    Punctuator,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum PreprocessorToken {
    HeaderName,
    Identifier,
    PPNumber,
    CharacterConstant,
    StringLiteral,
    Operator,
    Punctuator,
}
