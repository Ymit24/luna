use std::fs;

use clap::Parser;

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

    println!("contents: \n{}", contents);
}
