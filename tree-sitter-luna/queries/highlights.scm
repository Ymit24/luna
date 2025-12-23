"const" @keyword
"fn" @keyword
"mod" @keyword
"struct" @keyword
"return" @keyword
"while" @keyword
"if" @keyword
"union" @keyword
"enum" @keyword
"let" @keyword

(comment) @comment

[
 (function_macro)
 (macro_expression)
 ] @function.macro

[
  "="
  "-"
  "*"
  "/"
  "+"
  "++"
  "--"
  "|"
  "&"
  "^"
  "<<"
  ">>"
  "<"
  "<="
  ">="
  ">"
  "=="
  "!="
  "&&"
  "||"
  "!"
  "["
  "]"
] @operator

(data_type) @type
(number) @number

(string_literal) @string
