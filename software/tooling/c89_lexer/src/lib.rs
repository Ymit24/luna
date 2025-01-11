mod token;

use chumsky::prelude::*;
use token::Token;

fn lexer() -> impl Parser<char, Vec<Token>, Error = Simple<char>> {
    let keywords = [
        "auto", "double", "int", "struct", "break", "else", "long", "switch", "case", "enum",
        "register", "typedef", "char", "extern", "return", "union", "const", "float", "short",
        "unsigned", "continue", "for", "signed", "void", "default", "goto", "sizeof", "volatile",
        "do", "if", "static", "while",
    ];

    let identifier = text::ident().to(Token::Identifier);

    let keyword_lexer = choice(keywords.map(|keyword| text::keyword(keyword).to(Token::Keyword)));
    choice((keyword_lexer, identifier))
        .padded()
        .repeated()
        .then_ignore(end())
}

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn test_keyword_lexing() {
        let source = "auto double for signed void";

        let results = lexer().parse(source);
        assert_eq!(
            results,
            Ok(vec![
                Token::Keyword,
                Token::Keyword,
                Token::Keyword,
                Token::Keyword,
                Token::Keyword
            ])
        );
    }
}
