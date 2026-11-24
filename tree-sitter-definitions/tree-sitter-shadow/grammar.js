module.exports = grammar({
  name: 'shadow',

  rules: {
    source_file: $ => repeat(
      choice(
        $.address_instruction,
        $.compute_instruction,
        $.label_instruction,
        $.comment,
      )
    ),
    address_instruction: $ => seq($.at_sign, choice(seq('$', $.label), $.number)),
    compute_instruction: $ => seq($.destination, '=', $.expression, optional(seq(';', $.jmp))),
    label_instruction: $ => seq('$', $.label, ':'),
    destination: $ => choice('_', 'A', 'D', 'M'),

    label: $ => /[a-zA-Z]*/,

    register: $ => choice('A', 'D', 'M', '1'),

    jmp: $ => choice('JMP', 'JEQ', 'JLT', 'JNQ', 'JGT', 'JGQ', 'JLQ'),

    operator: $ => choice('+', '-', '&', '|', '~'),

    comment: $ => seq('#', /[^\n]*/),

    expression: $ => choice(
      prec.left(2, seq($.register, $.operator, optional($.register))),
      $.register
    ),

    number: $ => /\d+/,
    at_sign: $ => '@',


  }
})
