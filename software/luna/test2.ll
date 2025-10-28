; ModuleID = 'm'
source_filename = "m"

@0 = global ptr null
@1 = global ptr null
@2 = global i32 0
@3 = global ptr null
@4 = private unnamed_addr constant [12 x i8] c"hello world\00", align 1
@5 = private unnamed_addr constant [10 x i8] c"1. b: %d\0A\00", align 1
@6 = private unnamed_addr constant [10 x i8] c"2. b: %d\0A\00", align 1
@7 = private unnamed_addr constant [10 x i8] c"3. c: %d\0A\00", align 1
@8 = private unnamed_addr constant [10 x i8] c"4. f: %d\0A\00", align 1

define i32 @main() {
entry:
  store ptr @puts, ptr @0, align 8
  store ptr @printf, ptr @1, align 8
  store i32 100, ptr @2, align 4
  %0 = alloca ptr, align 8
  store ptr @9, ptr %0, align 8
  store ptr %0, ptr @3, align 8
  %1 = load ptr, ptr @3, align 8
  %2 = call i32 %1()
  ret i32 0
}

declare i32 @puts(ptr)

declare i32 @printf(ptr, ...)

define void @9() {
entry:
  %0 = load ptr, ptr @0, align 8
  %1 = call i32 %0(ptr @4)
  %b = alloca i32, align 4
  store i32 30, ptr %b, align 4
  %2 = load ptr, ptr @1, align 8
  %3 = load i32, ptr %b, align 4
  %4 = call i32 (ptr, ...) %2(ptr @5, i32 %3)
  store i32 25, ptr %b, align 4
  %5 = load ptr, ptr @1, align 8
  %6 = load i32, ptr %b, align 4
  %7 = call i32 (ptr, ...) %5(ptr @6, i32 %6)
  %c = alloca i32, align 4
  %8 = load i32, ptr @2, align 4
  %9 = load i32, ptr %b, align 4
  %10 = add i32 %8, %9
  store i32 %10, ptr %c, align 4
  %11 = load ptr, ptr @1, align 8
  %12 = load i32, ptr %c, align 4
  %13 = call i32 (ptr, ...) %11(ptr @7, i32 %12)
  %v = alloca ptr, align 8
  %14 = alloca ptr, align 8
  store ptr @10, ptr %14, align 8
  store ptr %14, ptr %v, align 8
  %f = alloca ptr, align 8
  %15 = alloca ptr, align 8
  store ptr @11, ptr %15, align 8
  store ptr %15, ptr %f, align 8
  %16 = load ptr, ptr @1, align 8
  %17 = load ptr, ptr %f, align 8
  %18 = alloca ptr, align 8
  store ptr @12, ptr %18, align 8
  %19 = call i32 %17(ptr %18)
  %20 = call i32 (ptr, ...) %16(ptr @8, i32 %19)
  ret void
}

define i32 @10() {
entry:
  ret i32 3
}

define i32 @11(ptr %0) {
entry:
  %1 = load ptr, ptr %0, align 8
  %2 = call i32 %1()
  %3 = add i32 1, %2
  ret i32 %3
}

define i32 @12() {
entry:
  ret i32 7
}
