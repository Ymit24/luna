use thiserror::Error;

/// Errors that can occur during instruction parsing
#[derive(Debug, Error, Clone, PartialEq)]
pub enum ParseError {
    #[error("End of input reached")]
    EndOfInput,
    
    #[error("Invalid instruction format")]
    NotInstruction,
    
    #[error("Comment line, skipping")]
    SkipComment,
    
    #[error("Compute instruction missing equals sign")]
    ComputeMissingEquals,
    
    #[error("Invalid destination register")]
    IllegalDestination,
    
    #[error("Invalid operation")]
    IllegalOperation,
    
    #[error("Invalid jump condition")]
    IllegalJump,
}
