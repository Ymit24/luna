mod token;

use chumsky::prelude::*;
use token::Token;

pub fn lexer() -> impl Parser<char, Vec<Token>, Error = Simple<char>> {
    let operators = [
        "[", "]", "(", ")", ".", "->", "++", "--", "&", "*", "+", "-", "~", "!", "sizeof", "/",
        "%", "<<", ">>", "<", ">", "<=", ">=", "==", "!=", "^", "|", "&&", "||", "?", ":", "=",
        "*=", "/=", "%=", "+=", "-=", "<<=", ">>=", "&=", "^=", "|=", ",", "#", "##",
    ];

    let keyword = choice([
        text::keyword("auto").to(Token::Auto),
        text::keyword("Auto").to(Token::Auto),
        text::keyword("double").to(Token::Double),
        text::keyword("int").to(Token::Int),
        text::keyword("struct").to(Token::Struct),
        text::keyword("break").to(Token::Break),
        text::keyword("else").to(Token::Else),
        text::keyword("long").to(Token::Long),
        text::keyword("switch").to(Token::Switch),
        text::keyword("case").to(Token::Case),
        text::keyword("enum").to(Token::Enum),
        text::keyword("register").to(Token::Register),
        text::keyword("typedef").to(Token::Typedef),
        text::keyword("char").to(Token::Char),
        text::keyword("extern").to(Token::Extern),
        text::keyword("return").to(Token::Return),
        text::keyword("union").to(Token::Union),
        text::keyword("const").to(Token::Const),
        text::keyword("float").to(Token::Float),
        text::keyword("short").to(Token::Short),
        text::keyword("unsigned").to(Token::Unsigned),
        text::keyword("continue").to(Token::Continue),
        text::keyword("for").to(Token::For),
        text::keyword("signed").to(Token::Signed),
        text::keyword("void").to(Token::Void),
        text::keyword("default").to(Token::Default),
        text::keyword("goto").to(Token::Goto),
        text::keyword("sizeof").to(Token::Sizeof),
        text::keyword("volatile").to(Token::Volatile),
        text::keyword("do").to(Token::Do),
        text::keyword("if").to(Token::If),
        text::keyword("static").to(Token::Static),
        text::keyword("while").to(Token::While),
    ]);

    let operator = choice([
        just("[").to(Token::LeftBracket),
        just("]").to(Token::RightBracket),
        just("(").to(Token::LeftParen),
        just(")").to(Token::RightParen),
        just("{").to(Token::LeftBrace),
        just("}").to(Token::RightBrace),
        just(".").to(Token::Dot),
        just("->").to(Token::Arrow),
        just("++").to(Token::PlusPlus),
        just("--").to(Token::MinusMinus),
        just("&&").to(Token::AndAnd),
        just("&").to(Token::Ampersand),
        just("*").to(Token::Asterisk),
        just("+").to(Token::Plus),
        just("-").to(Token::Minus),
        just("~").to(Token::Tilde),
        just("!").to(Token::Bang),
        just("/").to(Token::Slash),
        just("%").to(Token::Percent),
        just("<<").to(Token::LeftShift),
        just(">>").to(Token::RightShift),
        just("<").to(Token::Less),
        just(">").to(Token::Greater),
        just("<=").to(Token::LessEqual),
        just(">=").to(Token::GreaterEqual),
        just("==").to(Token::EqualEqual),
        just("!=").to(Token::NotEqual),
        just("^").to(Token::Caret),
        just("|").to(Token::Pipe),
        just("||").to(Token::OrOr),
        just("?").to(Token::Question),
        just(":").to(Token::Colon),
        just(";").to(Token::Semicolon),
        just("*=").to(Token::AsteriskEqual),
        just("/=").to(Token::SlashEqual),
        just("%=").to(Token::PercentEqual),
        just("+=").to(Token::PlusEqual),
        just("-=").to(Token::MinusEqual),
        just("<<=").to(Token::LeftShiftEqual),
        just(">>=").to(Token::RightShiftEqual),
        just("&=").to(Token::AmpersandEqual),
        just("^=").to(Token::CaretEqual),
        just("|=").to(Token::PipeEqual),
        just(",").to(Token::Comma),
        just("=").to(Token::Equal),
        just("#").to(Token::Hash),
        just("##").to(Token::HashHash),
        just("...").to(Token::Ellipsis),
    ]);

    let identifier = text::ident().map(Token::Identifier);

    let constant = text::int(10).to(Token::Constant);
    let string_literal = none_of('"')
        .repeated()
        .delimited_by(just('"'), just('"'))
        .map(|_| Token::StringLiteral);

    choice((keyword, identifier, constant, string_literal, operator))
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
                Token::Auto,
                Token::Double,
                Token::For,
                Token::Signed,
                Token::Void
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
                Token::Identifier("abc123".into()),
                Token::Identifier("the_name".into()),
                Token::Identifier("hey".into())
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
            Ok(vec![Token::Plus, Token::Equal, Token::Minus, Token::AndAnd,])
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
