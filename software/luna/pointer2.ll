; ModuleID = 'm'
source_filename = "m"

@0 = global ptr null
@1 = global ptr null
@2 = global ptr null
@3 = private unnamed_addr constant [32 x i8] c"a: %p -- %d -- %p -- %p ( %d )\0A\00", align 1
@4 = private unnamed_addr constant [5 x i8] c"test\00", align 1
@5 = private unnamed_addr constant [13 x i8] c"1. a: %p %d\0A\00", align 1
@6 = private unnamed_addr constant [13 x i8] c"2. a: %d %d\0A\00", align 1
@7 = private unnamed_addr constant [10 x i8] c"3. a: %d\0A\00", align 1
@8 = private unnamed_addr constant [9 x i8] c"foo: %d\0A\00", align 1
@9 = private unnamed_addr constant [13 x i8] c"b: %d -- %p\0A\00", align 1
@10 = private unnamed_addr constant [13 x i8] c"b: %d -- %p\0A\00", align 1

define i32 @main() {
entry:
  store ptr @printf, ptr @0, align 8
  store ptr @puts, ptr @1, align 8
  store ptr @11, ptr @2, align 8
  %0 = load ptr, ptr @2, align 8
  %1 = call i32 %0()
  ret i32 %1
}

declare void @printf(ptr, ...)

declare void @puts(ptr)

define i32 @11() {
entry:
  %foo = alloca i32, align 4
  store i32 21, ptr %foo, align 4
  %a = alloca ptr, align 8
  store ptr %foo, ptr %a, align 8
  %c = alloca ptr, align 8
  store ptr %a, ptr %c, align 8
  %0 = load ptr, ptr @0, align 8
  %1 = load ptr, ptr %a, align 8
  %2 = load ptr, ptr %a, align 8
  %3 = load i32, ptr %2, align 4
  %4 = load i32, ptr %foo, align 4
  call void (ptr, ...) %0(ptr @3, ptr %1, i32 %3, ptr %a, ptr %foo, i32 %4)
  %5 = load ptr, ptr @1, align 8
  call void %5(ptr @4)
  %6 = load ptr, ptr @0, align 8
  %7 = load ptr, ptr %a, align 8
  %8 = load ptr, ptr %a, align 8
  %9 = load i32, ptr %8, align 4
  call void (ptr, ...) %6(ptr @5, ptr %7, i32 %9)
  %10 = load ptr, ptr %a, align 8
  store i32 102, ptr %10, align 4
  %11 = load ptr, ptr @0, align 8
  %12 = load ptr, ptr %a, align 8
  %13 = load ptr, ptr %a, align 8
  %14 = load i32, ptr %13, align 4
  call void (ptr, ...) %11(ptr @6, ptr %12, i32 %14)
  store ptr inttoptr (i32 101 to ptr), ptr %a, align 8
  %15 = load ptr, ptr @0, align 8
  %16 = load ptr, ptr %a, align 8
  call void (ptr, ...) %15(ptr @7, ptr %16)
  %17 = load ptr, ptr @0, align 8
  %18 = load i32, ptr %foo, align 4
  call void (ptr, ...) %17(ptr @8, i32 %18)
  %b = alloca i32, align 4
  store i32 200, ptr %b, align 4
  %19 = load ptr, ptr @0, align 8
  %20 = load i32, ptr %b, align 4
  call void (ptr, ...) %19(ptr @9, i32 %20, ptr %b)
  %21 = load ptr, ptr @0, align 8
  %22 = load i32, ptr %b, align 4
  call void (ptr, ...) %21(ptr @10, i32 %22, ptr %b)
  ret i32 1
}
