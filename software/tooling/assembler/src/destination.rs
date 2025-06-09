use ux::u3;

#[derive(Debug, Clone, Copy)]
pub enum Destination {
    Null,
    A,
    D,
    M,
}

impl From<Destination> for u3 {
    fn from(value: Destination) -> Self {
        match value {
            Destination::Null => u3::new(0b000),
            Destination::A => u3::new(0b100),
            Destination::D => u3::new(0b010),
            Destination::M => u3::new(0b001),
        }
    }
}
