use crate::ast::{Expr, Operator};
use crate::lexer::Lexer;
use crate::token::Token;

pub struct Parser {
    lexer: Lexer,
    current_token: Option<Token>,
    peek_token: Option<Token>,
}

impl Parser {
    pub fn new(lexer: Lexer) -> Self {
        let mut parser = Self {
            lexer,
            current_token: None,
            peek_token: None,
        };
        parser.next_token();
        parser.next_token();
        parser
    }

    fn next_token(&mut self) {
        self.current_token = self.peek_token.take();
        self.peek_token = self.lexer.next();
    }

    pub fn parse(&mut self) -> Option<Expr> {
        let mut statements = Vec::new();
        loop {
            match self.current_token {
                Some(Token::Int) => {
                    self.next_token();
                    if let Some(Token::Ident(name)) = &self.current_token {
                        let name = name.clone();
                        self.next_token();
                        if let Some(Token::Assign) = &self.current_token {
                            self.next_token();
                            let value = self.parse_expression(0)?;
                            statements.push(Expr::VarDecl {
                                name,
                                value: Box::new(value),
                            });
                        } else {
                            return None;
                        }
                    } else {
                        return None;
                    }
                }
                Some(Token::Ident(_)) | Some(Token::Literal(_)) | Some(Token::LeftParen) => {
                    let expr = self.parse_expression(0)?;
                    statements.push(expr);
                }
                _ => break,
            }
        }
        if statements.is_empty() {
            None
        } else if statements.len() == 1 {
            Some(statements.remove(0))
        } else {
            Some(Expr::Statements(statements))
        }
    }

    fn parse_expression(&mut self, precedence: u8) -> Option<Expr> {
        let mut left = self.parse_prefix()?;

        while let Some(token) = &self.current_token {
            let token_precedence = self.get_token_precedence(token);
            if token_precedence < precedence {
                break;
            }

            match token {
                Token::Plus | Token::Minus => {
                    let operator = match token {
                        Token::Plus => Operator::Plus,
                        Token::Minus => Operator::Minus,
                        _ => unreachable!(),
                    };
                    self.next_token();
                    let right = self.parse_expression(token_precedence)?;
                    left = Expr::Binary {
                        left: Box::new(left),
                        operator,
                        right: Box::new(right),
                    };
                }
                _ => break,
            }
        }

        Some(left)
    }

    fn parse_prefix(&mut self) -> Option<Expr> {
        match self.current_token.take()? {
            Token::Literal(value) => {
                self.next_token();
                Some(Expr::Literal(value))
            }
            Token::LeftParen => {
                self.next_token();
                let expr = self.parse_expression(0)?;
                if let Some(Token::RightParen) = self.current_token {
                    self.next_token();
                    Some(expr)
                } else {
                    None // Missing closing parenthesis
                }
            }
            Token::Ident(name) => {
                self.next_token();
                Some(Expr::VarUsage(name))
            }
            _ => None,
        }
    }

    fn get_token_precedence(&self, token: &Token) -> u8 {
        match token {
            Token::Plus | Token::Minus => 1,
            _ => 0,
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_parse_literal() {
        let lexer = Lexer::new("42");
        let mut parser = Parser::new(lexer);
        let expr = parser.parse().expect("Expected to parse expression");
        assert!(matches!(expr, Expr::Literal(42)));
    }

    #[test]
    fn test_parse_addition() {
        let lexer = Lexer::new("1 + 2");
        let mut parser = Parser::new(lexer);
        let expr = parser.parse().expect("Expected to parse expression");
        match expr {
            Expr::Binary { left, operator, right } => {
                assert!(matches!(*left, Expr::Literal(1)));
                assert_eq!(operator, Operator::Plus);
                assert!(matches!(*right, Expr::Literal(2)));
            }
            _ => panic!("Expected binary expression"),
        }
    }

    #[test]
    fn test_parse_subtraction() {
        let lexer = Lexer::new("10 - 5");
        let mut parser = Parser::new(lexer);
        let expr = parser.parse().expect("Expected to parse expression");
        match expr {
            Expr::Binary { left, operator, right } => {
                assert!(matches!(*left, Expr::Literal(10)));
                assert_eq!(operator, Operator::Minus);
                assert!(matches!(*right, Expr::Literal(5)));
            }
            _ => panic!("Expected binary expression"),
        }
    }

    #[test]
    fn test_parse_parentheses() {
        let lexer = Lexer::new("(1 + 2) - 3");
        let mut parser = Parser::new(lexer);
        let expr = parser.parse().expect("Expected to parse expression");
        match expr {
            Expr::Binary { left, operator, right } => {
                match *left {
                    Expr::Binary { left, operator: op, right: r } => {
                        assert!(matches!(*left, Expr::Literal(1)));
                        assert_eq!(op, Operator::Plus);
                        assert!(matches!(*r, Expr::Literal(2)));
                    }
                    _ => panic!("Expected nested binary expression"),
                }
                assert_eq!(operator, Operator::Minus);
                assert!(matches!(*right, Expr::Literal(3)));
            }
            _ => panic!("Expected binary expression"),
        }
    }

    #[test]
    fn test_parse_nested_parentheses() {
        let lexer = Lexer::new("((1 + 2) - 3) + 4");
        let mut parser = Parser::new(lexer);
        let expr = parser.parse().expect("Expected to parse expression");
        match expr {
            Expr::Binary { left, operator, right } => {
                match *left {
                    Expr::Binary { left, operator: op, right: r } => {
                        match *left {
                            Expr::Binary { left, operator: op2, right: r2 } => {
                                assert!(matches!(*left, Expr::Literal(1)));
                                assert_eq!(op2, Operator::Plus);
                                assert!(matches!(*r2, Expr::Literal(2)));
                            }
                            _ => panic!("Expected nested binary expression"),
                        }
                        assert_eq!(op, Operator::Minus);
                        assert!(matches!(*r, Expr::Literal(3)));
                    }
                    _ => panic!("Expected nested binary expression"),
                }
                assert_eq!(operator, Operator::Plus);
                assert!(matches!(*right, Expr::Literal(4)));
            }
            _ => panic!("Expected binary expression"),
        }
    }

    #[test]
    fn test_parse_var_decl() {
        let lexer = Lexer::new("int a = 5");
        let mut parser = Parser::new(lexer);
        let expr = parser.parse().expect("Expected to parse var decl");
        match expr {
            Expr::VarDecl { name, value } => {
                assert_eq!(name, "a");
                assert!(matches!(*value, Expr::Literal(5)));
            }
            _ => panic!("Expected VarDecl"),
        }
    }

    #[test]
    fn test_parse_var_usage() {
        let lexer = Lexer::new("a + b");
        let mut parser = Parser::new(lexer);
        let expr = parser.parse().expect("Expected to parse var usage");
        match expr {
            Expr::Binary { left, operator, right } => {
                assert!(matches!(*left, Expr::VarUsage(ref name) if name == "a"));
                assert_eq!(operator, Operator::Plus);
                assert!(matches!(*right, Expr::VarUsage(ref name) if name == "b"));
            }
            _ => panic!("Expected binary expression"),
        }
    }
}
