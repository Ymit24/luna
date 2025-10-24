; ModuleID = 'm'
source_filename = "m"

@0 = global ptr null
@1 = global ptr null
@2 = global ptr null

define i32 @main() {
entry:
  store ptr @puts, ptr @0, align 8
  store ptr @3, ptr @1, align 8
  store ptr @4, ptr @2, align 8
  %0 = load ptr, ptr @2, align 8
  %1 = call i32 %0()
  ret i32 %1
}

declare void @puts()

define i8 @3() {
entry:
  %a = alloca i8, align 1
  store i32 10, ptr %a, align 4
  %0 = load i8, ptr %a, align 1
  ret i8 %0
}

define i32 @4() {
entry:
  %0 = load ptr, ptr @1, align 8
  %1 = call i8 %0()
  ret i32 1
}
