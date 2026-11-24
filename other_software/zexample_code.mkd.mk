const strings = use "strings";

const source_file = mod {
    const SourceFile = struct {
        path: strings::string,
        content: strings::string,
    };

    const make = fn(path: strings::string, content: strings::string) {
        return SourceFile {
            path: path,
            content: content,
        };
    };

    const from_file = fn(path: strings::string) {
        const file = io::open(path);
        const content = io::read_all(file);
        return make(path, content);
    };
};


const Span = struct {
    source_file: *SourceFile,
    start: usize,
    end: usize,
};

const toks = mod {
    const Type = enum { Illegal, Eof };
    const Value = union {
        None: i1,
        Int: i16,
        Bool: i1,
        String: strings::string,
    };
    const Token = struct {
        type: Type,
        value: Value,
        span: Span,
    };

    const make = fn(type: Type, value: Value, span: Span) {
        return Token {
            type: type,
            value: value,
            span: span,
        };
    };
};

const lexer = mod {
    const Lexer = struct {
        source_file: *SourceFile,
        stream: strings::Iter,
        current_pos: usize,
    };

    const make = fn(source: *SourceFile, stream: strings::Iter) {
        return Lexer {
            source_file: source,
            stream: stream,
            current_pos: 0,
        };
    };

    const current_span = fn(self: *Lexer, length: u16) {
        return Span {
            source_file: self.source_file,
            start: self.current_pos,
            end: self.current_pos + length,
        };
    };

    const is_ident_start = fn(c: u8) {
        return chars::is_alpha(c) or c == '_';
    };

    const is_ident_continue = fn(c: u8) {
        return chars::is_alpha(c) or chars::is_digit(c) or c == '_';
    };

    const skip_whitespace = fn(self: *Lexer) {
        while (true) {
            const c = strings::iters::peek(self.stream);
            if c == ' ' or c == '\t' or c == '\n' or c == '\r' {
                strings::iters::next(self.stream);
                self.current_pos += 1;
            } else {
                break;
            }
        }
    };

    const next = fn(self: *Lexer): toks::Token {
        skip_whitespace(self);

        const start_pos = self.current_pos;
        const cur = strings::iters::peek(self.stream);

        // End of file
        if cur == 0 {
            return toks::make(toks::Type::Eof, toks::Value::Int, current_span(self, 0));
        }

        // Identifiers and keywords
        if is_ident_start(cur) {
            var ident = "";
            var len: u16 = 0;
            while (is_ident_continue(strings::iters::peek(self.stream))) {
                ident = strings::concat(ident, strings::from_char(strings::iters::peek(self.stream)));
                strings::iters::next(self.stream);
                self.current_pos += 1;
                len += 1;
            }
            // Keywords
            if ident == "const" {
                return toks::make(toks::Type::Const, toks::Value::String, current_span(self, len));
            }
            if ident == "fn" {
                return toks::make(toks::Type::Fn, toks::Value::String, current_span(self, len));
            }
            if ident == "mod" {
                return toks::make(toks::Type::Mod, toks::Value::String, current_span(self, len));
            }
            if ident == "struct" {
                return toks::make(toks::Type::Struct, toks::Value::String, current_span(self, len));
            }
            if ident == "enum" {
                return toks::make(toks::Type::Enum, toks::Value::String, current_span(self, len));
            }
            if ident == "return" {
                return toks::make(toks::Type::Return, toks::Value::String, current_span(self, len));
            }
            if ident == "if" {
                return toks::make(toks::Type::If, toks::Value::String, current_span(self, len));
            }
            if ident == "else" {
                return toks::make(toks::Type::Else, toks::Value::String, current_span(self, len));
            }
            if ident == "while" {
                return toks::make(toks::Type::While, toks::Value::String, current_span(self, len));
            }
            if ident == "for" {
                return toks::make(toks::Type::For, toks::Value::String, current_span(self, len));
            }
            // Otherwise, identifier
            return toks::make(toks::Type::Ident, toks::Value::String, current_span(self, len));
        }

        // Numbers (only integer for now)
        if chars::is_digit(cur) {
            var value: i16 = 0;
            var len: u16 = 0;
            while (chars::is_digit(strings::iters::peek(self.stream))) {
                value = value * 10 + (strings::iters::peek(self.stream) - '0');
                strings::iters::next(self.stream);
                self.current_pos += 1;
                len += 1;
            }
            return toks::make(toks::Type::Int, toks::Value::Int, current_span(self, len));
        }

        // String literals
        if cur == '"' {
            strings::iters::next(self.stream);
            self.current_pos += 1;
            var str = "";
            var len: u16 = 1;
            while (true) {
                const c = strings::iters::peek(self.stream);
                if c == '"' or c == 0 {
                    break;
                }
                str = strings::concat(str, strings::from_char(c));
                strings::iters::next(self.stream);
                self.current_pos += 1;
                len += 1;
            }
            if strings::iters::peek(self.stream) == '"' {
                strings::iters::next(self.stream);
                self.current_pos += 1;
                len += 1;
            }
            return toks::make(toks::Type::String, toks::Value::String, current_span(self, len));
        }


        // Two-character tokens (==, !=, <=, >=, ::, ->, =>)
        const next_c = strings::iters::peek_ahead(self.stream, 1);
        if cur == '=' and next_c == '=' {
            strings::iters::next(self.stream);
            strings::iters::next(self.stream);
            self.current_pos += 2;
            return toks::make(toks::Type::Eq, toks::Value::Int, current_span(self, 2));
        }
        if cur == '!' and next_c == '=' {
            strings::iters::next(self.stream);
            strings::iters::next(self.stream);
            self.current_pos += 2;
            return toks::make(toks::Type::Neq, toks::Value::Int, current_span(self, 2));
        }
        if cur == '<' and next_c == '=' {
            strings::iters::next(self.stream);
            strings::iters::next(self.stream);
            self.current_pos += 2;
            return toks::make(toks::Type::Le, toks::Value::Int, current_span(self, 2));
        }
        if cur == '>' and next_c == '=' {
            strings::iters::next(self.stream);
            strings::iters::next(self.stream);
            self.current_pos += 2;
            return toks::make(toks::Type::Ge, toks::Value::Int, current_span(self, 2));
        }
        if cur == ':' and next_c == ':' {
            strings::iters::next(self.stream);
            strings::iters::next(self.stream);
            self.current_pos += 2;
            return toks::make(toks::Type::DoubleColon, toks::Value::Int, current_span(self, 2));
        }
        if cur == '-' and next_c == '>' {
            strings::iters::next(self.stream);
            strings::iters::next(self.stream);
            self.current_pos += 2;
            return toks::make(toks::Type::Arrow, toks::Value::Int, current_span(self, 2));
        }
        if cur == '=' and next_c == '>' {
            strings::iters::next(self.stream);
            strings::iters::next(self.stream);
            self.current_pos += 2;
            return toks::make(toks::Type::FatArrow, toks::Value::Int, current_span(self, 2));
        }

        const check_single_char_token = fn (literal: u16, type: toks::Type) {
            if cur == single_char_tokens[i][0]) {
                strings::iters::next(self.stream);
                self.current_pos += 1;
                return toks::make(single_char_tokens[i][1], toks::Value::Int, current_span(self, 1));
            }
        };

        let single_char_token = toks::Type::Illegal;
        single_char_token = check_single_char_token('{', toks::Type::LBrace);
        if single_char_token != toks::Type::Illegal { return single_char_token; }
        single_char_token = check_single_char_token('}', toks::Type::RBrace);
        if single_char_token != toks::Type::Illegal { return single_char_token; }
        single_char_token = check_single_char_token('(', toks::Type::LParen);
        if single_char_token != toks::Type::Illegal { return single_char_token; }
        single_char_token = check_single_char_token(')', toks::Type::RParen);
        if single_char_token != toks::Type::Illegal { return single_char_token; }
        single_char_token = check_single_char_token('[', toks::Type::LBracket);
        if single_char_token != toks::Type::Illegal { return single_char_token; }
        single_char_token = check_single_char_token(']', toks::Type::RBracket);
        if single_char_token != toks::Type::Illegal { return single_char_token; }
        single_char_token = check_single_char_token(',', toks::Type::Comma);
        if single_char_token != toks::Type::Illegal { return single_char_token; }
        single_char_token = check_single_char_token(':', toks::Type::Colon);
        if single_char_token != toks::Type::Illegal { return single_char_token; }
        single_char_token = check_single_char_token(';', toks::Type::Semicolon);
        if single_char_token != toks::Type::Illegal { return single_char_token; }
        single_char_token = check_single_char_token('.', toks::Type::Dot);
        if single_char_token != toks::Type::Illegal { return single_char_token; }
        single_char_token = check_single_char_token('=', toks::Type::Assign);   
        if single_char_token != toks::Type::Illegal { return single_char_token; }
        single_char_token = check_single_char_token('+', toks::Type::Plus);
        if single_char_token != toks::Type::Illegal { return single_char_token; }
        single_char_token = check_single_char_token('-', toks::Type::Minus);
        if single_char_token != toks::Type::Illegal { return single_char_token; }
        single_char_token = check_single_char_token('*', toks::Type::Star);
        if single_char_token != toks::Type::Illegal { return single_char_token; }
        single_char_token = check_single_char_token('/', toks::Type::Slash);
        if single_char_token != toks::Type::Illegal { return single_char_token; }
        single_char_token = check_single_char_token('%', toks::Type::Percent);
        if single_char_token != toks::Type::Illegal { return single_char_token; }
        single_char_token = check_single_char_token('<', toks::Type::Lt);
        if single_char_token != toks::Type::Illegal { return single_char_token; }
        single_char_token = check_single_char_token('>', toks::Type::Gt);
        if single_char_token != toks::Type::Illegal { return single_char_token; }
        single_char_token = check_single_char_token('|', toks::Type::Pipe);
        if single_char_token != toks::Type::Illegal { return single_char_token; }
        single_char_token = check_single_char_token('&', toks::Type::Amp);  
        if single_char_token != toks::Type::Illegal { return single_char_token; }
        single_char_token = check_single_char_token('!', toks::Type::Bang);
        if single_char_token != toks::Type::Illegal { return single_char_token; }
         

        // Comments (// to end of line)
        if cur == '/' and next_c == '/' {
            while (true) {
                const c = strings::iters::peek(self.stream);
                if c == '\n' or c == 0 {
                    break;
                }
                strings::iters::next(self.stream);
                self.current_pos += 1;
            }
            // Skip newline
            if strings::iters::peek(self.stream) == '\n' {
                strings::iters::next(self.stream);
                self.current_pos += 1;
            }
            // After skipping comment, get next token
            return next(self);
        }

        // Unknown/illegal
        strings::iters::next(self.stream);
        self.current_pos += 1;
        return toks::make(toks::Type::Illegal, toks::Value::Int, current_span(self, 1));
    };
};

