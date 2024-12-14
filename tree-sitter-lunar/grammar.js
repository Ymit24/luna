/**
 * @file Lunar grammar for tree-sitter
 * @author Christian Smith
 * @license MIT
 */

/// <reference types="tree-sitter-cli/dsl" />
// @ts-check

// module.exports = grammar({
//   name: "lunar",
//
//   rules: {
//     // TODO: add the actual grammar rules
//     source_file: $ => "hello"
//   }
// });



module.exports = grammar({
  name: 'lunar',

  rules: {
    source_file: $ => repeat($._statement),

    _statement: $ => choice(
      $.comment,
      $.function_definition,
      $.constant_declaration,
      $.structure_definition,
      $.variable_assignment,
      $.function_pointer_declaration,
      $.macro,
      $.function_call,
      $.with_clause,
      $.assembly_block,
      $.if_statement,
      $.while_statement,
      $.return_statement
    ),

    return_statement: $ => prec.left(seq(
      'return',
      optional($._expression)
    )),

    macro: $ => seq('@', $.function_call),

    if_statement: $ => seq(
      'if',
      $._expression,
      'begin',
      repeat($._statement),
      'end'
    ),

    while_statement: $ => seq(
      'while',
      $._expression,
      'begin',
      repeat($._statement),
      'end'
    ),

    function_definition: $ => seq(
      'function',
      field('name', $.identifier),
      field('args', optional(
        $.parameter_list,
      )),
      field('returns', optional(seq(
        'returns',
        $._type_with_modifiers,
      ))),
      field('locals', optional(
        $.with_clause
      )),
      field('body', choice(
        seq('begin', repeat($._statement), 'end'),
        // seq('is', $._expression),
        // seq('is', 'begin', repeat($._statement), 'end')
      ))
    ),

    constant_declaration: $ => seq(
      'constant',
      $.type,
      $.identifier,
      '=',
      $._expression
    ),

    variable_assignment: $ => seq(
      $.identifier,
      '=',
      $._expression
    ),

    struct_field: $ => seq(
      $._type_with_modifiers,
      $.identifier
    ),

    structure_definition: $ => seq(
      'structure',
      $.identifier,
      repeat($.structure_field),
      'end'
    ),

    structure_field: $ => seq(
      $._type_with_modifiers,
      $.identifier
    ),

    function_pointer_declaration: $ => seq(
      'function_pointer',
      $.identifier
    ),

    with_clause: $ => seq(
      'with',
      $.parameter_list
    ),

    local_variable_declaration: $ => seq(
      $._type_with_modifiers,
      $.identifier
    ),

    assembly_block: $ => seq(
      choice('assembly', 'raw_assembly'),
      'begin',
      repeat($._assembly_instruction),
      'end'
    ),

    _assembly_instruction: $ => /.+/, // Can be refined for actual instructions

    parameter_list: $ => repeat1(
      seq($._type_with_modifiers, $.identifier)
    ),

    _type_with_modifiers: $ => seq(
      optional('*'),
      $.type,
      optional(seq('[', $._expression, ';', $._expression, ']'))
    ),

    type: $ => choice('word', 'byte'),

    _expression: $ => choice(
      $.identifier,
      $.number,
      $.string,
      $.paren_expression,
      $.binary_expression,
      $.function_call,
      $.array_access,
      $.type_cast
    ),

    paren_expression: $ => seq(
      "(",
      $._expression,
      ")"
    ),

    binary_expression: $ => prec.left(seq(
      $._expression,
      choice('+', '-', '*', '/', 'or', 'and', '<', '>', '!=', '==', '<=', '>='),
      $._expression
    )),

    function_call: $ => seq(
      field('name', $.identifier),
      '(',
      optional(commaSep($._expression)),
      ')'
    ),

    array_access: $ => seq(
      $.identifier,
      '[',
      $._expression,
      ']'
    ),

    type_cast: $ => seq(
      '(',
      $._expression,
      'as',
      $.type,
      ')'
    ),

    identifier: $ => /[a-zA-Z_][a-zA-Z0-9_]*/,
    number: $ => /\d+/,
    string: $ => /".*?"/,

    comment: $ => token(seq('#', /.*/)),
  }
});

function commaSep(rule) {
  return optional(seq(rule, repeat(seq(',', rule))));
}
