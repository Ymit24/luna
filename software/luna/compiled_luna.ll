; ModuleID = 'm'
source_filename = "m"

@0 = global ptr null
@1 = global ptr null
@2 = global ptr null
@3 = global i8 0
@4 = global ptr null
@5 = private unnamed_addr constant [4 x i8] c"bar\00", align 1
@6 = global ptr null
@7 = private unnamed_addr constant [9 x i8] c"abc: %d\0A\00", align 1
@8 = private unnamed_addr constant [20 x i8] c"foo pre change: %d\0A\00", align 1
@9 = private unnamed_addr constant [21 x i8] c"foo post change: %d\0A\00", align 1
@10 = global ptr null
@11 = private unnamed_addr constant [14 x i8] c"pointer test.\00", align 1
@12 = private unnamed_addr constant [13 x i8] c"bar is '%s'\0A\00", align 1
@13 = private unnamed_addr constant [13 x i8] c"hello world!\00", align 1
@14 = private unnamed_addr constant [19 x i8] c"THIS IS REAL COOL\0A\00", align 1
@15 = private unnamed_addr constant [14 x i8] c"two %d is %d\0A\00", align 1

define i32 @main() {
entry:
  store ptr @puts, ptr @0, align 8
  store ptr @putchar, ptr @1, align 8
  store ptr @printf, ptr @2, align 8
  store i32 123, ptr @3, align 4
  store ptr @5, ptr @4, align 8
  store ptr @16, ptr @6, align 8
  store ptr @17, ptr @10, align 8
  %0 = load ptr, ptr @10, align 8
  %1 = call i32 %0()
  ret i32 %1
}

declare void @puts(ptr)

declare i8 @putchar(i8)

declare i32 @printf(ptr, ...)

define void @16(ptr %0) {
entry:
  %1 = load ptr, ptr @2, align 8
  %2 = load i8, ptr @3, align 1
  %3 = call i32 (ptr, ...) %1(ptr @7, i8 %2)
  %4 = load ptr, ptr @2, align 8
  %5 = load ptr, ptr %0, align 8
  %6 = call i32 (ptr, ...) %4(ptr @8, ptr %5)
  store i32 52, ptr %0, align 4
  %7 = load ptr, ptr @2, align 8
  %8 = load ptr, ptr %0, align 8
  %9 = call i32 (ptr, ...) %7(ptr @9, ptr %8)
  ret void
}

define i32 @17() {
entry:
  %0 = load ptr, ptr @0, align 8
  call void %0(ptr @11)
  %var = alloca i8, align 1
  store i32 48, ptr %var, align 4
  %1 = load ptr, ptr @6, align 8
  call void %1(ptr %var)
  %2 = load ptr, ptr @2, align 8
  %3 = load ptr, ptr @4, align 8
  %4 = call i32 (ptr, ...) %2(ptr @12, ptr %3)
  %5 = load ptr, ptr @1, align 8
  %6 = load i8, ptr %var, align 1
  %7 = call i8 %5(i8 %6)
  %test = alloca ptr, align 8
  store ptr @13, ptr %test, align 8
  %8 = load ptr, ptr @0, align 8
  %9 = load ptr, ptr %test, align 8
  call void %8(ptr %9)
  %10 = load ptr, ptr @2, align 8
  %11 = call i32 (ptr, ...) %10(ptr @14)
  %12 = load ptr, ptr @2, align 8
  %13 = call i32 (ptr, ...) %12(ptr @15, i32 1, i32 3)
  ret i32 1
}
