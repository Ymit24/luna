; ModuleID = 'm'
source_filename = "m"

@a = external global i32
@x = external global i32
@y = external global i32
@g = external global i32
@other = external global i32
@getseven = external global ptr
@putsa = external global ptr
@bigger = external global i32
@main = external global ptr

define void @global_module_intializer() {
entry:
  store i32 2, ptr @a, align 4
  %0 = load i32, ptr @a, align 4
  %1 = sub i32 3, %0
  store i32 %1, ptr @x, align 4
  %2 = load i32, ptr @x, align 4
  store i32 %2, ptr @y, align 4
  store i32 5, ptr @g, align 4
  store i32 100, ptr @other, align 4
  store ptr @0, ptr @getseven, align 8
  store ptr @puts, ptr @putsa, align 8
  %3 = call i32 @getseven()
  store i32 %3, ptr @bigger, align 4
  store ptr @1, ptr @main, align 8
}

define i32 @0() {
entry:
  ret i32 300
}

declare i32 @puts()

define void @1() {
entry:
  %two = alloca i32, align 4
  %0 = call i32 @getseven()
  store i32 %0, ptr %two, align 4
  %three = alloca i32, align 4
  %1 = call i32 @putsa()
  store i32 %1, ptr %three, align 4
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
  %abcab = alloca i32, align 4
  %0 = call i32 %deepinner()
  store i32 %0, ptr %abcab, align 4
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
