use std::{error::Error, fs};

use clap::Parser;
use ux::{u15, u3, u6, u7};

#[derive(Parser, Debug)]
#[command(author, version, about, long_about = None)]
struct Args {
    source_filepath: String,
}

#[derive(Debug)]
struct ComputeInstruction {
    destination: Destination, // for now just single destination
    operation: Operation,
    jump_condition: JumpCondition,
}

impl From<ComputeInstruction> for u16 {
    fn from(value: ComputeInstruction) -> Self {
        let top_mask = 0b1110_0000_0000_0000;
        let dest_mask: u16 = u16::from(u3::from(value.destination)) << 3;
        let jmp_mask: u16 = u16::from(u3::from(value.jump_condition));
        let op_mask: u16 = u16::from(u7::from(value.operation)) << 6;
        top_mask | op_mask | dest_mask | jmp_mask
    }
}

enum ParseError {
    NotComputeInstruction,
}

fn remove_all_whitespace<T: Into<String>>(s: T) -> String {
    s.into().chars().filter(|c| !c.is_whitespace()).collect()
}

impl From<Instruction> for u16 {
    fn from(value: Instruction) -> Self {
        match value {
            Instruction::Address(addr) => addr.into(),
            Instruction::Compute(ci) => ci.into(),
        }
    }
}

impl TryFrom<String> for Instruction {
    type Error = ParseError;

    fn try_from(value: String) -> Result<Self, Self::Error> {
        if value.len() == 0 {
            return Err(ParseError::NotComputeInstruction);
        }
        if let Ok(ci) = ComputeInstruction::try_from(value.clone()) {
            return Ok(Instruction::Compute(ci));
        }
        if value.starts_with("#") {
            return Err(ParseError::NotComputeInstruction);
        }
        if value.starts_with("@") {
            let imm = &value[1..];
            let imm = match imm.parse::<u16>() {
                Err(_) => return Err(ParseError::NotComputeInstruction),
                Ok(i) => i,
            };
            let imm = match u15::try_from(imm) {
                Err(_) => return Err(ParseError::NotComputeInstruction),
                Ok(i) => i,
            };
            return Ok(Instruction::Address(imm));
        }
        Err(ParseError::NotComputeInstruction)
    }
}

impl TryFrom<String> for ComputeInstruction {
    type Error = ParseError;

    fn try_from(value: String) -> Result<Self, Self::Error> {
        if value.len() == 0 {
            return Err(ParseError::NotComputeInstruction);
        }
        let (reg_str, rem) = match value.split_once('=') {
            None => return Err(ParseError::NotComputeInstruction),
            Some(reg) => reg,
        };

        let dest = match reg_str.trim().to_uppercase().as_str() {
            "A" => Destination::A,
            "D" => Destination::D,
            "M" => Destination::M,
            "_" => Destination::Null,
            _ => return Err(ParseError::NotComputeInstruction),
        };

        let (op_str, rem) = match rem.split_once(';') {
            None => (rem, ""),
            Some(x) => x,
        };

        let opcode = match remove_all_whitespace(op_str.trim().to_uppercase()).as_str() {
            "0" => Operation::Zero,
            "1" => Operation::One,
            "-1" => Operation::NegOne,
            "D" => Operation::D,
            "A" => Operation::A,
            "!D" => Operation::NotD,
            "!A" => Operation::NotA,
            "D+1" => Operation::DPlus1,
            "A+1" => Operation::APlus1,
            "D-1" => Operation::DMin1,
            "A-1" => Operation::AMin1,
            "D+A" => Operation::DPlusA,
            "D-A" => Operation::DMinA,
            "A-D" => Operation::AMinD,
            "D&A" => Operation::DAndA,
            "D|A" => Operation::DOrA,

            "M" => Operation::M,
            "!M" => Operation::NotM,
            "-M" => Operation::NegM,
            "M+1" => Operation::MPlus1,
            "M-1" => Operation::MMin1,
            "D+M" => Operation::DPlusM,
            "D-M" => Operation::DMinM,
            "M-D" => Operation::MMinD,
            "D&M" => Operation::DAndM,
            "D|M" => Operation::DOrM,
            _ => return Err(ParseError::NotComputeInstruction),
        };

        let jmp = match remove_all_whitespace(rem.to_uppercase()).as_str() {
            "" => JumpCondition::Null,
            "JGT" => JumpCondition::JGT,
            "JEQ" => JumpCondition::JEQ,
            "JGE" => JumpCondition::JGE,
            "JLT" => JumpCondition::JLT,
            "JNE" => JumpCondition::JNE,
            "JLE" => JumpCondition::JLE,
            "JMP" => JumpCondition::JMP,
            _ => return Err(ParseError::NotComputeInstruction),
        };

        Ok(ComputeInstruction {
            destination: dest,
            operation: opcode,
            jump_condition: jmp,
        })
    }
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

// TODO: confirm these values
impl From<JumpCondition> for u3 {
    fn from(value: JumpCondition) -> Self {
        match value {
            JumpCondition::Null => u3::new(0b000),
            JumpCondition::JGT => u3::new(0b001),
            JumpCondition::JEQ => u3::new(0b010),
            JumpCondition::JGE => u3::new(0b011),
            JumpCondition::JLT => u3::new(0b100),
            JumpCondition::JNE => u3::new(0b101),
            JumpCondition::JLE => u3::new(0b110),
            JumpCondition::JMP => u3::new(0b111),
        }
    }
}

#[derive(Debug)]
enum Destination {
    Null,
    A,
    D,
    M,
}

#[derive(Debug)]
enum Operation {
    Zero,
    One,
    NegOne,
    D,
    A,
    NotD,
    NotA,
    NegD,
    NegA,
    DPlus1,
    APlus1,
    DMin1,
    AMin1,
    DPlusA,
    DMinA,
    AMinD,
    DAndA,
    DOrA,
    M,
    NotM,
    NegM,
    MPlus1,
    MMin1,
    DPlusM,
    DMinM,
    MMinD,
    DAndM,
    DOrM,
}

#[derive(Debug)]
enum JumpCondition {
    Null,
    JGT,
    JEQ,
    JGE,
    JLT,
    JNE,
    JLE,
    JMP,
}

#[derive(Debug)]
enum Instruction {
    Address(u15),
    Compute(ComputeInstruction),
}

fn main() {
    let args = Args::parse();

    let source_filepath = args.source_filepath;
    println!("name: {}", &source_filepath);

    let contents = match fs::read_to_string(source_filepath) {
        Err(_) => {
            panic!("Failed to read source file.");
        }
        Ok(contents) => contents,
    };

    println!("contents: \n{}", contents);

    let lines: Vec<String> = contents.split_terminator("\n").map(Into::into).collect();
    println!("has {} lines", lines.len());

    let instr = ComputeInstruction {
        destination: Destination::Null,
        operation: Operation::Zero,
        jump_condition: JumpCondition::JMP,
    };

    println!("instr as u16: {:04X}", u16::from(instr));

    for line in lines.iter() {
        if let Ok(instr) = Instruction::try_from(line.clone()) {
            println!("{:04X}", u16::from(instr));
        } else {
            // eprintln!("Parse error, skipping line");
        }
    }
}
