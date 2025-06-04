use crate::{lexer::Lexer, token::Token};

mod lexer;
mod token;
mod parser;
mod ast;

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let source_code = include_str!("../examples/example.shd");

    println!("source: {}", source_code);

    println!("lexed:");

    let mut lexer = Lexer::new(source_code);
    loop {
        let token = lexer.next();
        if token == Token::EOF {
            println!("End of file.");
            break;
        }

        println!("\t{:?}", token);
    }

    Ok(())
}
