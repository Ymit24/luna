; ModuleID = 'm'
source_filename = "m"

@0 = global ptr null
@1 = global ptr null
@2 = global ptr null
@3 = private unnamed_addr constant [11 x i8] c"it worked!\00", align 1
@4 = private unnamed_addr constant [5 x i8] c"test\00", align 1
@5 = private unnamed_addr constant [11 x i8] c"foo is %d\0A\00", align 1
@6 = private unnamed_addr constant [6 x i8] c"other\00", align 1

define i32 @main() {
entry:
  store ptr @printf, ptr @0, align 8
  store ptr @puts, ptr @1, align 8
  store ptr @7, ptr @2, align 8
  %0 = load ptr, ptr @2, align 8
  %1 = call i32 %0()
  ret i32 %1
}

declare void @printf(ptr, ...)

declare void @puts(ptr)

define i32 @7() {
entry:
  %a = alloca i32, align 4
  store i32 1, ptr %a, align 4
  %0 = load i32, ptr %a, align 4
  %1 = trunc i32 %0 to i1
  br i1 %1, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %2 = load ptr, ptr @1, align 8
  call void %2(ptr @3)
  %foo = alloca i8, align 1
  store i8 100, ptr %foo, align 1
  %inner = alloca ptr, align 8
  store ptr @8, ptr %inner, align 8
  %3 = load ptr, ptr @0, align 8
  %4 = load i8, ptr %foo, align 1
  call void (ptr, ...) %3(ptr @5, i8 %4)
  %5 = load ptr, ptr %inner, align 8
  call void %5()
  %6 = load i32, ptr %a, align 4
  %7 = trunc i32 %6 to i1
  br i1 %7, label %if.then1, label %if.end2

if.end:                                           ; preds = %if.end2, %entry
  ret i32 0

if.then1:                                         ; preds = %if.then
  %8 = load ptr, ptr @1, align 8
  call void %8(ptr @6)
  br label %if.end2

if.end2:                                          ; preds = %if.then1, %if.then
  br label %if.end
}

define void @8() {
entry:
  %0 = load ptr, ptr @1, align 8
  call void %0(ptr @4)
  ret void
}
