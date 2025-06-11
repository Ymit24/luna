use clap::Parser;
use std::{fs, path::PathBuf};
use thiserror::Error;

use crate::{
    instruction::Instruction,
    parse_error::ParseError,
};

mod compute_instruction;
mod destination;
mod instruction;
mod jump_condition;
mod operation;
mod parse_error;

/// Command line arguments for the assembler
#[derive(Parser, Debug)]
#[command(author, version, about, long_about = None)]
struct Args {
    /// Path to the source file to assemble
    source_filepath: PathBuf,
}

/// Custom error type for the assembler
#[derive(Error, Debug)]
pub enum AssemblerError {
    #[error("Failed to read source file: {0}")]
    FileReadError(#[from] std::io::Error),
    
    #[error("Failed to parse instruction: {0}")]
    ParseError(#[from] ParseError),
}

/// Result type for the assembler
type Result<T> = std::result::Result<T, AssemblerError>;

/// Main assembler function that processes a source file and returns the assembled instructions
fn assemble_file(source_path: &PathBuf) -> Result<Vec<u16>> {
    let contents = fs::read_to_string(source_path)?;
    let lines: Vec<String> = contents.lines().map(String::from).collect();
    
    let mut output = Vec::new();
    for line in lines {
        match Instruction::try_from(line.clone()) {
            Ok(instr) => {
                let instr = u16::from(instr);
                println!("{:?} -> {:04X}", line, instr);
                output.push(instr);
            }
            Err(ParseError::SkipComment | ParseError::EndOfInput) => {}
            Err(e) => eprintln!("skipping '{}', error: '{:?}'", line, e),
        }
    }
    
    Ok(output)
}

fn main() -> Result<()> {
    let args = Args::parse();
    
    println!("Processing file: {}", args.source_filepath.display());
    
    let output = assemble_file(&args.source_filepath)?;
    
    println!("\nOutput:");
    for out in output {
        println!("{:04X}", out);
    }
    
    Ok(())
}
