; ModuleID = 'm'
source_filename = "m"

@0 = global ptr null
@1 = global ptr null
@2 = global ptr null
@3 = global ptr null
@4 = private unnamed_addr constant [11 x i8] c"it worked!\00", align 1
@5 = private unnamed_addr constant [5 x i8] c"test\00", align 1
@6 = private unnamed_addr constant [11 x i8] c"foo is %d\0A\00", align 1
@7 = private unnamed_addr constant [6 x i8] c"other\00", align 1
@8 = private unnamed_addr constant [4 x i8] c"hmm\00", align 1
@9 = private unnamed_addr constant [10 x i8] c"not other\00", align 1
@10 = private unnamed_addr constant [5 x i8] c"lame\00", align 1

define i32 @main() {
entry:
  store ptr @printf, ptr @0, align 8
  store ptr @puts, ptr @1, align 8
  store ptr @11, ptr @2, align 8
  store ptr @12, ptr @3, align 8
  %0 = load ptr, ptr @3, align 8
  %1 = call i32 %0()
  ret i32 %1
}

declare void @printf(ptr, ...)

declare void @puts(ptr)

define i32 @11(i32 %0) {
entry:
  %1 = load i32, i32 %0, align 4
  %2 = icmp slt i32 %1, 2
  br i1 %2, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  ret i32 1
  br label %if.end

if.else:                                          ; preds = %entry
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  %3 = load i32, i32 %0, align 4
  %4 = load ptr, ptr @2, align 8
  %5 = load i32, i32 %0, align 4
  %6 = sub i32 %5, 1
  %7 = call i32 %4(i32 %6)
  %8 = mul i32 %3, %7
  ret i32 %8
}

define i32 @12() {
entry:
  %a = alloca i32, align 4
  store i32 1, ptr %a, align 4
  %0 = load i32, ptr %a, align 4
  %1 = trunc i32 %0 to i1
  br i1 %1, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  %2 = load ptr, ptr @1, align 8
  call void %2(ptr @4)
  %foo = alloca i8, align 1
  store i8 100, ptr %foo, align 1
  %inner = alloca ptr, align 8
  store ptr @13, ptr %inner, align 8
  %3 = load ptr, ptr @0, align 8
  %4 = load i8, ptr %foo, align 1
  call void (ptr, ...) %3(ptr @6, i8 %4)
  %5 = load ptr, ptr %inner, align 8
  call void %5()
  store i32 0, ptr %a, align 4
  %6 = load i32, ptr %a, align 4
  %7 = trunc i32 %6 to i1
  br i1 %7, label %if.then1, label %if.else2

if.else:                                          ; preds = %entry
  br i1 true, label %if.then10, label %if.else11

if.end:                                           ; preds = %if.end12, %if.end3
  ret i32 0

if.then1:                                         ; preds = %if.then
  %8 = load ptr, ptr @1, align 8
  call void %8(ptr @7)
  br label %if.end3

if.else2:                                         ; preds = %if.then
  br i1 false, label %if.then4, label %if.else5

if.end3:                                          ; preds = %if.end6, %if.then1
  br label %if.end

if.then4:                                         ; preds = %if.else2
  %9 = load ptr, ptr @1, align 8
  call void %9(ptr @8)
  br label %if.end6

if.else5:                                         ; preds = %if.else2
  br i1 true, label %if.then7, label %if.else8

if.end6:                                          ; preds = %if.end9, %if.then4
  br label %if.end3

if.then7:                                         ; preds = %if.else5
  %10 = load ptr, ptr @1, align 8
  call void %10(ptr @9)
  br label %if.end9

if.else8:                                         ; preds = %if.else5
  br label %if.end9

if.end9:                                          ; preds = %if.else8, %if.then7
  br label %if.end6

if.then10:                                        ; preds = %if.else
  %11 = load ptr, ptr @1, align 8
  call void %11(ptr @10)
  br label %if.end12

if.else11:                                        ; preds = %if.else
  br label %if.end12

if.end12:                                         ; preds = %if.else11, %if.then10
  br label %if.end
}

define void @13() {
entry:
  %0 = load ptr, ptr @1, align 8
  call void %0(ptr @5)
  ret void
}
