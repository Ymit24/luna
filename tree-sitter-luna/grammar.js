
module.exports = grammar({
  name: 'luna',

  rules: {
    // TODO: add the actual grammar rules
    source_file: $ => repeat(choice($.function, $.comment)),

    comment: $ => token(seq('//', /.*/)),

    function: $ => seq('fn', field('name', $.identifier), '(', field('args', optional($.function_args)), ')', ':', field('ret_type', $.type),
      field('body', $._block_scope),
    ),

    function_args: $ => seq($.identifier, ':', $.type),

    type: $ => choice(
      seq('u', choice('8', '16')),
      seq('i', choice('8', '16')),
      'void',
    ),

    if: $ => seq('if', $._expression, $._block_scope),

    _expression: $ => choice($.unary_expression, $.binary_expression, $.identifier, $.number, $.string_literal, $.function_call),

    unary_expression: $ => prec(3, choice(seq('-', $._expression), seq('!', $._expression))),
    binary_expression: $ => choice(
      prec.left(2, seq($._expression, '*', $._expression)),
      prec.left(2, seq($._expression, '/', $._expression)),
      prec.left(1, seq($._expression, '+', $._expression)),
      prec.left(1, seq($._expression, '-', $._expression)),
      prec.left(1, seq($._expression, '==', $._expression))
    ),

    statement: $ => choice(seq($.function_call, ';'), $.if, seq($.return, ';'), $.comment),

    return: $ => seq('return', $._expression),

    function_call: $ => seq($.identifier, '(', optional($._expression), ')'),

    _block_scope: $ => seq('{', repeat($.statement), '}'),

    identifier: $ => /[a-z]+/,

    string_literal: $ => token(seq(
      '"',
      repeat(choice(
        /[^"\\\n]/,      // Any character except quote, backslash, or newline
        /\\./            // Escape sequences starting with a backslash
      )),
      '"'
    )),

    number: $ => /\d+/
  }
});
