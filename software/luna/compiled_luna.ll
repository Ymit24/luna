; ModuleID = 'm'
source_filename = "m"

@0 = global ptr null
@1 = global ptr null
@2 = global ptr null
@3 = global ptr null
@4 = private unnamed_addr constant [14 x i8] c"pointer test.\00", align 1
@5 = private unnamed_addr constant [13 x i8] c"hello world!\00", align 1

define i32 @main() {
entry:
  store ptr @puts, ptr @0, align 8
  store ptr @putchar, ptr @1, align 8
  store ptr @6, ptr @2, align 8
  store ptr @7, ptr @3, align 8
  %0 = load ptr, ptr @3, align 8
  %1 = call i32 %0()
  ret i32 %1
}

declare void @puts(ptr)

declare i8 @putchar(i8)

define void @6(ptr %0) {
entry:
  store i32 52, ptr %0, align 4
  ret void
}

define i32 @7() {
entry:
  %0 = load ptr, ptr @0, align 8
  call void %0(ptr @4)
  %var = alloca i8, align 1
  store i32 48, ptr %var, align 4
  %1 = load ptr, ptr @1, align 8
  %2 = load i8, ptr %var, align 1
  %3 = call i8 %1(i8 %2)
  %4 = load ptr, ptr @2, align 8
  call void %4(ptr %var)
  %5 = load ptr, ptr @1, align 8
  %6 = load i8, ptr %var, align 1
  %7 = call i8 %5(i8 %6)
  %test = alloca ptr, align 8
  store ptr @5, ptr %test, align 8
  %8 = load ptr, ptr @0, align 8
  %9 = load ptr, ptr %test, align 8
  call void %8(ptr %9)
  ret i32 1
}
