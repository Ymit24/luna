; ModuleID = 'm'
source_filename = "m"

@0 = global ptr null
@1 = global ptr null
@2 = private unnamed_addr constant [13 x i8] c"a: %d -- %p\0A\00", align 1
@3 = private unnamed_addr constant [13 x i8] c"a: %d -- %p\0A\00", align 1
@4 = private unnamed_addr constant [13 x i8] c"b: %d -- %p\0A\00", align 1
@5 = private unnamed_addr constant [13 x i8] c"b: %d -- %p\0A\00", align 1

define i32 @main() {
entry:
  store ptr @printf, ptr @0, align 8
  store ptr @6, ptr @1, align 8
  %0 = load ptr, ptr @1, align 8
  %1 = call i32 %0()
  ret i32 %1
}

declare void @printf(ptr, ...)

define i32 @6() {
entry:
  %a = alloca ptr, align 8
  store ptr inttoptr (i32 100 to ptr), ptr %a, align 8
  %0 = load ptr, ptr @0, align 8
  call void (ptr, ...) %0(ptr @2, ptr %a, ptr %a)
  store ptr inttoptr (i32 50 to ptr), ptr %a, align 8
  %1 = load ptr, ptr @0, align 8
  call void (ptr, ...) %1(ptr @3, ptr %a, ptr %a)
  %b = alloca i32, align 4
  store i32 200, ptr %b, align 4
  %2 = load ptr, ptr @0, align 8
  call void (ptr, ...) %2(ptr @4, ptr %b, ptr %b)
  store i32 30, ptr %b, align 4
  %3 = load ptr, ptr @0, align 8
  call void (ptr, ...) %3(ptr @5, ptr %b, ptr %b)
  ret i32 1
}