const ast = mod {
    const Type = enum {
        Let,
        Const,
        Fn,
        Struct,
        Enum,
    };
    const ModuleStatementNodePtr = union {
        let: *LetStatement,
        const: *ConstStatement,
        fn: *FnStatement,
        struct: *StructStatement,
        enum: *EnumStatement,
    };

    const ModuleStatementNode = struct {
        type: Type,
        value: ModuleStatementNodePtr,
        next: *ModuleStatementNode,
    };

    const Program = struct {
        firstNode: *ModuleStatementNode,
        nodeCount: u16
    };

    const ExpressionType = enum {
        Int,
        String,
        Bool,
        Ident,
        Binary
    };

    const ExpressionNodePtr = union {
        int: *IntExpressionNode,
        string: *StringExpressionNode,
        bool: *BoolExpressionNode,
        ident: *IdentExpressionNode,
        binary: *BinaryExpressionNode,
        unary: *UnaryExpressionNode,
        ternary: *TernaryExpressionNode,
        call: *CallExpressionNode,
    };

    const CallExpressionArgNode = struct {
        expr: ExpressionNode,
        next: *CallExpressionArgNode,
    };

    const CallExpressionNode = struct {
        callee: ExpressionNode,
        args: *CallExpressionArgNode,
        args_count: u16,
    };

    const ExpressionNode = struct {
        type: ExpressionType,
        value: ExpressionNodePtr,
    };

    const LetStatementNode = struct {
        name: strings::String,
        value: ExpressionNode,
    };

    const IntExpressionNode = struct {
        value: i16,
    };

    const StringExpressionNode = struct {
        value: strings::String,
    };

    const BoolExpressionNode = struct {
        value: i1,
    };

    const IdentExpressionNode = struct {
        value: strings::String,
    };

    const BinaryExpressionType = enum {
        Add,
        Sub,
        Mul,
        Div,
        Mod,
        Eq,
        Neq,
        Lt,
        Gt,
        Le,
        Ge,
    };

    const BinaryExpressionNode = struct {
        type: BinaryExpressionType,
        left: ExpressionNode,
        right: ExpressionNode,
    };

    const UnaryExpressionType = enum {
        Not,
        Neg,
    };

    const UnaryExpressionNode = struct {
        type: UnaryExpressionType,
        value: ExpressionNode,
    };

    const TernaryExpressionNode = struct {
        condition: ExpressionNode,
        true_value: ExpressionNode,
        false_value: ExpressionNode,
    };
};

