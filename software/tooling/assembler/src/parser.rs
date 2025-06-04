use common::instructions::Instruction;

use crate::{lexer::Lexer, token::Token};

pub struct Parser {
    lexer: Lexer,
    curr: Token,
    peek: Token,
    instructions: Vec<Instruction>,
}

impl Parser {
    pub fn new(mut lexer: Lexer) -> Self {
        let curr = lexer.next();
        let peek = curr.clone();

        Parser {
            lexer: lexer,
            instructions: Vec::new(),
            curr: curr,
            peek: peek,
        }
    }

    fn take(&mut self, token: Token) {
        if self.peek != token {
            panic!("Failed to take expected token!");
        }
        self.curr = self.peek.clone();
        self.peek = self.lexer.next();
    }

    pub fn parse(&mut self) -> Vec<Instruction> {
        println!("Peek: {:?}", self.peek);
        loop {
            match &self.peek {
                Token::Dollar => {
                    self.parse_label_instruction();
                }

                _ => break,
            };
        }
        self.instructions.clone()
    }

    fn parse_label_instruction(&mut self) {
        self.take(Token::Dollar);
        let label_ident = match &self.peek {
            Token::LabelIdent(label_ident) => label_ident,
            _ => panic!("Expected label identifier!"),
        }
        .clone();

        self.take(Token::LabelIdent(label_ident.clone()));
        self.take(Token::Colon);
        self.instructions.push(Instruction::Label(label_ident));
    }
}

#[cfg(test)]
mod tests {
    use common::instructions::AddressInstruction;

    use super::*;

    #[test]
    fn test_parse() {
        let input = "$main: @10";
        let expected: Vec<Instruction> = vec![
            Instruction::Label("main".into()),
            Instruction::Address(AddressInstruction { addr: 10.into() }),
        ];

        let mut parser = Parser::new(Lexer::new(input));

        let result = parser.parse();
        assert_eq!(result, expected);
    }
}
