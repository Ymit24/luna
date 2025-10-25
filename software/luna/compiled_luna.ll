; ModuleID = 'm'
source_filename = "m"

@0 = global ptr null
@1 = global ptr null
@2 = global ptr null
@3 = global ptr null
@4 = global ptr null

define i32 @main() {
entry:
  store ptr @puts, ptr @0, align 8
  store ptr @putchar, ptr @1, align 8
  store ptr @printf, ptr @2, align 8
  store ptr @strlen, ptr @3, align 8
  store ptr @5, ptr @4, align 8
  %0 = load ptr, ptr @4, align 8
  %1 = call i32 %0()
  ret i32 %1
}

declare void @puts(ptr)

declare i8 @putchar(i8)

declare i32 @printf(ptr, ...)

declare i32 @strlen(ptr)

define i32 @5() {
entry:
  %a = alloca ptr, align 8
  store i32 100, ptr %a, align 4
  store i32 50, ptr %a, align 4
  %b = alloca i32, align 4
  store i32 200, ptr %b, align 4
  store i32 30, ptr %b, align 4
  ret i32 1
}
