use crate::ast::{Expr, Operator};

/// Trait for visiting AST nodes
pub trait AstVisitor {
    type Output;

    fn visit_literal(&mut self, value: u16) -> Self::Output;
    fn visit_binary(&mut self, left: &Expr, operator: &Operator, right: &Expr) -> Self::Output;
    fn visit_var_decl(&mut self, name: &str, value: &Expr) -> Self::Output;
    fn visit_var_usage(&mut self, name: &str) -> Self::Output;
    fn visit_statements(&mut self, statements: &[Expr]) -> Self::Output;
}

/// Code generator that implements the visitor pattern
pub struct CodeGenerator {
    register_counter: u8,
}

impl CodeGenerator {
    pub fn new() -> Self {
        Self {
            register_counter: 0,
        }
    }

    pub fn generate(&mut self, expr: &Expr) -> Vec<String> {
        let mut code = vec![
            "section .text".to_string(),
            "global _start".to_string(),
            "_start:".to_string(),
        ];
        
        let result = self.visit_expr(expr);
        code.extend(result);
        
        // Add exit syscall
        code.extend(vec![
            "    mov rax, 60".to_string(),  // sys_exit
            "    mov rdi, 0".to_string(),   // exit code 0
            "    syscall".to_string(),
        ]);
        
        code
    }

    fn next_register(&mut self) -> String {
        // Reset counter if we've used all registers
        if self.register_counter >= 4 {
            self.register_counter = 0;
        }
        let reg = match self.register_counter {
            0 => "rax",
            1 => "rbx",
            2 => "rcx",
            3 => "rdx",
            _ => unreachable!(),
        };
        self.register_counter += 1;
        reg.to_string()
    }

    fn visit_expr(&mut self, expr: &Expr) -> Vec<String> {
        expr.accept(self)
    }
}

impl AstVisitor for CodeGenerator {
    type Output = Vec<String>;

    fn visit_literal(&mut self, value: u16) -> Self::Output {
        let reg = self.next_register();
        vec![
            format!("    mov {}, {}", reg, value),
        ]
    }

    fn visit_binary(&mut self, left: &Expr, operator: &Operator, right: &Expr) -> Self::Output {
        let mut code = Vec::new();
        // Generate code for left operand
        let left_reg = self.next_register();
        code.extend(self.visit_expr(left));
        // Save left result
        code.push(format!("    push {}", left_reg));
        // Generate code for right operand
        let right_reg = self.next_register();
        code.extend(self.visit_expr(right));
        // Restore left result
        code.push(format!("    pop {}", left_reg));
        // Perform operation
        match operator {
            Operator::Plus => {
                code.push(format!("    add {}, {}", left_reg, right_reg));
            }
            Operator::Minus => {
                code.push(format!("    sub {}, {}", left_reg, right_reg));
            }
        }
        code
    }

    fn visit_var_decl(&mut self, name: &str, value: &Expr) -> Self::Output {
        let mut code = Vec::new();
        // Generate code for value
        let reg = self.next_register();
        code.extend(self.visit_expr(value));
        // Store value in variable
        code.push(format!("    mov [{}], {}", name, reg));
        code
    }

    fn visit_var_usage(&mut self, name: &str) -> Self::Output {
        let reg = self.next_register();
        vec![
            format!("    mov {}, [{}]", reg, name),
        ]
    }

    fn visit_statements(&mut self, statements: &[Expr]) -> Self::Output {
        let mut code = Vec::new();
        
        // Add .bss section for all variables
        code.push("section .bss".to_string());
        for stmt in statements {
            if let Expr::VarDecl { name, .. } = stmt {
                code.push(format!("{}: resq 1", name));
            }
        }
        
        // Add .text section with single _start
        code.push("section .text".to_string());
        code.push("global _start".to_string());
        code.push("_start:".to_string());
        
        // Generate code for each statement
        for stmt in statements {
            code.extend(self.visit_expr(stmt));
        }
        
        // Add single exit syscall at the end
        code.extend(vec![
            "    mov rax, 60".to_string(),  // sys_exit
            "    mov rdi, 0".to_string(),   // exit code 0
            "    syscall".to_string(),
        ]);
        
        code
    }
}

// Helper trait to make visiting expressions easier
trait Visitable {
    fn accept<V: AstVisitor>(&self, visitor: &mut V) -> V::Output;
}

