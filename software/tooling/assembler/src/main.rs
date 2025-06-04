use crate::{lexer::Lexer, parser::Parser, token::Token};

mod ast;
mod lexer;
mod parser;
mod token;

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let source_code = include_str!("../examples/example.shd");

    println!("source: {}", source_code);

    let mut lexer = Lexer::new(source_code);
    let mut parser = Parser::new(lexer);

    let instructions = parser.parse();

    for instruction in instructions {
        println!("\t{:?}", instruction);
    }

    Ok(())
}
