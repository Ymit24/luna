; ModuleID = 'm'
source_filename = "m"

@externalputs = external global ptr
@main = external global ptr

define void @global_module_intializer() {
entry:
  store ptr @puts, ptr @externalputs, align 8
  store ptr @0, ptr @main, align 8
}

declare void @puts()

define void @0() {
entry:
}
