module.exports = grammar({
  name: 'bulb',

  rules: {
    source_file: $ => repeat(
      choice(
        $.label_definition,
        $.push,
        $.pop,
        $.comment
      )
    ),
    push: $ => seq('push', $.virtual_memory_segment, $.push_pop_arg),
    pop: $ => seq('pop', $.virtual_memory_segment, $.push_pop_arg),
    virtual_memory_segment: $ => choice('const', 'args', 'local'),

    push_pop_arg: $ => choice($.predefined_symbol, $.constant),
    predefined_symbol: $ => seq('%', 'sp'),

    comment: $ => seq('#', /[^\n]*/),

    constant: $ => choice(seq('0x', /\d+/),
      /\d+/,
    ),
    label_definition: $ => seq('$', $.identifier, ':'),
    identifier: $ => /[_a-zA-Z]*/,
  },
});
