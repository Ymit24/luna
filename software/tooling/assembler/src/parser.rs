use common::instructions::{AddressInstruction, AluFlags, ComputeInstruction, Instruction, Opcode};
use ux::u15;

use crate::{
    lexer::Lexer,
    token::{Register, Token},
};

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
        loop {
            match &self.peek.clone() {
                Token::Dollar => {
                    self.parse_label_instruction();
                }
                Token::At => {
                    self.parse_address_instruction();
                }
                Token::Register(reg) => {
                    let destination_registers = self.parse_destination_registers(reg);
                    self.take(Token::Equal);

                    let operand1 = self.parse_register();
                    if operand1 == Register::One || operand1 == Register::Void {
                        panic!("Operand 1 must not be register One or register Void.");
                    }

                    let (operator, operand2) = if self.peek_operator() {
                        let operator = self.parse_operator();
                        let operand2 = self.parse_register();
                        (operator, operand2)
                    } else {
                        (Token::And, operand1.clone())
                    };

                    let (zx, zy, no) = match operand2 {
                        Register::Void => (false, true, false),
                        Register::One => (false, false, false),
                        _ => (false, false, false),
                    };
                    println!("finished parsing instructions.");
                    self.instructions
                        .push(Instruction::Compute(ComputeInstruction {
                            destination: match destination_registers.first() {
                                None => panic!("Expected at least one destination register."),
                                Some(reg) => match reg {
                                    Register::A => common::instructions::Destination::Address,
                                    Register::D => common::instructions::Destination::Data,
                                    Register::M => common::instructions::Destination::Memory,
                                    _ => panic!("Illegal destination register. Expected either address, data, or memory"),
                                },
                            },
                            operand1: match operand1 {
                                    Register::A => common::instructions::Register::Address,
                                    Register::D => common::instructions::Register::Data,
                                    Register::M => common::instructions::Register::Memory,
                                    Register::One => common::instructions::Register::One,
                                    _ => panic!("Illegal register. Expected either address, data, memory, or One"),
                            },
                            operand2:  match operand2 {
                                    Register::A => common::instructions::Register::Address,
                                    Register::D => common::instructions::Register::Data,
                                    Register::M => common::instructions::Register::Memory,
                                    Register::One => common::instructions::Register::One,
                                    _ => panic!("Illegal register. Expected either address, data, memory, or One"),
                            },
                            alu_flags: AluFlags{
                                zero_x: zx,
                                zero_y: zy,
                                negate_output: no,
                            },
                            opcode: match operator {
                                Token::Add => Opcode::Add,
                                Token::Sub => Opcode::Sub,
                                Token::And => Opcode::And,
                                Token::Or => Opcode::Or,
                                _ => panic!("Unexpected operator token. Expected one of 'add, sub, and, or'"),
                            },
                            jump_condition: common::instructions::jump_condition::JumpCondition::Never,
                        }));
                }
                Token::EOF => break,
                other => {
                    println!("Unexpected token: {:?}", other);
                    break;
                }
            };
        }
        self.instructions.clone()
    }

    fn parse_operator(&mut self) -> Token {
        let operator = self.peek.clone();
        self.take(operator.clone());

        operator
    }

    fn peek_operator(&mut self) -> bool {
        match &self.peek {
            Token::Add | Token::Sub | Token::And | Token::Or => true,
            _ => false,
        }
    }

    fn parse_destination_registers(&mut self, reg: &Register) -> Vec<Register> {
        let first_destination_register = reg;
        let mut destination_registers: Vec<Register> = vec![first_destination_register.clone()];

        self.take(Token::Register(first_destination_register.clone()));

        while self.peek == Token::Comma {
            self.take(Token::Comma);
            let destination_register = self.parse_register();
            destination_registers.push(destination_register.clone());
        }

        destination_registers
    }

    fn parse_register(&mut self) -> Register {
        let register = match self.peek.clone() {
            Token::Register(reg) => reg,
            Token::Literal(1) => Register::One,
            _ => panic!("Expected register token."),
        };

        self.take(self.peek.clone());
        register
    }

    fn parse_address_instruction(&mut self) {
        self.take(Token::At);
        let literal = match &self.peek {
            Token::Literal(literal) => literal,
            _ => panic!("Expected literal!"),
        }
        .clone();

        self.take(Token::Literal(literal.clone()));

        self.instructions
            .push(Instruction::Address(AddressInstruction {
                addr: u15::try_from(literal).expect("Literal value doesn't fit into u15!"),
            }));
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
    use common::instructions::{
        jump_condition::JumpCondition, AddressInstruction, AluFlags, ComputeInstruction,
        Destination, Opcode, Register,
    };

    use super::*;

    #[test]
    fn test_parse() {
        let input = "$main: @10 @2 D=A";
        let expected: Vec<Instruction> = vec![
            Instruction::Label("main".into()),
            Instruction::Address(AddressInstruction { addr: 10.into() }),
            Instruction::Address(AddressInstruction { addr: 2.into() }),
            Instruction::Compute(ComputeInstruction {
                destination: Destination::Data,
                operand1: Register::Address,
                operand2: Register::Address,
                alu_flags: AluFlags {
                    zero_x: false,
                    zero_y: false,
                    negate_output: false,
                },
                opcode: Opcode::And,
                jump_condition: JumpCondition::Never,
            }),
        ];

        let mut parser = Parser::new(Lexer::new(input));

        let result = parser.parse();
        assert_eq!(result, expected);
    }
}
