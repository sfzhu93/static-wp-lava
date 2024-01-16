; ModuleID = 'benchmarks/hello_named.bc'
source_filename = "benchmarks/hello.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@.str.1 = private unnamed_addr constant [4 x i8] c"119\00", align 1
@.str.3 = private unnamed_addr constant [41 x i8] c"(\22C should be greater than 45\22, c >= 45)\00", align 1
@.str.4 = private unnamed_addr constant [19 x i8] c"benchmarks/hello.c\00", align 1
@__PRETTY_FUNCTION__.hello = private unnamed_addr constant [25 x i8] c"int hello(int, int, int)\00", align 1

; Function Attrs: nofree noinline nounwind uwtable
define dso_local void @_wp_begin() local_unnamed_addr #0 {
entry:
  %putchar = tail call i32 @putchar(i32 46)
  ret void
}

; Function Attrs: nofree noinline nounwind uwtable
define dso_local ptr @_wp_end(ptr noundef readnone returned %a) local_unnamed_addr #0 {
entry:
  %putchar = tail call i32 @putchar(i32 46)
  ret ptr %a
}

; Function Attrs: nofree nounwind uwtable
define dso_local i32 @main() local_unnamed_addr #1 {
entry:
  tail call void @_wp_begin()
  %call.i = tail call ptr @_wp_end(ptr noundef nonnull @.str.1)
  ret i32 343
}

; Function Attrs: nounwind uwtable
define dso_local i32 @hello(i32 noundef %a, i32 noundef %b, i32 noundef %c) local_unnamed_addr #2 {
entry:
  tail call void @_wp_begin()
  %add = add nsw i32 %a, 5
  %add1 = add nsw i32 %add, %b
  %mul = mul nsw i32 %add1, %b
  %mul2 = mul i32 %add1, %c
  %mul3 = mul i32 %mul2, %mul
  %mul4 = mul i32 %mul3, %mul
  %call = tail call ptr @_wp_end(ptr noundef nonnull @.str.1)
  %cmp = icmp sgt i32 %mul4, 44
  br i1 %cmp, label %if.end, label %if.else

if.else:                                          ; preds = %entry
  tail call void @__assert_fail(ptr noundef nonnull @.str.3, ptr noundef nonnull @.str.4, i32 noundef 18, ptr noundef nonnull @__PRETTY_FUNCTION__.hello) #5
  unreachable

if.end:                                           ; preds = %entry
  ret i32 %mul4
}

; Function Attrs: noreturn nounwind
declare void @__assert_fail(ptr noundef, ptr noundef, i32 noundef, ptr noundef) local_unnamed_addr #3

; Function Attrs: nofree nounwind
declare noundef i32 @putchar(i32 noundef) local_unnamed_addr #4

attributes #0 = { nofree noinline nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nofree nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { noreturn nounwind "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { nofree nounwind }
attributes #5 = { noreturn nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{!"Debian clang version 16.0.6 (15~deb12u1)"}
