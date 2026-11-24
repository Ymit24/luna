#[derive(Debug)]
struct ComputeInstruction {
    dest_addr: bool,
    dest_data: bool,
    dest_memory: bool,

    operand1: Register,
    operand2: Register,

    zero_x: bool,
    zero_y: bool,

    negate_output: bool,

    is_arith_instr_not_logic: bool,

    op_code: bool,

    jump_condition: JumpCondition,
}

#[derive(Debug)]
struct AddressInstruction {
    addr: u16, // NOTE: This is actually u15 in hardware
}

#[derive(Debug)]
enum JumpCondition {
    Never,
    Greaterthan,
    Equal,
    Greaterthanequals,
    Lessthan,
    Notequal,
    Lessthanequal,
    Unconditional,
}

#[derive(Debug)]
enum Register {
    A,
    D,
    M,
    One,
}

#[derive(Debug)]
enum Instruction {
    Address(AddressInstruction),
    Compute(ComputeInstruction),
}

fn extract_bit(raw: u16, location: u8) -> u16 {
    (raw & (1 << location)) >> location
}

impl From<u16> for ComputeInstruction {
    fn from(value: u16) -> Self {
        Self {
            dest_addr: extract_bit(value, 14) == 1,
            dest_data: extract_bit(value, 13) == 1,
            dest_memory: extract_bit(value, 12) == 1,

            operand1: Register::from((extract_bit(value, 11), extract_bit(value, 10))),
            operand2: Register::from((extract_bit(value, 9), extract_bit(value, 8))),

            zero_x: extract_bit(value, 7) == 1,
            zero_y: extract_bit(value, 6) == 1,
            negate_output: extract_bit(value, 5) == 1,
            is_arith_instr_not_logic: extract_bit(value, 4) == 0,
            op_code: extract_bit(value, 3) == 1,
            jump_condition: JumpCondition::from((
                extract_bit(value, 2),
                extract_bit(value, 1),
                extract_bit(value, 0),
            )),
        }
    }
}

impl From<(u16, u16, u16)> for JumpCondition {
    fn from(value: (u16, u16, u16)) -> Self {
        match value {
            (0, 0, 0) => JumpCondition::Never,
            (0, 0, 1) => JumpCondition::Greaterthan,
            (0, 1, 0) => JumpCondition::Equal,
            (1, 0, 0) => JumpCondition::Lessthan,
            (1, 1, 0) => JumpCondition::Lessthanequal,
            (1, 0, 1) => JumpCondition::Notequal,
            (0, 1, 1) => JumpCondition::Greaterthanequals,
            (1, 1, 1) => JumpCondition::Unconditional,
            _ => unreachable!(),
        }
    }
}
impl From<(u16, u16)> for Register {
    fn from(value: (u16, u16)) -> Self {
        match value {
            (0, 0) => Register::A,
            (0, 1) => Register::D,
            (1, 0) => Register::M,
            (1, 1) => Register::One,
            _ => unreachable!(),
        }
    }
}
impl From<u16> for AddressInstruction {
    fn from(value: u16) -> Self {
        Self {
            addr: 0x7fff & value,
        }
    }
}

impl From<u16> for Instruction {
    fn from(value: u16) -> Self {
        let first_bit = value & (1 << 15);
        match first_bit >> 15 {
            0 => Instruction::Compute(ComputeInstruction::from(value)),
            1 => Instruction::Address(AddressInstruction::from(value)),
            _ => unreachable!(),
        }
    }
}

fn run(program: Vec<Instruction>) {
    let mut areg = 0i16;
    let mut dreg = 0i16;
    let mut mreg = 0i16;
    let mut ram = [0i16; 65535];

    let mut pc = 0u16;
    while (pc as usize) < program.len() {
        let instr = &program[pc as usize];
        match instr {
            Instruction::Compute(details) => {
                println!("Found compute instr. {:?}", details);
                let operand1 = if details.zero_x {
                    0
                } else {
                    match details.operand1 {
                        Register::A => areg,
                        Register::D => dreg,
                        Register::M => mreg,
                        Register::One => 1,
                    }
                };
                let operand2 = if details.zero_y {
                    0
                } else {
                    match details.operand2 {
                        Register::A => areg,
                        Register::D => dreg,
                        Register::M => mreg,
                        Register::One => 1,
                    }
                };
                let result = match details.is_arith_instr_not_logic {
                    true => match details.op_code {
                        true => operand1 - operand2,
                        false => operand1 + operand2,
                    },
                    false => match details.op_code {
                        true => operand1 | operand2,
                        false => operand1 & operand2,
                    },
                };
                if details.dest_addr {
                    areg = result;
                }
                if details.dest_data {
                    dreg = result;
                }
                if details.dest_memory {
                    println!("Result: {}", result);
                    ram[areg as usize] = result;
                    mreg = ram[areg as usize];
                }
                let is_neg = result < 0;
                let is_eq = result == 0;
                let should_jump = match details.jump_condition {
                    JumpCondition::Never => false,
                    JumpCondition::Equal => is_eq,
                    JumpCondition::Lessthan => is_neg,
                    JumpCondition::Notequal => !is_eq,
                    JumpCondition::Greaterthan => !is_neg,
                    JumpCondition::Lessthanequal => is_neg || is_eq,
                    JumpCondition::Unconditional => true,
                    JumpCondition::Greaterthanequals => !is_neg || is_eq,
                };

                if should_jump {
                    pc = areg as u16;
                }
            }
            Instruction::Address(details) => {
                println!("Found address instr. {:?}", details);
                areg = details.addr as i16;
                mreg = ram[areg as usize];
            }
        }
        println!(
            "Register: A({}), D({}), M({}), PC({})",
            areg, dreg, mreg, pc
        );
        pc += 1;
    }
}

fn parse(instructions: Vec<u16>) -> Vec<Instruction> {
    instructions.into_iter().map(Instruction::from).collect()
}

fn main() {
    run(parse(vec![
        0b1000_0000_0000_1111, // 0x800F : A = 15
        0b0010_0000_0100_0000, // 0x2040 : D = A
        0b0010_0111_0000_0000, // 0x2700 : D = D + 1
        0b1000_0000_0000_0000, // 0x8000 : A = 0
        0b0000_0000_0000_0111, // 0x0007 : JMP

                               // // --- setup M(2)=5
                               // 0b1000_0000_0000_0010,
                               // 0b0010_0111_0000_0000,
                               // 0b0010_0111_0000_0000,
                               // 0b0010_0111_0000_0000,
                               // 0b0010_0111_0000_0000,
                               // 0b0010_0111_0000_0000,
                               // 0b0001_0111_0100_0000,
                               // // --- reset D=0
                               // 0b0010_0111_1100_0000,
                               // // ---
                               // 0b1000_0000_0000_1111,
                               // 0b0100_0011_0000_0000,
                               // 0b0100_0011_0000_0000,
                               // 0b0010_0111_0000_0000,
                               // 0b0010_0111_0000_0000,
                               // 0b1000_0000_0000_0010,
                               // 0b0010_0110_0000_1000,
                               // // 0b0000_0111_0100_0000,
                               // 0b1000_0000_0000_1000,
                               // 0b0000_0100_0100_0100,
                               // // 0b0010_0100_0000_1000,
    ]));
}