impl Visitable for Expr {
    fn accept<V: AstVisitor>(&self, visitor: &mut V) -> V::Output {
        match self {
            Expr::Literal(value) => visitor.visit_literal(*value),
            Expr::Binary { left, operator, right } => visitor.visit_binary(left, operator, right),
            Expr::VarDecl { name, value } => visitor.visit_var_decl(name, value),
            Expr::VarUsage(name) => visitor.visit_var_usage(name),
            Expr::Statements(statements) => visitor.visit_statements(statements),
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::ast::{Expr, Operator};

    #[test]
    fn test_codegen_literal() {
        let mut codegen = CodeGenerator::new();
        let expr = Expr::Literal(42);
        let output = codegen.generate(&expr);
        assert!(output.iter().any(|line| line.contains("mov") && line.contains("42")));
    }

    #[test]
    fn test_codegen_addition() {
        let mut codegen = CodeGenerator::new();
        let expr = Expr::Binary {
            left: Box::new(Expr::Literal(1)),
            operator: Operator::Plus,
            right: Box::new(Expr::Literal(2)),
        };
        let output = codegen.generate(&expr);
        
        // Check that we have two mov instructions for the literals
        assert!(output.iter().any(|line| line.contains("mov") && line.contains("1")));
        assert!(output.iter().any(|line| line.contains("mov") && line.contains("2")));
        // Check for the add operation
        assert!(output.iter().any(|line| line.contains("add")));
    }

    #[test]
    fn test_codegen_subtraction() {
        let mut codegen = CodeGenerator::new();
        let expr = Expr::Binary {
            left: Box::new(Expr::Literal(10)),
            operator: Operator::Minus,
            right: Box::new(Expr::Literal(5)),
        };
        let output = codegen.generate(&expr);
        
        // Check that we have two mov instructions for the literals
        assert!(output.iter().any(|line| line.contains("mov") && line.contains("10")));
        assert!(output.iter().any(|line| line.contains("mov") && line.contains("5")));
        // Check for the sub operation
        assert!(output.iter().any(|line| line.contains("sub")));
    }

    #[test]
    fn test_codegen_nested_operations() {
        let mut codegen = CodeGenerator::new();
        let expr = Expr::Binary {
            left: Box::new(Expr::Binary {
                left: Box::new(Expr::Literal(1)),
                operator: Operator::Plus,
                right: Box::new(Expr::Literal(3)),
            }),
            operator: Operator::Minus,
            right: Box::new(Expr::Literal(3)),
        };
        let output = codegen.generate(&expr);
        
        // Check that we have all the necessary operations
        assert!(output.iter().any(|line| line.contains("mov") && line.contains("1")));
        assert!(output.iter().any(|line| line.contains("mov") && line.contains("3")));
        assert!(output.iter().any(|line| line.contains("add")));
        assert!(output.iter().any(|line| line.contains("sub")));
    }

    #[test]
    fn test_codegen_var_decl() {
        let mut codegen = CodeGenerator::new();
        let expr = Expr::VarDecl {
            name: "a".to_string(),
            value: Box::new(Expr::Literal(5)),
        };
        let output = codegen.generate(&expr);
        // Check for bss section and variable
        assert!(output.iter().any(|line| line.contains("section .bss")));
        assert!(output.iter().any(|line| line.contains("a: resq 1")));
        // Check for mov to [a]
        assert!(output.iter().any(|line| line.contains("mov [a]")));
    }

    #[test]
    fn test_codegen_var_usage() {
        let mut codegen = CodeGenerator::new();
        let expr = Expr::VarUsage("a".to_string());
        let output = codegen.generate(&expr);
        assert!(output.iter().any(|line| line.contains("mov") && line.contains("[a]")));
    }

    #[test]
    fn test_codegen_statements() {
        let mut codegen = CodeGenerator::new();
        let expr = Expr::Statements(vec![
            Expr::VarDecl {
                name: "a".to_string(),
                value: Box::new(Expr::Literal(10)),
            },
            Expr::VarDecl {
                name: "b".to_string(),
                value: Box::new(Expr::Literal(5)),
            },
            Expr::Binary {
                left: Box::new(Expr::VarUsage("a".to_string())),
                operator: Operator::Plus,
                right: Box::new(Expr::VarUsage("b".to_string())),
            },
        ]);
        let output = codegen.generate(&expr);
        assert!(output.iter().any(|line| line.contains("section .bss")));
        assert!(output.iter().any(|line| line.contains("a: resq 1")));
        assert!(output.iter().any(|line| line.contains("b: resq 1")));
        assert!(output.iter().any(|line| line.contains("mov [a]")));
        assert!(output.iter().any(|line| line.contains("mov [b]")));
        assert!(output.iter().any(|line| line.contains("add")));
    }
} 