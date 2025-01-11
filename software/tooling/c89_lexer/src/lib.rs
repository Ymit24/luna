mod token;

use chumsky::prelude::*;
use token::Token;

fn lexer() -> impl Parser<char, Vec<Token>, Error = Simple<char>> {
    let base_keywords = [
        "auto", "double", "int", "struct", "break", "else", "long", "switch", "case", "enum",
        "register", "typedef", "char", "extern", "return", "union", "const", "float", "short",
        "unsigned", "continue", "for", "signed", "void", "default", "goto", "sizeof", "volatile",
        "do", "if", "static", "while",
    ];
    let operators = [
        "[", "]", "(", ")", ".", "->", "++", "--", "&", "*", "+", "-", "~", "!", "sizeof", "/",
        "%", "<<", ">>", "<", ">", "<=", ">=", "==", "!=", "^", "|", "&&", "||", "?", ":", "=",
        "*=", "/=", "%=", "+=", "-=", "<<=", ">>=", "&=", "^=", "|=", ",", "#", "##",
    ];

    let punctuators = [
        "[", "]", "(", ")", "{", "}", "*", ",", ":", "=", ";", "...", "#",
    ];

    let keyword = choice(base_keywords.map(|keyword| text::keyword(keyword).to(Token::Keyword)));
    let operator = choice(operators.map(|operator| just(operator).to(Token::Operator)));
    let punctuator = choice(punctuators.map(|punctuator| just(punctuator).to(Token::Punctuator)));

    let identifier = text::ident().to(Token::Identifier);

    let constant = text::int(10).to(Token::Constant);
    let string_literal = none_of('"')
        .repeated()
        .delimited_by(just('"'), just('"'))
        .map(|_| Token::StringLiteral);

    choice((
        keyword,
        identifier,
        constant,
        string_literal,
        operator,
        punctuator,
    ))
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

    #[test]
    fn test_identifier() {
        let source = "abc123 the_name hey";
        let results = lexer().parse(source);
        assert_eq!(
            results,
            Ok(vec![
                Token::Identifier,
                Token::Identifier,
                Token::Identifier
            ])
        );
    }

    #[test]
    fn test_constant() {
        let source = "1 399 835";
        let results = lexer().parse(source);
        assert_eq!(
            results,
            Ok(vec![Token::Constant, Token::Constant, Token::Constant])
        );
    }

    #[test]
    fn test_string_literal() {
        let source = "\"foo\" \"bar bar '\"";
        let results = lexer().parse(source);
        assert_eq!(
            results,
            Ok(vec![Token::StringLiteral, Token::StringLiteral])
        );
    }

    #[test]
    fn test_operator() {
        let source = "+ = - &&";
        let results = lexer().parse(source);
        assert_eq!(
            results,
            Ok(vec![
                Token::Operator,
                Token::Operator,
                Token::Operator,
                Token::Operator,
                Token::Operator
            ])
        );
    }

    #[test]
    /// NOTE: This doesn't check that it lex's correctly or is a valid program, just that it
    /// doesn't "fail"
    fn test_basic_source() {
        let sources = [
            "int main(char argc, char** argz) { printf(\"Hello World!\"); }",
            "struct foo { char asdf; };",
        ];
        for source in sources.into_iter() {
            assert!(lexer().parse(source).is_ok());
        }
    }
}
