#[derive(Debug, Clone)]
pub enum Expr {
    Literal(u16),
    Binary {
        left: Box<Expr>,
        operator: Operator,
        right: Box<Expr>,
    },
    VarDecl {
        name: String,
        value: Box<Expr>,
    },
    VarUsage(String),
    Statements(Vec<Expr>),
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum Operator {
    Plus,
    Minus,
}
