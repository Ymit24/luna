; ModuleID = 'm'
source_filename = "m"

@0 = global ptr null
@1 = global ptr null
@2 = private unnamed_addr constant [4 x i8] c"abc\00", align 1
@3 = global ptr null

define i32 @main() {
entry:
  store ptr @puts, ptr @0, align 8
  store ptr @4, ptr @1, align 8
  store ptr @5, ptr @3, align 8
  %0 = load ptr, ptr @3, align 8
  %1 = call i32 %0()
  ret i32 %1
}

declare void @puts()

define ptr @4() {
entry:
  %a = alloca i8, align 1
  store i32 10, ptr %a, align 4
  %str = alloca ptr, align 8
  store ptr @2, ptr %str, align 8
  %0 = load ptr, ptr @0, align 8
  call void %0()
  %1 = load ptr, ptr %str, align 8
  ret ptr %1
}

define i32 @5() {
entry:
  %0 = load ptr, ptr @1, align 8
  %1 = call ptr %0()
  ret i32 1
}
