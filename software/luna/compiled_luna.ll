; ModuleID = 'm'
source_filename = "m"

define void @module_function() {
entry:
  %a = alloca i32, align 4
  store i32 2, ptr %a, align 4
  %x = alloca i32, align 4
  store i32 3, ptr %x, align 4
  %g = alloca i32, align 4
  store i32 5, ptr %g, align 4
  %other = alloca i32, align 4
  store i32 100, ptr %other, align 4
  %main = alloca void (), align 8
  store ptr @0, ptr %main, align 8
}

declare void @0()
