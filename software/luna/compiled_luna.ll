; ModuleID = 'm'
source_filename = "m"

@0 = global ptr null

define i32 @main() {
entry:
  store ptr @1, ptr @0, align 8
  %0 = load ptr, ptr @0, align 8
  %1 = call i32 %0()
  ret i32 %1
}

define i32 @1() {
entry:
  %a = alloca ptr, align 8
  store i32 100, ptr %a, align 4
  store i32 50, ptr %a, align 4
  %b = alloca i32, align 4
  store i32 200, ptr %b, align 4
  store i32 30, ptr %b, align 4
  ret i32 1
}
