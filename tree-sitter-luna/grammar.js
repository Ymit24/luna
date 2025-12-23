
module.exports = grammar({
  name: 'luna',

  extras: $ => [
    $.single_line_comment,
    $.multi_line_commment,
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
    return_statement: $ => seq('return', $.expression),

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
      $.qualified_symbol,
      $.number,
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
      prec.left(200, seq($.qualified_symbol, '(', repeat(seq($.expression, ')')), ')')),
    ),

    field_accessor_expression: $ => seq(
      '.',
      $.symbol
    ),

    binary_expression: $ => choice(
      prec.left(15, seq($.qualified_symbol, repeat($.field_accessor_expression))),
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

    symbol: $ => /[a-zA-Z_][a-zA-Z0-9_]*/,
    number: $ => /\d+/,
    string_literal: $ => token(seq(
      '"',
      repeat(choice(
        /[^"\\\n]/,      // Any character except quote, backslash, or newline
        /\\./            // Escape sequences starting with a backslash
      )),
      '"'
    )),

    single_line_comment: $ => token(seq('//', /.*/)),

    multi_line_commment: $ => token(seq(
      '/*',
      /[^*]*\*+([^/*][^*]*\*+)*/,
      '/'
    )),

    // TODO: add the actual grammar rules
    // source_file: $ => repeat(choice($.function, $.comment)),
    //
    // comment: $ => token(seq('//', /.*/)),
    //
    // function: $ => seq('fn', field('name', $.identifier), '(', field('args', optional($.function_args)), ')', ':', field('ret_type', $.type),
    //   field('body', $._block_scope),
    // ),
    //
    // function_args: $ => seq($.identifier, ':', $.type),
    //
    // type: $ => choice(
    //   seq('u', choice('8', '16')),
    //   seq('i', choice('8', '16')),
    //   'void',
    // ),
    //
    // if: $ => seq('if', $._expression, $._block_scope),
    //
    // _expression: $ => choice($.unary_expression, $.binary_expression, $.identifier, $.number, $.string_literal, $.function_call),
    //
    // unary_expression: $ => prec(3, choice(seq('-', $._expression), seq('!', $._expression))),
    // binary_expression: $ => choice(
    //   prec.left(2, seq($._expression, '*', $._expression)),
    //   prec.left(2, seq($._expression, '/', $._expression)),
    //   prec.left(1, seq($._expression, '+', $._expression)),
    //   prec.left(1, seq($._expression, '-', $._expression)),
    //   prec.left(1, seq($._expression, '==', $._expression))
    // ),
    //
    // statement: $ => choice(seq($.function_call, ';'), $.if, seq($.return, ';'), $.comment),
    //
    // return: $ => seq('return', $._expression),
    //
    // function_call: $ => seq($.identifier, '(', optional($._expression), ')'),
    //
    // _block_scope: $ => seq('{', repeat($.statement), '}'),
    //
    // identifier: $ => /[a-z]+/,
    //
    // string_literal: $ => token(seq(
    //   '"',
    //   repeat(choice(
    //     /[^"\\\n]/,      // Any character except quote, backslash, or newline
    //     /\\./            // Escape sequences starting with a backslash
    //   )),
    //   '"'
    // )),
    //
    // number: $ => /\d+/
  }
});
