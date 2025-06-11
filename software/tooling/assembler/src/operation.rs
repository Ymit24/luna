use ux::u7;

/// Represents the operation to perform in a compute instruction
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Operation {
    /// Set to 0
    Zero,
    /// Set to 1
    One,
    /// Set to -1
    NegOne,
    /// Use D register
    D,
    /// Use A register
    A,
    /// Bitwise NOT of D
    NotD,
    /// Bitwise NOT of A
    NotA,
    /// Negate D
    NegD,
    /// Negate A
    NegA,
    /// D + 1
    DPlus1,
    /// A + 1
    APlus1,
    /// D - 1
    DMin1,
    /// A - 1
    AMin1,
    /// D + A
    DPlusA,
    /// D - A
    DMinA,
    /// A - D
    AMinD,
    /// D & A (bitwise AND)
    DAndA,
    /// D | A (bitwise OR)
    DOrA,
    /// Use M register
    M,
    /// Bitwise NOT of M
    NotM,
    /// Negate M
    NegM,
    /// M + 1
    MPlus1,
    /// M - 1
    MMin1,
    /// D + M
    DPlusM,
    /// D - M
    DMinM,
    /// M - D
    MMinD,
    /// D & M (bitwise AND)
    DAndM,
    /// D | M (bitwise OR)
    DOrM,
}

impl From<Operation> for u7 {
    fn from(value: Operation) -> Self {
        match value {
            Operation::Zero => u7::new(0b0101010),
            Operation::One => u7::new(0b0111111),
            Operation::NegOne => u7::new(0b0111010),
            Operation::D => u7::new(0b0001100),
            Operation::A => u7::new(0b0110000),
            Operation::NotD => u7::new(0b0001101),
            Operation::NotA => u7::new(0b0110001),
            Operation::NegD => u7::new(0b0001111),
            Operation::NegA => u7::new(0b0110011),
            Operation::DPlus1 => u7::new(0b0011111),
            Operation::APlus1 => u7::new(0b0110111),
            Operation::DMin1 => u7::new(0b0001110),
            Operation::AMin1 => u7::new(0b0110010),
            Operation::DPlusA => u7::new(0b0000010),
            Operation::DMinA => u7::new(0b0010011),
            Operation::AMinD => u7::new(0b0000111),
            Operation::DAndA => u7::new(0b0000000),
            Operation::DOrA => u7::new(0b0010101),
            Operation::M => u7::new(0b1110000),
            Operation::NotM => u7::new(0b1110001),
            Operation::NegM => u7::new(0b1110011),
            Operation::MPlus1 => u7::new(0b1110111),
            Operation::MMin1 => u7::new(0b1110010),
            Operation::DPlusM => u7::new(0b1000010),
            Operation::DMinM => u7::new(0b1010011),
            Operation::MMinD => u7::new(0b1000111),
            Operation::DAndM => u7::new(0b1000000),
            Operation::DOrM => u7::new(0b1010101),
        }
    }
}

impl TryFrom<&str> for Operation {
    type Error = crate::parse_error::ParseError;

    fn try_from(value: &str) -> Result<Self, Self::Error> {
        match value {
            "0" => Ok(Operation::Zero),
            "1" => Ok(Operation::One),
            "-1" => Ok(Operation::NegOne),
            "D" => Ok(Operation::D),
            "A" => Ok(Operation::A),
            "!D" => Ok(Operation::NotD),
            "!A" => Ok(Operation::NotA),
            "-D" => Ok(Operation::NegD),
            "-A" => Ok(Operation::NegA),
            "D+1" => Ok(Operation::DPlus1),
            "A+1" => Ok(Operation::APlus1),
            "D-1" => Ok(Operation::DMin1),
            "A-1" => Ok(Operation::AMin1),
            "D+A" => Ok(Operation::DPlusA),
            "D-A" => Ok(Operation::DMinA),
            "A-D" => Ok(Operation::AMinD),
            "D&A" => Ok(Operation::DAndA),
            "D|A" => Ok(Operation::DOrA),
            "M" => Ok(Operation::M),
            "!M" => Ok(Operation::NotM),
            "-M" => Ok(Operation::NegM),
            "M+1" => Ok(Operation::MPlus1),
            "M-1" => Ok(Operation::MMin1),
            "D+M" => Ok(Operation::DPlusM),
            "D-M" => Ok(Operation::DMinM),
            "M-D" => Ok(Operation::MMinD),
            "D&M" => Ok(Operation::DAndM),
            "D|M" => Ok(Operation::DOrM),
            _ => Err(crate::parse_error::ParseError::IllegalOperation),
        }
    }
}
