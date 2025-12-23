[(module_body) (function_body) (struct_definition_body) (struct_initialization_body) (enum_definition_body)] @indent.begin

(comment) @indent.auto

"}" @indent.branch
(module_body "}" @indent.end)
(function_body "}" @indent.end)
(struct_definition_body "}" @indent.end)
(struct_initialization_body "}" @indent.end)
(enum_definition_body "}" @indent.end)
