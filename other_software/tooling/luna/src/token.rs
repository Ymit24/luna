pub enum Token {
    Illegal,
    Eof,
    Integer { literal: i32 },
}
