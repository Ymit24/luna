use std::{
    io::{self, Stdin, Stdout, Write},
};

use crate::{lexer::Lexer, parser::Parser};

mod ast;
mod lexer;
mod parser;
mod token;

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let mut stdout = &mut io::stdout();

    repl(io::stdin(), &mut stdout);

    Ok(())
}

fn parse_and_print<T: Into<String>>(source_code: T, stdout: &mut Stdout) {
    let mut parser = Parser::new(Lexer::new(source_code));

    let instructions = parser.parse();

    for instruction in instructions {
        stdout
            .write(format!("\t{:?}", instruction).as_bytes())
            .expect("Failed to write instruction out");
    }
}

fn repl(stdin: Stdin, stdout: &mut Stdout) {
    loop {
        stdout.write("\n>> ".as_bytes()).expect("Failed to write.");
        stdout.flush().expect("Failed to flush.");

        let mut input = String::new();
        stdin
            .read_line(&mut input)
            .expect("Failed to read user input.");

        if input == "quit\n" {
            break;
        }
        parse_and_print(input, stdout);
    }
}