const parser = mod {
    const Parser = struct {
        lexer: *lexer::Lexer,
        current_token: toks::Token,
        peek_token: toks::Token,
    };

    const make = fn(lexer: *lexer::Lexer) {
        return Parser {
            lexer: lexer,
            current_token: lexer::next(lexer),
            peek_token: lexer::next(lexer),
        };
    };

    const next_token = fn(self: *Parser) {
        self.current_token = self.peek_token;
        self.peek_token = lexer::next(self.lexer);
    };

    const parse_let_statement = fn(self: *Parser): ast::ModuleStatementNode {
        const token = self.current_token;
        next_token(self);
        if token.type != toks::Type::Ident {
            error("Expected identifier");
        }
        const name = token.value;
        next_token(self);
        if token.type != toks::Type::Assign {
            error("Expected assignment operator");
        }
        const value = parse_expression(self);
        next_token(self);
        if token.type != toks::Type::Semicolon {
            error("Expected semicolon");
        }
        return ast::ModuleStatementNode {
            type: ast::Type::Let,
            value: ast::LetStatementNode {
                name: name,
                value: value,
            },
            next: nil,
        };
    };

    const parse_expression = fn(self: *Parser): ast::ExpressionNode {
        // NOTE: Placeholder for now
        return ast::ExpressionNode {
            type: ast::ExpressionType::Int,
            value: ast::IntExpressionNode { value: 0 },
        };
    };

    const parse_module_statement = fn(self: *Parser): ast::ModuleStatementNode {
        const token = self.current_token;
        next_token(self);
        if token.type == toks::Type::Let {
            return parse_let_statement(self);
        } else if token.type == toks::Type::Const {
            return parse_const_statement(self);
        }
        error("Expected module statement");
    };

    const parse_program = fn(self: *Parser): ast::Program {
        const token = self.current_token;
        next_token(self);

        const node = parse_module_statement(self);
        let node_count: u16 = 1;
        while self.current_token.type != toks::Type::Eof {
            node.next = parse_module_statement(self);
            node_count += 1;
        }

        return ast::Program {
            firstNode: node,
            nodeCount: node_count,
        };
    };
};

