"const" @keyword
"fn" @keyword
"mod" @keyword
"struct" @keyword
"union" @keyword
"enum" @keyword
"let" @keyword

(single_line_comment) @comment
(multi_line_commment) @comment

(function_macro) @function.macro

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
