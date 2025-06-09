#[derive(Debug)]
pub enum ParseError {
    EndOfInput,
    NotInstruction,
    SkipComment,
    ComputeMissingEquals,
    IllegalDestination,
    IllegalOperation,
    IllegalJump,
}