const printer = mod {
    const print_program = fn(program: ast::Program) {
        let node = program.firstNode;
        while node != nil {
            print_module_statement(node);
            node = node.next;
        }
    };

    const print_module_statement = fn(node: ast::ModuleStatementNode) {
        if node.type == ast::Type::Let {
            io::println("let " + node.value.name + " = " + print_expression(node.value.value));
        } else if node.type == ast::Type::Const {
            io::println("const " + node.value.name + " = " + print_expression(node.value.value));
        }
    };

    const print_expression = fn(expr: ast::ExpressionNode) {
        if expr.type == ast::ExpressionType::Int {
            io::write(expr.value.int.value);
        } else if expr.type == ast::ExpressionType::String {
            io::write(expr.value.string.value);
        } else if expr.type == ast::ExpressionType::Bool {
            io::write(expr.value.bool.value ? "true" : "false");
        } else if expr.type == ast::ExpressionType::Ident {
            io::write(expr.value.ident.value);
        } else if expr.type == ast::ExpressionType::Binary {
            io::write(print_expression(expr.value.binary.left) + " " + expr.value.binary.type + " " + print_expression(expr.value.binary.right));
        } else if expr.type == ast::ExpressionType::Unary {
            io::write(expr.value.unary.type + print_expression(expr.value.unary.value));
        } else if expr.type == ast::ExpressionType::Ternary {
            io::write(print_expression(expr.value.ternary.condition) + " ? " + print_expression(expr.value.ternary.true_value) + " : " + print_expression(expr.value.ternary.false_value));
        } else if expr.type == ast::ExpressionType::Call {
            io::write(print_expression(expr.value.call.callee) + "(");
            let arg = expr.value.call.args;
            while arg != nil {
                io::write(print_expression(arg.expr));
                arg = arg.next;
            }
            io::write(")");
        }
    };

};

