use crate::token::Token;

#[derive(Debug, Clone)]
pub struct Lexer {
    source: String,
    position: usize,
}

impl Lexer {
    pub fn new<T: Into<String>>(source: T) -> Self {
        Self {
            source: source.into(),
            position: 0,
        }
    }

    fn curr(&self) -> Option<char> {
        self.source[self.position..].chars().next()
    }

    fn bump(&mut self) {
        if let Some(c) = self.curr() {
            self.position += c.len_utf8();
        }
    }

    fn skip_whitespace(&mut self) {
        while let Some(c) = self.curr() {
            if !c.is_whitespace() {
                break;
            }
            self.bump();
        }
    }
}

impl Iterator for Lexer {
    type Item = Token;

    fn next(&mut self) -> Option<Self::Item> {
        self.skip_whitespace();

        let token = match self.curr()? {
            '+' => Token::Plus,
            '-' => Token::Minus,
            '(' => Token::LeftParen,
            ')' => Token::RightParen,
            '0'..='9' => {
                let mut literal = String::new();
                while let Some(curr) = self.curr() {
                    if !curr.is_ascii_digit() {
                        break;
                    }
                    literal.push(curr);
                    self.bump();
                }
                return Some(Token::Literal(literal.parse::<u16>().ok()?));
            }
            _ => Token::Illegal,
        };

        self.bump();
        Some(token)
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_lexer_new() {
        let lexer = Lexer::new("2");
        assert_eq!("2".to_string(), lexer.source);
    }

    #[test]
    fn test_next_literal() {
        let mut lexer = Lexer::new("22");
        let next = lexer.next().expect("Expected to get a token");
        assert_eq!(Token::Literal(22), next);
    }

    #[test]
    fn test_next_plus() {
        let mut lexer = Lexer::new("+");
        let next = lexer.next().expect("Expected to get a token");
        assert_eq!(Token::Plus, next);
    }

    #[test]
    fn test_next_minus() {
        let mut lexer = Lexer::new("-");
        let next = lexer.next().expect("Expected to get a token");
        assert_eq!(Token::Minus, next);
    }

    #[test]
    fn test_multiple_tokens() {
        let lexer = Lexer::new("10+2-10");
        let tokens: Vec<Token> = lexer.collect();

        assert_eq!(
            vec![
                Token::Literal(10),
                Token::Plus,
                Token::Literal(2),
                Token::Minus,
                Token::Literal(10)
            ],
            tokens
        );
    }

    #[test]
    fn test_bump() {
        let mut lexer = Lexer::new("+");
        assert_eq!(lexer.curr(), Some('+'));
        lexer.bump();
        assert_eq!(lexer.curr(), None);
    }
}
