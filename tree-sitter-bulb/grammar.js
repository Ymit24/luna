module.exports = grammar({
  name: 'bulb',

  rules: {
    source_file: $ => repeat(
      choice(
        $.label_definition,
        $.push,
        $.pop,
        $.add,
        $.sub,
        $.and,
        $.or,
        $.call,
        $.ret,
        $.asm,
        $.jmp,
        $.comment
      )
    ),
    push: $ => seq('push', $.virtual_memory_segment, $.push_pop_arg),
    pop: $ => seq('pop', $.virtual_memory_segment, $.push_pop_arg),
    add: _ => 'add',
    sub: _ => 'sub',
    and: _ => 'and',
    or: _ => 'or',
    call: $ => seq('call', $.identifier),
    ret: _ => 'ret',
    asm: $ => seq('asm', '{', $.asm_raw, '}'),
    asm_raw: _ => /[^}]*/,
    virtual_memory_segment: _ => choice('const', 'args', 'local'),
    jmp: $ => seq(
      choice(
        'jmp',
        'jeq',
        'jnq',
        'jlt',
        'jlq',
        'jgt',
        'jgq',
      ), $.label_definition
    ),
    push_pop_arg: $ => choice($.predefined_symbol, $.constant),
    predefined_symbol: _ => seq('%', choice('sp', 'bsp', 'x', 'pc')),

    comment: _ => seq('#', /[^\n]*/),

    constant: _ => choice(seq('0x', /[\da-fA-F]+/),
      /\d+/,
    ),
    label_definition: $ => seq('$', $.identifier, ':'),
    identifier: _ => /[_a-zA-Z0-9]*/,
  },
});
