
module.exports = grammar({
  name: 'luna',

  word: $ => $.symbol,

  extras: $ => [
    $.comment,
    /\s/ // Usually you also want to ignore whitespace
  ],

  rules: {
    source_file: $ => repeat($.module_statement),

    module_statement: $ => seq($.decl_statement),
    function_statement: $ => seq(
      choice(
        $.decl_statement,
        $.if_statement,
        $.while_statement,
        $.return_statement,
        $.expression,
      )
    ),

    if_statement: $ => seq('if', $.expression, $.function_body),
    while_statement: $ => seq('while', $.expression, $.function_body),
    return_statement: $ => seq('return', $.expression, optional(';')),

    decl_statement: $ => seq(
      choice('let', 'const'), $.symbol, optional(seq(':', $.data_type)), '=', $.expression
    ),

    data_type: $ => choice(
      seq('*', $.data_type),
      seq('[', $.data_type, ';', $.number, ']'),
      $.qualified_symbol,
    ),

    expression: $ => choice(
      $.leaf_expression,
      $.unary_expression,
      $.postfix_expression,
      $.binary_expression
    ),

    leaf_expression: $ => choice(
      'true',
      'false',
      'null',
      $.qualified_symbol,
      $.number,
      $.string_literal,
    ),

    unary_expression: $ => choice(
      seq('(', $.expression, ')'),
      $.module_definition_expression,
      $.function_definition_expression,
      $.struct_definition_expression,
      $.struct_initialization_expression,
      $.enum_definition_expression,
      prec.right(100, seq('++', $.expression)),
      prec.right(100, seq('--', $.expression)),
      prec.right(100, seq('&', $.expression)),
      prec.right(100, seq('*', $.expression)),
      prec.right(100, seq('!', $.expression)),
    ),

    postfix_expression: $ => choice(
      prec.right(200, seq($.expression, '[', $.expression, ']')),
      prec.left(200, seq($.expression, '++')),
      prec.left(200, seq($.expression, '--')),
      prec.left(200, seq($.qualified_symbol, '(', optional(seq($.expression, repeat(seq(',', $.expression)))), ')')),
      prec.left(200, $.macro_expression),
    ),

    binary_expression: $ => choice(
      prec.left(15, seq($.expression, '.', $.symbol)),
      prec.left(14, seq($.expression, '*', $.expression)),
      prec.left(14, seq($.expression, '/', $.expression)),
      prec.left(13, seq($.expression, '+', $.expression)),
      prec.left(13, seq($.expression, '-', $.expression)),
      prec.left(12, seq($.expression, '<<', $.expression)),
      prec.left(12, seq($.expression, '>>', $.expression)),
      prec.left(11, seq($.expression, '<', $.expression)),
      prec.left(11, seq($.expression, '>', $.expression)),
      prec.left(11, seq($.expression, '<=', $.expression)),
      prec.left(11, seq($.expression, '>=', $.expression)),
      prec.left(10, seq($.expression, '==', $.expression)),
      prec.left(10, seq($.expression, '!=', $.expression)),
      prec.left(9, seq($.expression, '&', $.expression)),
      prec.left(8, seq($.expression, '^', $.expression)),
      prec.left(7, seq($.expression, '|', $.expression)),
      prec.left(6, seq($.expression, '&&', $.expression)),
      prec.left(5, seq($.expression, '||', $.expression)),
      prec.left(2, seq($.expression, ',', $.expression)),
      prec.left(1, seq($.expression, '=', $.expression)),
    ),

    macro_expression: $ => seq('@', choice(
      seq('cast', '(', $.data_type, ',', $.expression, ')'),
      seq('valuesize', '(', $.expression, ')'),
      seq('typesize', '(', $.expression, ')'),
    )),

    struct_initialization_expression: $ => seq(
      '.',
      $.qualified_symbol,
      $.struct_initialization_body,
    ),

    struct_initialization_body: $ => seq(
      '{',
      optional(
        seq(
          seq($.symbol, ':', $.expression),
          repeat(
            seq(',', $.symbol, ':', $.expression),
          ),
        ),
      ),
      optional(','),
      '}'
    ),

    enum_definition_expression: $ => seq('enum', $.enum_definition_body),
    enum_definition_body: $ => seq('{', optional(seq($.symbol, repeat(seq(',', $.symbol)))), optional(','), '}'),

    struct_definition_expression: $ => seq(choice('struct', 'union'), $.struct_definition_body),
    struct_definition_body: $ => seq('{', optional($.struct_definition_argument_list), '}'),
    struct_definition_argument_list_item: $ => seq($.symbol, ':', $.data_type),
    struct_definition_argument_list: $ => seq(
      $.struct_definition_argument_list_item,
      repeat(seq(',', $.struct_definition_argument_list_item)),
      optional(','),
    ),


    function_definition_expression: $ => prec.left(100, seq(
      'fn',
      optional($.function_macros),
      '(',
      optional($.function_definition_argument_list),
      ')',
      optional(seq(':', $.data_type)),
      optional($.function_body),
    )),

    function_macros: $ => seq(
      $.function_macro,
      repeat($.function_macro)
    ),

    function_macro: $ => seq(
      '@', choice(
        seq('extern', '[', $.string_literal, ']'),
        'variadic'
      )
    ),

    function_definition_argument_list_item: $ => seq($.symbol, ':', $.data_type),
    function_definition_argument_list: $ => seq(
      $.function_definition_argument_list_item,
      repeat(seq(',', $.function_definition_argument_list_item)),
      optional(','),
    ),

    module_definition_expression: $ => seq('mod', $.module_body),

    function_body: $ => seq('{', repeat($.function_statement), '}'),
    module_body: $ => seq('{', repeat($.module_statement), '}'),

    qualified_symbol: $ => seq($.symbol, repeat(seq('::', $.symbol))),

    number: $ => /\d+/,
    string_literal: $ => token(seq(
      '"',
      repeat(choice(
        /[^"]/,      // Any character except quote, backslash, or newline
        /\\./            // Escape sequences starting with a backslash
      )),
      '"'
    )),

    comment: $ => token(choice(
      seq('//', /.*/),
      seq(
        '/*',
        /[^*]*\*+([^/*][^*]*\*+)*/,
        '/'
      )
    )),

    symbol: $ => /[a-zA-Z_][a-zA-Z0-9_]*/,
  }
});
