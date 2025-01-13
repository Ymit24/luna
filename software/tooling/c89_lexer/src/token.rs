#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Token {
    Identifier(String),
    Constant,
    StringLiteral,

    // Keywords
    Auto,
    Double,
    Int,
    Struct,
    Break,
    Else,
    Long,
    Switch,
    Case,
    Enum,
    Register,
    Typedef,
    Char,
    Extern,
    Return,
    Union,
    Const,
    Float,
    Short,
    Unsigned,
    Continue,
    For,
    Signed,
    Void,
    Default,
    Goto,
    Sizeof,
    Volatile,
    Do,
    If,
    Static,
    While,

    // Operators
    LeftBracket,     // [
    RightBracket,    // ]
    LeftBrace,       // {
    RightBrace,      // }
    LeftParen,       // (
    RightParen,      // )
    Dot,             // .
    Arrow,           // ->
    PlusPlus,        // ++
    MinusMinus,      // --
    Ampersand,       // &
    Asterisk,        // *
    Plus,            // +
    Minus,           // -
    Tilde,           // ~
    Bang,            // !
    Slash,           // /
    Percent,         // %
    LeftShift,       // <<
    RightShift,      // >>
    Less,            // <
    Greater,         // >
    LessEqual,       // <=
    GreaterEqual,    // >=
    EqualEqual,      // ==
    NotEqual,        // !=
    Caret,           // ^
    Pipe,            // |
    AndAnd,          // &&
    OrOr,            // ||
    Question,        // ?
    Colon,           // :
    Semicolon,       // ;
    Equal,           // =
    AsteriskEqual,   // *=
    SlashEqual,      // /=
    PercentEqual,    // %=
    PlusEqual,       // +=
    MinusEqual,      // -=
    LeftShiftEqual,  // <<=
    RightShiftEqual, // >>=
    AmpersandEqual,  // &=
    CaretEqual,      // ^=
    PipeEqual,       // |=
    Comma,           // ,
    Hash,            // #
    HashHash,        // ##
    Ellipsis,        // ...
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
