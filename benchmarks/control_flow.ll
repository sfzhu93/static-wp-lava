; ModuleID = 'control_flow.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [2 x i8] c".\00", align 1
@.str.1 = private unnamed_addr constant [4 x i8] c"119\00", align 1
@.str.3 = private unnamed_addr constant [41 x i8] c"(\22C should be greater than 45\22, c >= 45)\00", align 1
@.str.4 = private unnamed_addr constant [15 x i8] c"control_flow.c\00", align 1
@__PRETTY_FUNCTION__.hello = private unnamed_addr constant [25 x i8] c"int hello(int, int, int)\00", align 1

; Function Attrs: noinline nounwind uwtable
define void @_wp_begin() #0 {
entry:
  %putchar = tail call i32 @putchar(i32 46) #3
  ret void
}

; Function Attrs: noinline nounwind uwtable
define i8* @_wp_end(i8* readnone %a) #0 {
entry:
  %putchar = tail call i32 @putchar(i32 46) #3
  ret i8* %a
}

; Function Attrs: nounwind uwtable
define i32 @main() #1 {
entry:
  %call = tail call i32 @hello(i32 1, i32 1, i32 1)
  ret i32 %call
}

; Function Attrs: nounwind uwtable
define i32 @hello(i32 %a, i32 %b, i32 %c) #1 {
entry:
  tail call void @_wp_begin()
  %add = add nsw i32 %a, 5
  %add1 = add nsw i32 %add, %b
  %cmp = icmp sgt i32 %add1, 2
  %mul = mul nsw i32 %add1, %b
  %mul2 = mul nsw i32 %mul, %c
  %mul3 = mul nsw i32 %c, %b
  %mul4 = mul nsw i32 %mul3, %add1
  %b.addr.0 = select i1 %cmp, i32 %mul, i32 %mul4
  %c.addr.0 = select i1 %cmp, i32 %mul2, i32 %mul3
  %mul5 = mul nsw i32 %b.addr.0, %add1
  %mul6 = mul nsw i32 %mul5, %c.addr.0
  %call = tail call i8* @_wp_end(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i64 0, i64 0))
  %cmp7 = icmp sgt i32 %mul6, 44
  br i1 %cmp7, label %cond.end, label %cond.false

cond.false:                                       ; preds = %entry
  tail call void @__assert_fail(i8* getelementptr inbounds ([41 x i8], [41 x i8]* @.str.3, i64 0, i64 0), i8* getelementptr inbounds ([15 x i8], [15 x i8]* @.str.4, i64 0, i64 0), i32 24, i8* getelementptr inbounds ([25 x i8], [25 x i8]* @__PRETTY_FUNCTION__.hello, i64 0, i64 0)) #4
  unreachable

cond.end:                                         ; preds = %entry
  ret i32 %mul6
}

; Function Attrs: noreturn nounwind
declare void @__assert_fail(i8*, i8*, i32, i8*) #2

; Function Attrs: nounwind
declare i32 @putchar(i32) #3

attributes #0 = { noinline nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { noreturn nounwind "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }
attributes #4 = { noreturn nounwind }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.7.1 (tags/RELEASE_371/final)"}
