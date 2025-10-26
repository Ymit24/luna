; ModuleID = 'm'
source_filename = "m"

@0 = global ptr null
@1 = global ptr null
@2 = private unnamed_addr constant [19 x i8] c"a: %d -- %d -- %p\0A\00", align 1
@3 = private unnamed_addr constant [19 x i8] c"a: %d -- %d -- %p\0A\00", align 1
@4 = private unnamed_addr constant [13 x i8] c"b: %d -- %p\0A\00", align 1
@5 = private unnamed_addr constant [13 x i8] c"b: %d -- %p\0A\00", align 1

define i32 @main() {
entry:
  store ptr @printf, ptr @0, align 8
  store ptr @6, ptr @1, align 8
  %0 = load ptr, ptr @1, align 8
  %1 = call i32 %0()
  ret i32 %1
}

declare void @printf(ptr, ...)

define i32 @6() {
entry:
  %foo = alloca i8, align 1
  store i32 21, ptr %foo, align 4
  %a = alloca ptr, align 8
  store ptr %foo, ptr %a, align 8
  %0 = load ptr, ptr @0, align 8
  %1 = load ptr, ptr %a, align 8
  %2 = load ptr, ptr %a, align 8
  %3 = load ptr, ptr %2, align 8
  call void (ptr, ...) %0(ptr @2, ptr %1, ptr %3, ptr %a)
  %4 = load ptr, ptr @0, align 8
  %5 = load ptr, ptr %a, align 8
  %6 = load ptr, ptr %a, align 8
  %7 = load ptr, ptr %6, align 8
  call void (ptr, ...) %4(ptr @3, ptr %5, ptr %7, ptr %a)
  %b = alloca i32, align 4
  store i32 200, ptr %b, align 4
  %8 = load ptr, ptr @0, align 8
  %9 = load i32, ptr %b, align 4
  call void (ptr, ...) %8(ptr @4, i32 %9, ptr %b)
  store i32 30, ptr %b, align 4
  %10 = load ptr, ptr @0, align 8
  %11 = load i32, ptr %b, align 4
  call void (ptr, ...) %10(ptr @5, i32 %11, ptr %b)
  ret i32 1
}
