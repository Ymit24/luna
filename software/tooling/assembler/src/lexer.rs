use crate::token::{Jump, Register, Token};

pub struct Lexer {
    input: String,
    position: usize,
}

impl Lexer {
    pub fn new<T: Into<String>>(input: T) -> Self {
        Lexer {
            input: input.into(),
            position: 0,
        }
    }

    pub fn next(&mut self) -> Token {
        self.skip_whitespace();

        let found = match self.peek() {
            None => Token::EOF,
            Some(ch) => match ch {
                '@' => Token::At,
                '$' => Token::Dollar,
                ':' => Token::Colon,
                '=' => Token::Equal,
                ';' => Token::Semicolon,
                'A' => Token::Register(Register::A),
                'D' => Token::Register(Register::D),
                'M' => Token::Register(Register::M),
                '_' => Token::Register(Register::Void),
                '+' => Token::Add,
                '-' => Token::Sub,
                '&' => Token::And,
                '|' => Token::Or,
                other => {
                    if other.is_ascii_alphabetic() {
                        return self.read_identifier();
                    } else if other.is_ascii_digit() {
                        let mut literal = String::new();
                        loop {
                            match self.peek() {
                                Some(c) if c.is_ascii_digit() => {
                                    literal.push(self.read().expect("Expected an ascii digit"));
                                }
                                _ => break,
                            }
                        }
                        return Token::Literal(
                            literal
                                .parse::<u16>()
                                .expect("Failed to parse literal into u16."),
                        );
                    }
                    return Token::Illegal(other);
                }
            },
        };

        _ = self.read();
        found
    }

    fn read_identifier(&mut self) -> Token {
        let mut literal = String::new();
        loop {
            match self.peek() {
                Some(c) if c.is_ascii_alphanumeric() => {
                    literal.push(self.read().expect("Expected an ascii alphanumeric"));
                }
                _ => break,
            }
        }
        if let Some(value) = read_jump(&literal) {
            return value;
        }
        Token::LabelIdent(literal)
    }

    fn skip_whitespace(&mut self) {
        loop {
            match self.peek() {
                None => break,
                Some(c) if c.is_ascii_whitespace() => {
                    _ = self.read();
                }
                _ => break,
            }
        }
    }

    fn peek(&self) -> Option<char> {
        self.input.chars().nth(self.position)
    }

    fn read(&mut self) -> Option<char> {
        match self.peek() {
            None => None,
            Some(value) => {
                self.position += 1;
                Some(value)
            }
        }
    }
}

fn read_jump(literal: &String) -> Option<Token> {
    match literal.as_str() {
        "JMP" => Some(Token::Jump(Jump::JMP)),
        "JNE" => Some(Token::Jump(Jump::JNE)),
        "JEQ" => Some(Token::Jump(Jump::JEQ)),
        "JNQ" => Some(Token::Jump(Jump::JNQ)),
        "JGT" => Some(Token::Jump(Jump::JGT)),
        "JLT" => Some(Token::Jump(Jump::JLT)),
        "JGE" => Some(Token::Jump(Jump::JGE)),
        "JLE" => Some(Token::Jump(Jump::JLE)),
        _ => None,
    }
}

#[cfg(test)]
mod tests {
    use crate::token::Register;

    use super::*;

    #[test]
    fn test_next_token_eof() {
        let mut lexer = Lexer::new("");
        let result = lexer.next();

        assert_eq!(result, Token::EOF);
    }

    #[test]
    fn test_next_tokens() {
        let mut lexer = Lexer::new("$abc: 123 =; A D M _ 1 + - & | JMP JNE");
        let expected = [
            Token::Dollar,
            Token::LabelIdent("abc".into()),
            Token::Colon,
            Token::Literal(123),
            Token::Equal,
            Token::Semicolon,
            Token::Register(Register::A),
            Token::Register(Register::D),
            Token::Register(Register::M),
            Token::Register(Register::Void),
            Token::Literal(1),
            Token::Add,
            Token::Sub,
            Token::And,
            Token::Or,
            Token::Jump(Jump::JMP),
            Token::Jump(Jump::JNE),
            Token::EOF,
        ];

        for expect in expected {
            let result = lexer.next();

            assert_eq!(result, expect);
        }
    }
}
