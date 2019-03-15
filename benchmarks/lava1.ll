; ModuleID = 'benchmarks/lava1.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [2 x i8] c".\00", align 1
@lava_val = internal unnamed_addr global [1000000 x i32] zeroinitializer, align 16
@.str1 = private unnamed_addr constant [46 x i8] c"Successfully triggered bug %d, crashing now!\0A\00", align 1
@.str2 = private unnamed_addr constant [2 x i8] c"1\00", align 1

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
define i32 @lava_get(i32 %bug_num) #1 {
entry:
  %sub = sub i32 1818326625, %bug_num
  %idxprom = zext i32 %bug_num to i64
  %arrayidx = getelementptr inbounds [1000000 x i32]* @lava_val, i64 0, i64 %idxprom
  %0 = load i32* %arrayidx, align 4, !tbaa !1
  %cmp = icmp eq i32 %sub, %0
  %or9 = tail call i32 @llvm.bswap.i32(i32 %sub)
  %cmp12 = icmp eq i32 %or9, %0
  %or.cond = or i1 %cmp, %cmp12
  br i1 %or.cond, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %call = tail call i32 (i32, i8*, ...)* @dprintf(i32 1, i8* getelementptr inbounds ([46 x i8]* @.str1, i64 0, i64 0), i32 %bug_num) #3
  br label %if.end

if.end:                                           ; preds = %if.then, %entry
  %1 = load i32* %arrayidx, align 4, !tbaa !1
  ret i32 %1
}

declare i32 @dprintf(i32, i8*, ...) #2

; Function Attrs: nounwind uwtable
define void @lava_set(i32 %bug_num, i32 %val) #1 {
entry:
  %idxprom = zext i32 %bug_num to i64
  %arrayidx = getelementptr inbounds [1000000 x i32]* @lava_val, i64 0, i64 %idxprom
  store i32 %val, i32* %arrayidx, align 4, !tbaa !1
  ret void
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
  tail call void @lava_set(i32 1, i32 %c)
  %mul = mul nsw i32 %add1, %b
  %mul2 = mul i32 %add1, %c
  %mul3 = mul i32 %mul2, %mul
  %mul4 = mul i32 %mul3, %mul
  %call = tail call i32 @lava_get(i32 1)
  %cmp = icmp eq i32 %call, 10
  %mul5 = select i1 %cmp, i32 1234567, i32 0
  %add6 = add nsw i32 %mul5, %mul4
  %call7 = tail call i8* @_wp_end(i8* getelementptr inbounds ([2 x i8]* @.str2, i64 0, i64 0))
  ret i32 %add6
}

; Function Attrs: nounwind
declare i32 @putchar(i32) #3

; Function Attrs: nounwind readnone
declare i32 @llvm.bswap.i32(i32) #4

attributes #0 = { noinline nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }
attributes #4 = { nounwind readnone }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.6.0 (tags/RELEASE_360/final)"}
!1 = !{!2, !2, i64 0}
!2 = !{!"int", !3, i64 0}
!3 = !{!"omnipotent char", !4, i64 0}
!4 = !{!"Simple C/C++ TBAA"}
