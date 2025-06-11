use ux::u3;

/// Represents the destination register(s) for a compute instruction
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct Destination {
    /// Bitfield where:
    /// - bit 0: M register
    /// - bit 1: D register
    /// - bit 2: A register
    value: u3,
}

impl Destination {
    /// Creates a new destination with no registers selected
    pub fn new() -> Self {
        Self { value: u3::new(0) }
    }

    /// Creates a destination with only the A register
    pub fn a() -> Self {
        Self { value: u3::new(0b100) }
    }

    /// Creates a destination with only the D register
    pub fn d() -> Self {
        Self { value: u3::new(0b010) }
    }

    /// Creates a destination with only the M register
    pub fn m() -> Self {
        Self { value: u3::new(0b001) }
    }

    /// Returns true if the A register is selected
    pub fn has_a(&self) -> bool {
        (u8::from(self.value) & 0b100) != 0
    }

    /// Returns true if the D register is selected
    pub fn has_d(&self) -> bool {
        (u8::from(self.value) & 0b010) != 0
    }

    /// Returns true if the M register is selected
    pub fn has_m(&self) -> bool {
        (u8::from(self.value) & 0b001) != 0
    }
}

impl From<Destination> for u3 {
    fn from(value: Destination) -> Self {
        value.value
    }
}

impl TryFrom<&str> for Destination {
    type Error = crate::parse_error::ParseError;

    fn try_from(value: &str) -> Result<Self, Self::Error> {
        if value.is_empty() {
            return Ok(Destination::new());
        }

        let mut dest = Destination::new();
        for reg in value.split(',') {
            match reg.trim().to_uppercase().as_str() {
                "A" => dest.value = u3::new(u8::from(dest.value) | 0b100),
                "D" => dest.value = u3::new(u8::from(dest.value) | 0b010),
                "M" => dest.value = u3::new(u8::from(dest.value) | 0b001),
                _ => return Err(crate::parse_error::ParseError::IllegalDestination),
            }
        }
        Ok(dest)
    }
}
