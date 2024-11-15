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

impl Into<(u16, u16)> for Register {
    fn into(self) -> (u16, u16) {
        match self {
            Self::A => (0, 0),
            Self::D => (0, 1),
            Self::M => (1, 0),
            Self::One => (1, 1),
        }
    }
}

#[derive(Debug)]
enum Instruction {
    Address(AddressInstruction),
    Compute(ComputeInstruction),
}

impl Into<u16> for AddressInstruction {
    fn into(self) -> u16 {
        return (1 << 15) | self.addr;
    }
}

impl Into<(u16, u16, u16)> for JumpCondition {
    fn into(self) -> (u16, u16, u16) {
        match self {
            Self::Never => (0, 0, 0),
            Self::Greaterthan => (0, 0, 1),
            Self::Equal => (0, 1, 0),
            Self::Greaterthanequals => (0, 1, 1),
            Self::Lessthan => (1, 0, 0),
            Self::Notequal => (1, 0, 1),
            Self::Lessthanequal => (1, 1, 0),
            Self::Unconditional => (1, 1, 1),
        }
    }
}

impl Into<u16> for ComputeInstruction {
    fn into(self) -> u16 {
        let reg1: (u16, u16) = self.operand1.into();
        let reg2: (u16, u16) = self.operand2.into();
        let op: (u16, u16, u16) = self.jump_condition.into();
        (if self.dest_addr { 1 } else { 0 } << 14)
            | (if self.dest_data { 1 } else { 0 } << 13)
            | (if self.dest_memory { 1 } else { 0 } << 12)
            | reg1.1 << 11
            | reg1.0 << 10
            | reg2.1 << 9
            | reg2.0 << 8
            | (if self.zero_x { 1 } else { 0 } << 7)
            | (if self.zero_y { 1 } else { 0 } << 6)
            | (if self.negate_output { 1 } else { 0 } << 5)
            | (if self.is_arith_instr_not_logic { 1 } else { 0 } << 4)
            | (if self.op_code { 1 } else { 0 } << 3)
            | op.2 << 2
            | op.1 << 1
            | op.0
    }
}

impl Into<u16> for Instruction {
    fn into(self) -> u16 {
        match self {
            Self::Address(addr) => addr.into(),
            Self::Compute(comp) => comp.into(),
        }
    }
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

fn assembleLine(line: String) -> Instruction {
    println!("\tAssembling line '{}'..", line);
    if line.contains("@") {
        println!("Found address type Instruction");
        return Instruction::Address(AddressInstruction {
            addr: line[1..].parse::<u16>().unwrap(),
        });
    }
    println!("Found compute type Instruction");
    let segments: Vec<String> = line.split("=").map(Into::into).collect();
    assert_eq!(segments.len(), 2);
    let destFlags = parseDestination(segments[0].clone());
    println!("Found df: {:?}", destFlags);
    let subsegments: Vec<String> = segments[1]
        .clone()
        .split(";")
        .map(Into::into)
        .map(|s: String| s.replace(" ", ""))
        .collect();
    let compStr = subsegments.first().clone().unwrap();

    println!("CompStr: {:?}", compStr);

    let isBinOp = compStr.contains("+")
        || compStr.contains("-")
        || compStr.contains("&")
        || compStr.contains("|");

    let opcode = if isBinOp {
        match &compStr[1..2] {
            "+" => (false, false),
            "-" => (false, true),
            "&" => (true, false),
            "|" => (true, true),
            _ => unreachable!(),
        }
    } else {
        (false, false)
    };

    let (reg1, reg2, zx, zy, no, opcode) = if isBinOp {
        let reg1 = match &compStr[0..1] {
            "A" => Register::A,
            "D" => Register::D,
            "M" => Register::M,
            "1" => Register::One,
            _ => unreachable!(),
        };
        let reg2 = match &compStr[2..3] {
            "A" => Register::A,
            "D" => Register::D,
            "M" => Register::M,
            "1" => Register::One,
            _ => unreachable!(),
        };

        println!("Regs: {:?}, {:?}", reg1, reg2);
        (reg1, reg2, false, false, false, opcode)
    } else {
        let reg = match &compStr[0..1] {
            "A" => Register::A,
            "D" => Register::D,
            "M" => Register::M,
            "1" => Register::One,
            _ => unreachable!(),
        };
        println!("Reg: {:?}", reg);
        (reg, Register::One, false, true, false, (false, false))
    };

    println!(
        "Reg1: {:?}, Reg2: {:?}, Zx: {}, Zy: {}, No: {}, AL: {}, Op: {}",
        reg1, reg2, zx, zy, no, opcode.0, opcode.1
    );

    let jmpCnd = if subsegments.len() == 1 {
        println!("Has no jump");
        JumpCondition::Never
    } else {
        println!("Has a jump");
        let jmpStr = subsegments[1].clone();
        println!("Jmp Str: {}", jmpStr);
        match jmpStr.as_str() {
            "JMP" => JumpCondition::Unconditional,
            "JEQ" => JumpCondition::Equal,
            "JNQ" => JumpCondition::Notequal,
            "JLT" => JumpCondition::Lessthan,
            "JLQ" => JumpCondition::Lessthanequal,
            "JGT" => JumpCondition::Greaterthan,
            "JGQ" => JumpCondition::Greaterthanequals,
            _ => unreachable!(),
        }
    };

    Instruction::Compute(ComputeInstruction {
        dest_addr: destFlags.0,
        dest_data: destFlags.1,
        dest_memory: destFlags.2,
        operand1: reg1,
        operand2: reg2,
        zero_x: zx,
        zero_y: zy,
        negate_output: no,
        is_arith_instr_not_logic: opcode.1,
        op_code: opcode.0,
        jump_condition: jmpCnd,
    })
}

/// Parse destination string like 'A' or 'A,B' into dest flags
fn parseDestination(destination: String) -> (bool, bool, bool) {
    (
        destination.contains("A"),
        destination.contains("D"),
        destination.contains("M"),
    )
}

fn main() {
    println!("Starting assembly...");
    let trials = ["@15", "D=A", "A=D+1", "_ = D ; JMP", "_=D; JEQ"];
    let mut output: Vec<u16> = Vec::new();
    for trial in trials.into_iter() {
        let result = assembleLine(trial.into());
        println!("Got: {:?}", result);
        let bin: u16 = result.into();
        output.push(bin);
        println!("Binary:({}) {:#018b}\t{:#06x}", bin, bin, bin);
    }

    println!("output binary:");
    for bin in &output {
        println!("{:#04x}", bin);
    }

    let input = include_str!("../test.shadow");

    output.clear();

    println!("\n\n\n\n\n\n\n\noutput binary 2:");
    for line in input.split("\n") {
        let assembled = assembleLine(line.into());
        println!("Got: {:?}", assembled);
        let bin: u16 = assembled.into();
        output.push(bin);
        println!("Binary:({}) {:#018b}\t{:#06x}", bin, bin, bin);
    }

    println!("output binary 2:");
    for bin in output {
        println!("{:#04x}", bin);
    }
}

// 1000 0000 0000 1111
// 0010 0011 0100 0000
// 0100 1011 0000 0000
