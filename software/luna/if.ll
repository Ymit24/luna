; ModuleID = 'm'
source_filename = "m"

@0 = global ptr null
@1 = global ptr null
@2 = global ptr null
@3 = global ptr null
@4 = global ptr null
@5 = private unnamed_addr constant [11 x i8] c"it worked!\00", align 1
@6 = private unnamed_addr constant [5 x i8] c"test\00", align 1
@7 = private unnamed_addr constant [11 x i8] c"foo is %d\0A\00", align 1
@8 = private unnamed_addr constant [6 x i8] c"other\00", align 1
@9 = private unnamed_addr constant [4 x i8] c"hmm\00", align 1
@10 = private unnamed_addr constant [10 x i8] c"not other\00", align 1
@11 = private unnamed_addr constant [5 x i8] c"lame\00", align 1
@12 = private unnamed_addr constant [22 x i8] c"factorial of 3 is %d\0A\00", align 1
@13 = private unnamed_addr constant [14 x i8] c"fib(6) is %d\0A\00", align 1

define i32 @main() {
entry:
  store ptr @printf, ptr @0, align 8
  store ptr @puts, ptr @1, align 8
  store ptr @14, ptr @2, align 8
  store ptr @15, ptr @3, align 8
  store ptr @16, ptr @4, align 8
  %0 = load ptr, ptr @4, align 8
  %1 = call i32 %0()
  ret i32 %1
}

declare void @printf(ptr, ...)

declare void @puts(ptr)

define i32 @14(i32 %0) {
entry:
  %1 = icmp slt i32 %0, 2
  br i1 %1, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  ret i32 1

if.else:                                          ; preds = %entry
  br label %if.end

if.end:                                           ; preds = %if.else
  %2 = load ptr, ptr @2, align 8
  %3 = sub i32 %0, 1
  %4 = call i32 %2(i32 %3)
  %5 = mul i32 %0, %4
  ret i32 %5
}

define i32 @15(i32 %0) {
entry:
  %1 = icmp slt i32 %0, 2
  br i1 %1, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  ret i32 1

if.else:                                          ; preds = %entry
  br label %if.end

if.end:                                           ; preds = %if.else
  %2 = load ptr, ptr @3, align 8
  %3 = sub i32 %0, 1
  %4 = call i32 %2(i32 %3)
  %5 = load ptr, ptr @3, align 8
  %6 = sub i32 %0, 2
  %7 = call i32 %5(i32 %6)
  %8 = add i32 %4, %7
  ret i32 %8
}

define i32 @16() {
entry:
  %a = alloca i32, align 4
  store i32 3, ptr %a, align 4
  %0 = load i32, ptr %a, align 4
  %1 = icmp slt i32 %0, 5
  br i1 %1, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  %2 = load ptr, ptr @1, align 8
  call void %2(ptr @5)
  %foo = alloca i8, align 1
  store i8 100, ptr %foo, align 1
  %inner = alloca ptr, align 8
  store ptr @17, ptr %inner, align 8
  %3 = load ptr, ptr @0, align 8
  %4 = load i8, ptr %foo, align 1
  call void (ptr, ...) %3(ptr @7, i8 %4)
  %5 = load ptr, ptr %inner, align 8
  call void %5()
  store i32 0, ptr %a, align 4
  %6 = load i32, ptr %a, align 4
  %7 = trunc i32 %6 to i1
  br i1 %7, label %if.then1, label %if.else2

if.else:                                          ; preds = %entry
  br label %if.then7

if.end:                                           ; preds = %if.then7, %if.end3
  %8 = load ptr, ptr @0, align 8
  %9 = load ptr, ptr @2, align 8
  %10 = call i32 %9(i32 8)
  call void (ptr, ...) %8(ptr @12, i32 %10)
  %11 = load ptr, ptr @0, align 8
  %12 = load ptr, ptr @3, align 8
  %13 = call i32 %12(i32 8)
  call void (ptr, ...) %11(ptr @13, i32 %13)
  ret i32 0

if.then1:                                         ; preds = %if.then
  %14 = load ptr, ptr @1, align 8
  call void %14(ptr @8)
  br label %if.end3

if.else2:                                         ; preds = %if.then
  br i1 false, label %if.then4, label %if.else5

if.end3:                                          ; preds = %if.then6, %if.then4, %if.then1
  br label %if.end

if.then4:                                         ; preds = %if.else2
  %15 = load ptr, ptr @1, align 8
  call void %15(ptr @9)
  br label %if.end3

if.else5:                                         ; preds = %if.else2
  br label %if.then6

if.then6:                                         ; preds = %if.else5
  %16 = load ptr, ptr @1, align 8
  call void %16(ptr @10)
  br label %if.end3

if.then7:                                         ; preds = %if.else
  %17 = load ptr, ptr @1, align 8
  call void %17(ptr @11)
  br label %if.end
}

define void @17() {
entry:
  %0 = load ptr, ptr @1, align 8
  call void %0(ptr @6)
  ret void
}
