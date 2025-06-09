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
        match self.source.as_bytes().get(self.position) {
            None => return None,
            Some(c) => Some(*c as char),
        }
    }
}

impl Iterator for Lexer {
    type Item = Token;

    fn next(&mut self) -> Option<Self::Item> {
        let current = match self.curr() {
            None => return Some(Token::EOF),
            Some(x) => x,
        };
        match current {
            '+' => Some(Token::Plus),
            '-' => Some(Token::Minus),
            '0'..='9' => {
                let mut literal = String::new();
                while let Some(curr) = self.curr() {
                    if !curr.is_digit(10) {
                        break;
                    }
                    literal.push(curr);
                    self.position += 1;
                }
                Some(Token::Literal(literal.parse::<u16>().ok()?))
            }
            _ => Some(Token::Illegal),
        }
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
        let mut lexer = Lexer::new("10+2-10");
        assert_eq!(
            vec![
                Token::Literal(10),
                Token::Plus,
                Token::Literal(2),
                Token::Minus,
                Token::Literal(10)
            ],
            lexer
                .take_while(|x| *x != Token::EOF)
                .collect::<Vec<Token>>()
        );
    }
}
