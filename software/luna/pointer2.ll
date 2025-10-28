; ModuleID = 'm'
source_filename = "m"

@0 = global ptr null
@1 = global ptr null
@2 = global ptr null
@3 = private unnamed_addr constant [12 x i8] c"hello world\00", align 1
@4 = private unnamed_addr constant [32 x i8] c"a: %p -- %d -- %p -- %p ( %d )\0A\00", align 1
@5 = private unnamed_addr constant [5 x i8] c"test\00", align 1
@6 = private unnamed_addr constant [13 x i8] c"1. a: %p %d\0A\00", align 1
@7 = private unnamed_addr constant [13 x i8] c"2. a: %d %d\0A\00", align 1
@8 = private unnamed_addr constant [10 x i8] c"3. a: %d\0A\00", align 1
@9 = private unnamed_addr constant [9 x i8] c"foo: %d\0A\00", align 1
@10 = private unnamed_addr constant [13 x i8] c"b: %d -- %p\0A\00", align 1
@11 = private unnamed_addr constant [13 x i8] c"b: %d -- %p\0A\00", align 1

define i32 @main() {
entry:
  store ptr @printf, ptr @0, align 8
  store ptr @puts, ptr @1, align 8
  store ptr @12, ptr @2, align 8
  %0 = load ptr, ptr @2, align 8
  %1 = call i32 %0()
  ret i32 %1
}

declare void @printf(ptr, ...)

declare void @puts(ptr)

define i32 @12() {
entry:
  %0 = load ptr, ptr @1, align 8
  call void %0(ptr @3)
  %foo = alloca i32, align 4
  store i32 21, ptr %foo, align 4
  %a = alloca ptr, align 8
  store ptr %foo, ptr %a, align 8
  %c = alloca ptr, align 8
  store ptr %a, ptr %c, align 8
  %1 = load ptr, ptr @0, align 8
  %2 = load ptr, ptr %a, align 8
  %3 = load ptr, ptr %a, align 8
  %4 = load i32, ptr %3, align 4
  %5 = load i32, ptr %foo, align 4
  call void (ptr, ...) %1(ptr @4, ptr %2, i32 %4, ptr %a, ptr %foo, i32 %5)
  %6 = load ptr, ptr @1, align 8
  call void %6(ptr @5)
  %7 = load ptr, ptr @0, align 8
  %8 = load ptr, ptr %a, align 8
  %9 = load ptr, ptr %a, align 8
  %10 = load i32, ptr %9, align 4
  call void (ptr, ...) %7(ptr @6, ptr %8, i32 %10)
  %11 = load ptr, ptr %a, align 8
  store i32 102, ptr %11, align 4
  %12 = load ptr, ptr @0, align 8
  %13 = load ptr, ptr %a, align 8
  %14 = load ptr, ptr %a, align 8
  %15 = load i32, ptr %14, align 4
  call void (ptr, ...) %12(ptr @7, ptr %13, i32 %15)
  store ptr inttoptr (i32 101 to ptr), ptr %a, align 8
  %16 = load ptr, ptr @0, align 8
  %17 = load ptr, ptr %a, align 8
  call void (ptr, ...) %16(ptr @8, ptr %17)
  %18 = load ptr, ptr @0, align 8
  %19 = load i32, ptr %foo, align 4
  call void (ptr, ...) %18(ptr @9, i32 %19)
  %b = alloca i32, align 4
  store i32 200, ptr %b, align 4
  %20 = load ptr, ptr @0, align 8
  %21 = load i32, ptr %b, align 4
  call void (ptr, ...) %20(ptr @10, i32 %21, ptr %b)
  %22 = load ptr, ptr @0, align 8
  %23 = load i32, ptr %b, align 4
  call void (ptr, ...) %22(ptr @11, i32 %23, ptr %b)
  ret i32 1
}