const main = fn() {
    const source_file = source_file::from_file("test.txt");
    const lexer = lexer::make(&source_file, strings::iters::make(source_file.content));
    const token = lexer::next(&lexer);
    const parser = parser::make(&lexer);
    const program = parser::parse_program(&parser);
    printer::print_program(program);
};

const tests = mod {
    const lexer = mod {
        const make_lexer = fn(contents: strings::string) {
            return lexer::make(source_file::make("test.txt", contents), strings::iters::make(contents));
        };

        const test_arith_tokens = fn() {
            const lexer = make_lexer(strings::make("1 + 2 * 3"));
            
            assert_equals(lexer::next(&lexer), toks::make(toks::Type::Int, toks::Value::None, current_span(self, 1)));
            assert_equals(lexer::next(&lexer), toks::make(toks::Type::Plus, toks::Value::None, current_span(self, 1)));
            assert_equals(lexer::next(&lexer), toks::make(toks::Type::Int, toks::Value::None, current_span(self, 1)));
            assert_equals(lexer::next(&lexer), toks::make(toks::Type::Star, toks::Value::None, current_span(self, 1)));
            assert_equals(lexer::next(&lexer), toks::make(toks::Type::Int, toks::Value::None, current_span(self, 1)));
            assert_equals(lexer::next(&lexer), toks::make(toks::Type::Eof, toks::Value::None, current_span(self, 0)));
        };

        const test_fn_tokens = fn() {
            const lexer = make_lexer(strings::make("fn main() { return 0; }"));
            assert_equals(lexer::next(&lexer), toks::make(toks::Type::Fn, toks::Value::None, current_span(self, 1)));
            assert_equals(lexer::next(&lexer), toks::make(toks::Type::Ident, toks::Value::None, current_span(self, 1)));
            assert_equals(lexer::next(&lexer), toks::make(toks::Type::LParen, toks::Value::None, current_span(self, 1)));
            assert_equals(lexer::next(&lexer), toks::make(toks::Type::RParen, toks::Value::None, current_span(self, 1)));
            assert_equals(lexer::next(&lexer), toks::make(toks::Type::LBrace, toks::Value::None, current_span(self, 1)));
            assert_equals(lexer::next(&lexer), toks::make(toks::Type::Return, toks::Value::None, current_span(self, 1)));
            assert_equals(lexer::next(&lexer), toks::make(toks::Type::Int, toks::Value::None, current_span(self, 1)));
            assert_equals(lexer::next(&lexer), toks::make(toks::Type::Semicolon, toks::Value::None, current_span(self, 1)));
            assert_equals(lexer::next(&lexer), toks::make(toks::Type::RBrace, toks::Value::None, current_span(self, 1)));
            assert_equals(lexer::next(&lexer), toks::make(toks::Type::Eof, toks::Value::None, current_span(self, 0)));
        };
    };
};

