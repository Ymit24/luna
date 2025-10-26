; ModuleID = 'm'
source_filename = "m"

@0 = global ptr null
@1 = global ptr null
@2 = private unnamed_addr constant [32 x i8] c"a: %p -- %d -- %p -- %p ( %d )\0A\00", align 1
@3 = private unnamed_addr constant [13 x i8] c"b: %d -- %p\0A\00", align 1
@4 = private unnamed_addr constant [13 x i8] c"b: %d -- %p\0A\00", align 1

define i32 @main() {
entry:
  store ptr @printf, ptr @0, align 8
  store ptr @5, ptr @1, align 8
  %0 = load ptr, ptr @1, align 8
  %1 = call i32 %0()
  ret i32 %1
}

declare void @printf(ptr, ...)

define i32 @5() {
entry:
  %foo = alloca i8, align 1
  store i8 21, ptr %foo, align 1
  %a = alloca ptr, align 8
  store ptr %foo, ptr %a, align 8
  %c = alloca ptr, align 8
  store ptr %a, ptr %c, align 8
  %0 = load ptr, ptr @0, align 8
  %1 = load ptr, ptr %a, align 8
  %2 = load ptr, ptr %a, align 8
  %3 = load ptr, ptr %2, align 8
  %4 = load i8, ptr %foo, align 1
  call void (ptr, ...) %0(ptr @2, ptr %1, ptr %3, ptr %a, ptr %foo, i8 %4)
  %b = alloca i32, align 4
  store i32 200, ptr %b, align 4
  %5 = load ptr, ptr @0, align 8
  %6 = load i32, ptr %b, align 4
  call void (ptr, ...) %5(ptr @3, i32 %6, ptr %b)
  store i32 30, ptr %b, align 4
  %7 = load ptr, ptr @0, align 8
  %8 = load i32, ptr %b, align 4
  call void (ptr, ...) %7(ptr @4, i32 %8, ptr %b)
  ret i32 1
}
