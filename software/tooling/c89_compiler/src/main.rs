use crate::lexer::Lexer;

mod lexer;
mod token;

fn main() {
    println!("hello from c compiler!");

    let source = "2";

    let lexer = Lexer::new(source);
}
