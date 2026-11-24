module.exports = grammar({
  name: 'monkey_base',

  rules: {
    // Top-level program consists of a sequence of statements
    program: $ => repeat($.statement),

    // A statement can be a let, return, or an expression followed by a semicolon
    statement: $ => choice(
      $.letStatement,
      $.returnStatement,
      $.expressionStatement
    ),

    // 'let IDENTIFIER = EXPRESSION;'
    letStatement: $ => seq(
      'let',
      field('name', $.identifier),
      '=',
      field('value', $.expression),
      ';'
    ),

    // 'return EXPRESSION;'
    returnStatement: $ => seq(
      'return',
      field('returnValue', optional($.expression)),
      ';'
    ),

    // EXPRESSION ';'
    expressionStatement: $ => seq(
      $.expression,
      ';'
    ),

    // All possible expressions
    expression: $ => choice(
      $.identifier,
      $.integerLiteral,
      $.booleanLiteral,
      $.prefixExpression,
      $.infixExpression,
      $.ifExpression,
      $.functionLiteral,
      $.callExpression,
      $.stringLiteral,
      $.arrayLiteral,
      $.indexExpression,
      $.hashLiteral,
      $.groupedExpression // For expressions like (1 + 2)
    ),

    // Valid identifier names (e.g., 'foo', 'bar_baz')
    identifier: $ => /[a-zA-Z_][a-zA-Z_0-9]*/,

    // Integer numbers (e.g., '123', '45')
    integerLiteral: $ => /[0-9]+/,

    // String literals (e.g., "hello world")
    stringLiteral: $ => /"[^"]*"/,

    // Boolean literals 'true' or 'false'
    booleanLiteral: $ => choice('true', 'false'),

    // Prefix operators (e.g., !a, -b)
    prefixExpression: $ => prec(6, seq(
      field('operator', choice('!', '-')),
      field('right', $.expression)
    )),

    // Infix operators (e.g., a + b, x == y)
    infixExpression: $ => prec.left(5, seq(
      field('left', $.expression),
      field('operator', choice('+', '-', '*', '/', '<', '>', '==', '!=')),
      field('right', $.expression)
    )),

    // 'if (CONDITION) { CONSEQUENCE } else { ALTERNATIVE }'
    ifExpression: $ => seq(
      'if',
      field('condition', $.expression),
      field('consequence', $.blockStatement),
      optional(field('alternative', $.elseClause)),
    ),

    elseClause: $ => seq(
      'else',
      choice(
        $.blockStatement,
        $.ifExpression, // This allows for 'else if'
      ),
    ),
    // 'fn(PARAMETERS) { BODY }'
    functionLiteral: $ => seq(
      'fn',
      field('parameters', $.functionParameters),
      field('body', $.blockStatement)
    ),

    // List of function parameters 'a, b, c'
    functionParameters: $ => seq(
      '(',
      optional(seq(
        $.identifier,
        repeat(seq(',', $.identifier))
      )),
      ')'
    ),

    // Function calls 'foo(ARGS)'
    callExpression: $ => prec.left(7, seq(
      field('function', $.expression),
      field('arguments', $.callArguments)
    )),

    // List of call arguments '1, 2, "hello"'
    callArguments: $ => seq(
      '(',
      optional(seq(
        $.expression,
        repeat(seq(',', $.expression))
      )),
      ')'
    ),

    // Code blocks '{ STATEMENTS }'
    // This allows for explicit statements and an optional final expression (implicit return)
    blockStatement: $ => seq(
      '{',
      repeat($.statement),
      optional($.expression), // The last expression in a block (no trailing semicolon)
      '}'
    ),

    // Array literals '[1, 2, 3]'
    arrayLiteral: $ => seq(
      '[',
      optional(seq(
        $.expression,
        repeat(seq(',', $.expression))
      )),
      ']'
    ),

    // Index expressions 'array[index]'
    indexExpression: $ => prec.left(8, seq(
      field('left', $.expression),
      '[',
      field('index', $.expression),
      ']'
    )),

    // Hash literals '{"key": "value", 1: true}'
    hashLiteral: $ => seq(
      '{',
      optional(seq(
        $.hashPair,
        repeat(seq(',', $.hashPair))
      )),
      '}'
    ),

    // A key-value pair in a hash literal 'key : value'
    hashPair: $ => seq(
      field('key', $.expression),
      ':',
      field('value', $.expression)
    ),

    // Parenthesized expressions '(1 + 2)'
    groupedExpression: $ => seq(
      '(',
      $.expression,
      ')'
    )
  }
});
