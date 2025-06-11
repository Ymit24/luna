use rustyline::error::ReadlineError;
use rustyline::Editor;
use rustyline::history::DefaultHistory;

use crate::lexer::Lexer;
use crate::parser::Parser;
use crate::codegen::CodeGenerator;

mod token;
mod lexer;
mod ast;
mod parser;
mod codegen;

fn main() {
    println!("C89 Compiler REPL");
    println!("Enter your code. Press Enter twice to compile. Press Ctrl+D (Unix) or Ctrl+Z (Windows) to exit.");
    
    let mut rl = Editor::<(), DefaultHistory>::new().expect("Failed to create editor");
    let mut input_buffer = String::new();
    
    loop {
        match rl.readline("> ") {
            Ok(line) => {
                rl.add_history_entry(line.as_str());
                let line = line.trim();
                if line.is_empty() {
                    if !input_buffer.is_empty() {
                        // Compile the entire input at once
                        let input = input_buffer.trim();
                        // Lex
                        let lexer = Lexer::new(input);
                        let tokens: Vec<_> = lexer.collect();
                        println!("Tokens: {:?}", tokens);

                        // Parse
                        let mut parser = Parser::new(Lexer::new(input));
                        match parser.parse() {
                            Some(expr) => {
                                println!("AST: {:?}", expr);

                                // Code gen
                                let mut codegen = CodeGenerator::new();
                                let asm = codegen.generate(&expr);
                                println!("\nGenerated Assembly:");
                                for line in asm {
                                    println!("{}", line);
                                }
                            }
                            None => println!("Error: Failed to parse expression"),
                        }
                        println!();
                        input_buffer.clear();
                    }
                } else {
                    input_buffer.push_str(line);
                    input_buffer.push('\n');
                }
            }
            Err(ReadlineError::Interrupted) => {
                println!("CTRL-C");
                break;
            }
            Err(ReadlineError::Eof) => {
                println!("CTRL-D");
                break;
            }
            Err(err) => {
                println!("Error: {}", err);
                break;
            }
        }
    }
}
