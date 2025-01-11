use c89_lexer::lexer;
use chumsky::prelude::*;

fn main() {
    let filename = std::env::args().nth(1).unwrap();
    let src = std::fs::read_to_string(filename.clone()).unwrap();

    println!("Lexing {}...", filename);
    let tokens = match lexer().parse(src.clone()) {
        Ok(toks) => toks,
        Err(errs) => {
            for err in errs.iter() {
                eprintln!("Failed to lex: {}", err);
            }
            panic!("Failed to lex.");
        }
    };

    println!("Lex successful, found: {:?}", tokens);

    // let tokens = match lexer().parse(src.clone()) {
    //     Ok(toks) => toks,
    //     Err(errs) => {
    //         errs.iter()
    //             .for_each(|err| display_error(err, &filename, &src));
    //         panic!("Failed to compile.");
    //     }
    // };
    // println!("Got tokens: {:?}", tokens);
}
