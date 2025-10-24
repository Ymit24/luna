; ModuleID = 'm'
source_filename = "m"

@0 = global i32 null
@1 = global i32 null
@2 = global i32 null
@3 = global i32 null
@4 = global i32 null
@5 = global ptr null
@6 = global ptr null
@7 = private unnamed_addr constant [4 x i8] c"foo\00", align 1
@8 = global ptr null
@9 = global i32 null
@10 = global ptr null

define i32 @main() {
entry:
  store i32 2, ptr @0, align 4
  %0 = load i32, ptr @0, align 4
  %1 = sub i32 3, %0
  store i32 %1, ptr @1, align 4
  %2 = load i32, ptr @1, align 4
  store i32 %2, ptr @2, align 4
  store i32 5, ptr @3, align 4
  store i32 100, ptr @4, align 4
  store ptr @11, ptr @5, align 8
  store ptr @7, ptr @6, align 8
  store ptr @puts, ptr @8, align 8
  %3 = load ptr, ptr @5, align 8
  %4 = call i32 %3()
  store i32 %4, ptr @9, align 4
  store ptr @12, ptr @10, align 8
  %5 = load ptr, ptr @10, align 8
  call void %5()
  ret i32 0
}

define i32 @11() {
entry:
  ret i32 300
}

declare i32 @puts()

define void @12() {
entry:
  %two = alloca i32, align 4
  %0 = load ptr, ptr @5, align 8
  %1 = call i32 %0()
  store i32 %1, ptr %two, align 4
  %three = alloca i32, align 4
  %2 = load ptr, ptr @8, align 8
  %3 = call i32 %2()
  store i32 %3, ptr %three, align 4
  %thing = alloca i32, align 4
  store i32 10, ptr %thing, align 4
  %inner = alloca ptr, align 8
  store ptr @13, ptr %inner, align 8
  %next = alloca i32, align 4
  store i32 9, ptr %next, align 4
}

define void @13() {
entry:
  %otheragain = alloca i32, align 4
  store i32 20, ptr %otheragain, align 4
  %deepinner = alloca ptr, align 8
  store ptr @14, ptr %deepinner, align 8
  %abcab = alloca i32, align 4
  %0 = load ptr, ptr %deepinner, align 8
  %1 = call i32 %0()
  store i32 %1, ptr %abcab, align 4
  %deepinnerother = alloca ptr, align 8
  store ptr @15, ptr %deepinnerother, align 8
}

define i32 @14() {
entry:
  %foo = alloca i32, align 4
  store i32 1, ptr %foo, align 4
  %0 = load i32, ptr %foo, align 4
  ret i32 %0
}

define i32 @15() {
entry:
  %foo = alloca i32, align 4
  store i32 2, ptr %foo, align 4
  ret i32 10
}
