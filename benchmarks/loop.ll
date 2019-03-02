; ModuleID = 'benchmarks/loop.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@.str = private unnamed_addr constant [2 x i8] c".\00", align 1
@.str.1 = private unnamed_addr constant [4 x i8] c"119\00", align 1

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
define i32 @main() #1 {
  %1 = tail call i32 @hello(i32 100, i32 100000, i32 100)
  ret i32 %1
}

; Function Attrs: nounwind uwtable
define i32 @hello(i32 %a, i32 %b, i32 %c) #1 {
  tail call void @_wp_begin()
  %1 = icmp slt i32 %a, %b
  br i1 %1, label %.lr.ph, label %._crit_edge

.lr.ph:                                           ; preds = %0
  %2 = add nsw i32 %b, %a
  %3 = sdiv i32 %2, 2
  br label %5

._crit_edge:                                      ; preds = %14, %0
  %.0.lcssa = phi i32 [ %c, %0 ], [ %.1, %14 ]
  %4 = tail call i8* @_wp_end(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i64 0, i64 0))
  ret i32 %.0.lcssa

; <label>:5                                       ; preds = %14, %.lr.ph
  %i.02 = phi i32 [ %a, %.lr.ph ], [ %15, %14 ]
  %.01 = phi i32 [ %c, %.lr.ph ], [ %.1, %14 ]
  %6 = icmp sgt i32 %i.02, %3
  br i1 %6, label %7, label %11

; <label>:7                                       ; preds = %5
  %8 = add nsw i32 %i.02, -1
  %9 = tail call i32 @f(i32 %i.02, i32 %8)
  %10 = mul nsw i32 %9, %a
  br label %14

; <label>:11                                      ; preds = %5
  %12 = add nsw i32 %i.02, 1
  %13 = tail call i32 @f(i32 %i.02, i32 %12)
  br label %14

; <label>:14                                      ; preds = %7, %11
  %.pn = phi i32 [ %10, %7 ], [ %13, %11 ]
  %.1 = add nsw i32 %.pn, %.01
  %15 = add nsw i32 %i.02, 1
  %exitcond = icmp eq i32 %15, %b
  br i1 %exitcond, label %._crit_edge, label %5
}

; Function Attrs: nounwind readnone uwtable
define i32 @f(i32 %a, i32 %b) #2 {
  %1 = sdiv i32 %a, %b
  %2 = srem i32 %a, %b
  %3 = add nsw i32 %2, %1
  ret i32 %3
}

; Function Attrs: nounwind
declare i32 @putchar(i32) #3

attributes #0 = { noinline nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind readnone uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.ident = !{!0}

!0 = !{!"Ubuntu clang version 3.7.1-2ubuntu2 (tags/RELEASE_371/final) (based on LLVM 3.7.1)"}
