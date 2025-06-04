mod lexer;
mod token;

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let source_code = include_str!("../examples/example.shd");

    println!("source: {}", source_code);

    Ok(())
}
