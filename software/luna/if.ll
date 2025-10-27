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
  store i32 100, ptr %a, align 4
  %0 = load i32, ptr %a, align 4
  br i32 %0, label %if.then, label %if.end
  store ptr @8, ptr %inner, align 8
  %1 = load ptr, ptr @0, align 8
  %2 = load i8, ptr %foo, align 1
  call void (ptr, ...) %1(ptr @5, i8 %2)
  %3 = load i32, ptr %a, align 4
  br i32 %3, label %if.then1, label %if.end2

if.then:                                          ; preds = %entry
  %4 = load ptr, ptr @1, align 8
  call void %4(ptr @3)
  %foo = alloca i8, align 1
  store i8 100, ptr %foo, align 1
  %inner = alloca ptr, align 8

if.end:                                           ; preds = %if.end2, %entry
  ret i32 0

if.then1:                                         ; preds = %entry
  %5 = load ptr, ptr @1, align 8
  call void %5(ptr @6)
  br label %if.end2

if.end2:                                          ; preds = %if.then1, %entry
  br label %if.end
}

define void @8() {
entry:
  %0 = load ptr, ptr @1, align 8
  call void %0(ptr @4)
}
