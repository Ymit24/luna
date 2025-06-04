#[derive(Debug)]
pub enum Token {
    Dollar,
    LabelIdent,
    Register,
    Colon,
    At,
    Literal,
    Equal,
    Semicolon,
    Jump
}
