use clap::Parser;
use std::fs;

use compute_instruction::ComputeInstruction;

use crate::{
    destination::Destination, instruction::Instruction, jump_condition::JumpCondition,
    operation::Operation, parse_error::ParseError,
};

mod compute_instruction;
mod destination;
mod instruction;
mod jump_condition;
mod operation;
mod parse_error;

#[derive(Parser, Debug)]
#[command(author, version, about, long_about = None)]
struct Args {
    source_filepath: String,
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

    let lines: Vec<String> = contents.split_terminator("\n").map(Into::into).collect();
    println!("has {} lines", lines.len());

    let mut output: Vec<u16> = vec![];
    for line in lines.iter() {
        match Instruction::try_from(line.clone()) {
            Ok(instr) => {
                let instr = u16::from(instr);
                println!("{:?} -> {:04X}", line, instr);
                output.push(instr);
            }
            Err(ParseError::SkipComment | ParseError::EndOfInput) => {}
            Err(e) => eprintln!("skipping '{}', error: '{:?}'", line, e),
        };
    }

    println!("Output:");
    for out in output {
        println!("{:04X}", out);
    }
}
