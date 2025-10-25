; ModuleID = 'm'
source_filename = "m"

@0 = global ptr null
@1 = global ptr null
@2 = global ptr null
@3 = global ptr null
@4 = private unnamed_addr constant [4 x i8] c"abc\00", align 1
@5 = private unnamed_addr constant [13 x i8] c"hello world!\00", align 1
@6 = global ptr null
@7 = global ptr null
@8 = private unnamed_addr constant [10 x i8] c"deepinner\00", align 1
@9 = private unnamed_addr constant [12 x i8] c"computing..\00", align 1
@10 = private unnamed_addr constant [12 x i8] c"sum of 2+3=\00", align 1

define i32 @main() {
entry:
  store ptr @puts, ptr @0, align 8
  store ptr @getchar, ptr @1, align 8
  store ptr @putchar, ptr @2, align 8
  store ptr @11, ptr @3, align 8
  store ptr @12, ptr @6, align 8
  store ptr @13, ptr @7, align 8
  %0 = load ptr, ptr @7, align 8
  %1 = call i32 %0()
  ret i32 %1
}

declare void @puts(ptr)

declare i8 @getchar()

declare i8 @putchar(i8)

define ptr @11() {
entry:
  %a = alloca i8, align 1
  store i32 10, ptr %a, align 4
  %str = alloca ptr, align 8
  store ptr @4, ptr %str, align 8
  %0 = load ptr, ptr @0, align 8
  call void %0(ptr @5)
  %1 = load ptr, ptr %str, align 8
  ret ptr %1
}

define i32 @12(i32 %0, i32 %1) {
entry:
  %2 = add i32 %0, %1
  ret i32 %2
}

define i32 @13() {
entry:
  %0 = load ptr, ptr @3, align 8
  %1 = call ptr %0()
  %inner = alloca ptr, align 8
  store ptr @14, ptr %inner, align 8
  %2 = load ptr, ptr %inner, align 8
  call void %2()
  %3 = load ptr, ptr @0, align 8
  %4 = load ptr, ptr @3, align 8
  %5 = call ptr %4()
  call void %3(ptr %5)
  %6 = load ptr, ptr @0, align 8
  call void %6(ptr @9)
  %result = alloca i32, align 4
  %7 = load ptr, ptr @6, align 8
  %8 = call i32 %7(i32 2, i32 3)
  store i32 %8, ptr %result, align 4
  %9 = load ptr, ptr @2, align 8
  %10 = call i8 %9(i32 52)
  %11 = load ptr, ptr @0, align 8
  call void %11(ptr @10)
  %12 = load ptr, ptr @2, align 8
  %13 = load i32, ptr %result, align 4
  %14 = add i32 %13, 48
  %15 = call i8 %12(i32 %14)
  %16 = load ptr, ptr @2, align 8
  %17 = call i8 %16(i32 0)
  ret i32 1
}

define void @14() {
entry:
  %deepinner = alloca ptr, align 8
  store ptr @15, ptr %deepinner, align 8
  %0 = load ptr, ptr %deepinner, align 8
  call void %0()
  ret void
}

define void @15() {
entry:
  %0 = load ptr, ptr @0, align 8
  call void %0(ptr @8)
  ret void
}