const std = mod {
    const iters = mod {
        const Iterator = struct<T, U> {
            iterator: T,
            next: fn(self: *Iterator<T>): U,
            peek: fn(self: *Iterator<T>): U,
        };

        const GenericIterator = struct {
            index: u16,
        };

        const make_generic_iterator = fn(): GenericIterator {
            return GenericIterator {
                index: 0,
            };
        };

        const reduce = fn<T, U>(self: *Iterator<T, U>, reducer: fn(acc: U, item: U): U, initial: U): U {
            let acc = initial;
            while self.peek(self.iterator) != nil {
                acc = reducer(acc, self.next(self.iterator));
            }
            return acc;
        };

        const map = fn<T, U, V>(self: *Iterator<T, U>, mapper: fn(item: U): V): Iterator<GenericIterator, V> {
            return Iterator<T, V> {
                iterator: make_generic_iterator(),
                next: fn(self: *Iterator<GenericIterator, V>): V {
                    return mapper(self.next(self.iterator));
                },
                peek: fn(self: *Iterator<GenericIterator, V>): V {
                    return mapper(self.peek(self.iterator));
                },
            };
        };
    };

    const strings = mod {
        const iters = mod {
            const StringIterator = struct {
                string: *strings::String,
                index: u16,
            };

            const make = fn(string: *strings::String): StringIterator {
                return StringIterator {
                    string: string,
                    index: 0,
                };
            };

            const next = fn(self: *StringIterator): u8 {
                return self.string.data[self.index++];
            };

            const peek = fn(self: *StringIterator): u8 {
                return self.string.data[self.index];
            };
        };
    };

    const mem = mod {
        const Allocator = struct<T> {
            allocator: T,
            alloc: fn(self: *Allocator<T>, size: u16): *T,
            free: fn(self: *Allocator<T>),
        };

        const linear = mod {
            const LinearAllocator = struct {
                data: *u8,
                length: u16,
                capacity: u16,
            };

            const make = fn(arena: *u8, size: u16): LinearAllocator {
                return LinearAllocator {
                    data: arena,
                    length: 0,
                    capacity: size,
                };
            };

            const make_allocator = fn(arena: *u8, size: u16): Allocator<LinearAllocator> {
                return Allocator<LinearAllocator> {
                    allocator: make(arena, size),
                    alloc: alloc,
                    free: free,
                };
            };

            const alloc = fn(self: *Allocator<LinearAllocator>, size: u16): *u8 {
                const ptr = self.allocator.data + self.allocator.length;
                self.allocator.length += size;
                return ptr;
            };

            const realloc = fn(self: *Allocator<LinearAllocator>, size: u16): *u8 {
                @panic("Not available for linear allocator");
            };

            const free = fn(self: *Allocator<LinearAllocator>) {
                self.allocator.length = 0;
            };
        };

        const luna = mod {
            const heap = mod {
                const HeapAllocator = struct {
                    data: *u8,
                    length: u16,
                    capacity: u16,
                };

                const make = fn(): HeapAllocator {
                    if !std::platform::is_luna() {
                        @panic("Heap allocator is only available on Luna platform.");
                    }

                    return HeapAllocator {
                        data: std::platform::luna::memory_map::HEAP_START,
                        length: 0,
                        capacity: std::platform::luna::memory_map::HEAP_SIZE,
                    };
                };

                const make_allocator = fn(): Allocator<HeapAllocator> {
                    return Allocator<HeapAllocator> {
                        allocator: make(),
                        alloc: alloc,
                        free: free,
                    };
                };
            };
        };
    };
};

const vec = mod {
    const Vec = struct<T> {
        allocator: *std::mem::Allocator<T>,
        data: *T,
        length: u16,
        capacity: u16,
    };

    const make = fn<T>(allocator: *std::mem::Allocator<T>, capacity: u16): Vec<T> {
        const data = allocator.alloc(capacity * @size(T));
        return Vec<T> {
            allocator: allocator,
            data: data,
            length: 0,
            capacity: capacity,
        };
    };

    const unmake = fn<T>(self: *Vec<T>) {
        self.allocator.free(self.data);
    };

    const push = fn<T>(self: *Vec<T>, item: T) {
        if self.length >= self.capacity {
            self.capacity *= 2;
            self.data = self.allocator.realloc(self.capacity * @size(T));
        }
        self.data[self.length] = item;
        self.length += 1;
    };

    const pop = fn<T>(self: *Vec<T>): T {
        const item = self.data[self.length - 1];
        self.length -= 1;
        return item;
    };

    const get = fn<T>(self: *Vec<T>, index: u16): T {
        return self.data[index];
    };

    const set = fn<T>(self: *Vec<T>, index: u16, item: T) {
        self.data[index] = item;
    };
};

const test_vec = mod {
    const test_push = fn() {
        const arena: [u8; 1024] = _;
        const allocator = std::mem::linear::make_allocator(&arena, 1024);
        const vec = vec::make(&allocator, 1);
        vec::push(&vec, 1);
        assert_equals(vec::get(&vec, 0), 1);
        vec::unmake(&vec);
    };

    const test_pop = fn() {
        const arena: [u8; 1024] = _;
        const allocator = std::mem::linear::make_allocator(&arena, 1024);
        const vec = vec::make(&allocator, 1);
        vec::push(&vec, 1);
        assert_equals(vec::pop(&vec), 1);
        vec::unmake(&vec);
    };
}