; ModuleID = 'benchmarks/lava1.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@.str = private unnamed_addr constant [2 x i8] c".\00", align 1
@lava_val = internal unnamed_addr global [1000000 x i32] zeroinitializer, align 16
@.str.1 = private unnamed_addr constant [46 x i8] c"Successfully triggered bug %d, crashing now!\0A\00", align 1
@.str.2 = private unnamed_addr constant [2 x i8] c"1\00", align 1

; Function Attrs: noinline nounwind uwtable
define void @_wp_begin() #0 {
  %putchar = tail call i32 @putchar(i32 46) #3
  ret void
}

; Function Attrs: noinline nounwind uwtable
define i8* @_wp_end(i8* readnone %a) #0 {
  %putchar = tail call i32 @putchar(i32 46) #3
  ret i8* %a
}

; Function Attrs: nounwind uwtable
define i32 @lava_get(i32 %bug_num) #1 {
  %1 = sub i32 1818326625, %bug_num
  %2 = zext i32 %bug_num to i64
  %3 = getelementptr inbounds [1000000 x i32], [1000000 x i32]* @lava_val, i64 0, i64 %2
  %4 = load i32, i32* %3, align 4, !tbaa !1
  %5 = icmp eq i32 %1, %4
  %6 = tail call i32 @llvm.bswap.i32(i32 %1)
  %7 = icmp eq i32 %6, %4
  %or.cond = or i1 %5, %7
  br i1 %or.cond, label %8, label %10

; <label>:8                                       ; preds = %0
  %9 = tail call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([46 x i8], [46 x i8]* @.str.1, i64 0, i64 0), i32 %bug_num) #3
  br label %10

; <label>:10                                      ; preds = %0, %8
  %11 = load i32, i32* %3, align 4, !tbaa !1
  ret i32 %11
}

declare i32 @dprintf(i32, i8*, ...) #2

; Function Attrs: nounwind uwtable
define void @lava_set(i32 %bug_num, i32 %val) #1 {
  %1 = zext i32 %bug_num to i64
  %2 = getelementptr inbounds [1000000 x i32], [1000000 x i32]* @lava_val, i64 0, i64 %1
  store i32 %val, i32* %2, align 4, !tbaa !1
  ret void
}

; Function Attrs: nounwind uwtable
define i32 @main() #1 {
  %1 = tail call i32 @hello(i32 1, i32 1, i32 1)
  ret i32 %1
}

; Function Attrs: nounwind uwtable
define i32 @hello(i32 %a, i32 %b, i32 %c) #1 {
  tail call void @_wp_begin()
  %1 = add nsw i32 %a, 5
  %2 = add nsw i32 %1, %b
  tail call void @lava_set(i32 1, i32 %c)
  %3 = mul nsw i32 %2, %b
  %4 = mul i32 %2, %c
  %5 = mul i32 %4, %3
  %6 = mul i32 %5, %3
  %7 = tail call i32 @lava_get(i32 1)
  %8 = icmp eq i32 %7, 10
  %9 = select i1 %8, i32 1234567, i32 0
  %10 = add nsw i32 %9, %6
  %11 = tail call i8* @_wp_end(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.2, i64 0, i64 0))
  ret i32 %10
}

; Function Attrs: nounwind
declare i32 @putchar(i32) #3

; Function Attrs: nounwind readnone
declare i32 @llvm.bswap.i32(i32) #4

attributes #0 = { noinline nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }
attributes #4 = { nounwind readnone }

!llvm.ident = !{!0}

!0 = !{!"Ubuntu clang version 3.7.1-2ubuntu2 (tags/RELEASE_371/final) (based on LLVM 3.7.1)"}
!1 = !{!2, !2, i64 0}
!2 = !{!"int", !3, i64 0}
!3 = !{!"omnipotent char", !4, i64 0}
!4 = !{!"Simple C/C++ TBAA"}
