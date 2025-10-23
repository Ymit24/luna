use crate::token::Token;

pub struct Lexer {
    source: String,
    position: usize,
}

impl Lexer {
    pub fn new(source: String) -> Self {
        Self {
            source,
            position: 0,
        }
    }

    fn peek(&self) -> Option<char> {
        if self.position >= self.source.len() {
            return None;
        }

        Some(self.source.as_bytes()[self.position] as char)
    }

    fn next_token(&mut self) -> Option<Token> {
        match self.peek() {
            None => return None,
            Some(value) => match value {
                '0'..'9' => {
                    return Some(Token::Integer { literal: 0 });
                }
                _ => {
                    return None;
                }
            },
        }
    }
}
