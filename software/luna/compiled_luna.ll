; ModuleID = 'm'
source_filename = "m"

define void @main() {
entry:
  %a = alloca i32, align 4
  store i32 2, ptr %a, align 4
  %x = alloca i32, align 4
  store i32 3, ptr %x, align 4
  %g = alloca i32, align 4
  store i32 5, ptr %g, align 4
  %other = alloca i32, align 4
  store i32 100, ptr %other, align 4
  %main = alloca ptr, align 8
  store ptr @0, ptr %main, align 8
}

define void @0() {
entry:
  %thing = alloca i32, align 4
  store i32 10, ptr %thing, align 4
  %inner = alloca ptr, align 8
  store ptr @1, ptr %inner, align 8
  %next = alloca i32, align 4
  store i32 9, ptr %next, align 4
}

define void @1() {
entry:
  %otheragain = alloca i32, align 4
  store i32 20, ptr %otheragain, align 4
}
