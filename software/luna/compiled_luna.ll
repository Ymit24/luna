; ModuleID = 'm'
source_filename = "m"

define void @main() {
entry:
  %a = alloca i32, align 4
  store i32 2, ptr %a, align 4
  %x = alloca i32, align 4
  %0 = load i32, ptr %a, align 4
  %1 = sub i32 3, %0
  store i32 %1, ptr %x, align 4
  %y = alloca i32, align 4
  %2 = load i32, ptr %x, align 4
  store i32 %2, ptr %y, align 4
  %g = alloca i32, align 4
  store i32 5, ptr %g, align 4
  %other = alloca i32, align 4
  store i32 100, ptr %other, align 4
  %getseven = alloca ptr, align 8
  store ptr @0, ptr %getseven, align 8
  %main = alloca ptr, align 8
  store ptr @1, ptr %main, align 8
}

define i32 @0() {
entry:
  ret i32 300
}

define void @1() {
entry:
  %thing = alloca i32, align 4
  store i32 10, ptr %thing, align 4
  %inner = alloca ptr, align 8
  store ptr @2, ptr %inner, align 8
  %next = alloca i32, align 4
  store i32 9, ptr %next, align 4
}

define void @2() {
entry:
  %otheragain = alloca i32, align 4
  store i32 20, ptr %otheragain, align 4
  %deepinner = alloca ptr, align 8
  store ptr @3, ptr %deepinner, align 8
  %deepinnerother = alloca ptr, align 8
  store ptr @4, ptr %deepinnerother, align 8
}

define i32 @3() {
entry:
  %foo = alloca i32, align 4
  store i32 1, ptr %foo, align 4
  %0 = load i32, ptr %foo, align 4
  ret i32 %0
}

define i32 @4() {
entry:
  %foo = alloca i32, align 4
  store i32 2, ptr %foo, align 4
  ret i32 10
}
