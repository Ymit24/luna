; ModuleID = 'm'
source_filename = "m"

@0 = global ptr null
@1 = global ptr null
@2 = private unnamed_addr constant [12 x i8] c"hello world\00", align 1

define i32 @main() {
entry:
  store ptr @puts, ptr @0, align 8
  store ptr @3, ptr @1, align 8
  %0 = load ptr, ptr @1, align 8
  %1 = call i32 %0()
  ret i32 0
}

declare void @puts(ptr)

define void @3() {
entry:
  %0 = load ptr, ptr @0, align 8
  call void %0(ptr @2)
  ret void
}
