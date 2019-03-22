; ModuleID = 'benchmarks/sort3.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [2 x i8] c".\00", align 1
@.str.1 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@.str.2 = private unnamed_addr constant [9 x i8] c"%d,%d,%d\00", align 1
@.str.3 = private unnamed_addr constant [3 x i8] c"%d\00", align 1

; Function Attrs: noinline nounwind uwtable
define void @_wp_begin() #0 {
entry:
  %putchar = tail call i32 @putchar(i32 46) #2
  ret void
}

; Function Attrs: nounwind
declare i32 @printf(i8* nocapture readonly, ...) #1

; Function Attrs: noinline nounwind uwtable
define i8* @_wp_end(i8* readnone %a) #0 {
entry:
  %putchar = tail call i32 @putchar(i32 46) #2
  ret i8* %a
}

; Function Attrs: noinline nounwind uwtable
define i32 @sort3(i32 %a, i32 %b, i32 %c) #0 {
entry:
  tail call void @_wp_begin()
  %cmp = icmp sgt i32 %a, %b
  %a.b = select i1 %cmp, i32 %a, i32 %b
  %b.a = select i1 %cmp, i32 %b, i32 %a
  %cmp1 = icmp sgt i32 %b.a, %c
  %c.addr.0 = select i1 %cmp1, i32 %b.a, i32 %c
  %a.addr.1 = select i1 %cmp1, i32 %c, i32 %b.a
  %cmp4 = icmp sgt i32 %a.b, %c.addr.0
  %c.addr.0.a.b = select i1 %cmp4, i32 %c.addr.0, i32 %a.b
  %a.b.c.addr.0 = select i1 %cmp4, i32 %a.b, i32 %c.addr.0
  %mul = mul nsw i32 %a.addr.1, 10000
  %mul7 = mul nsw i32 %c.addr.0.a.b, 100
  %add = add i32 %a.b.c.addr.0, %mul
  %add8 = add i32 %add, %mul7
  %call = tail call i8* @_wp_end(i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str.1, i64 0, i64 0))
  ret i32 %add8
}

; Function Attrs: nounwind
declare void @llvm.lifetime.start(i64, i8* nocapture) #2

; Function Attrs: nounwind
declare void @llvm.lifetime.end(i64, i8* nocapture) #2

; Function Attrs: nounwind uwtable
define i32 @main() #3 {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %c = alloca i32, align 4
  %0 = bitcast i32* %a to i8*
  call void @llvm.lifetime.start(i64 4, i8* %0) #2
  %1 = bitcast i32* %b to i8*
  call void @llvm.lifetime.start(i64 4, i8* %1) #2
  %2 = bitcast i32* %c to i8*
  call void @llvm.lifetime.start(i64 4, i8* %2) #2
  %call = call i32 (i8*, ...) @__isoc99_scanf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str.2, i64 0, i64 0), i32* nonnull %a, i32* nonnull %b, i32* nonnull %c) #2
  %3 = load i32, i32* %a, align 4, !tbaa !1
  %4 = load i32, i32* %b, align 4, !tbaa !1
  %5 = load i32, i32* %c, align 4, !tbaa !1
  %call1 = call i32 @sort3(i32 %3, i32 %4, i32 %5)
  %call2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.3, i64 0, i64 0), i32 %call1) #2
  call void @llvm.lifetime.end(i64 4, i8* %2) #2
  call void @llvm.lifetime.end(i64 4, i8* %1) #2
  call void @llvm.lifetime.end(i64 4, i8* %0) #2
  ret i32 0
}

; Function Attrs: nounwind
declare i32 @__isoc99_scanf(i8* nocapture readonly, ...) #1

; Function Attrs: nounwind
declare i32 @putchar(i32) #2

attributes #0 = { noinline nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind }
attributes #3 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.7.1 (tags/RELEASE_371/final)"}
!1 = !{!2, !2, i64 0}
!2 = !{!"int", !3, i64 0}
!3 = !{!"omnipotent char", !4, i64 0}
!4 = !{!"Simple C/C++ TBAA"}
