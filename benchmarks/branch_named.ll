; ModuleID = 'benchmarks/branch_named.bc'
source_filename = "benchmarks/branch.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@.str.1 = private unnamed_addr constant [2 x i8] c"1\00", align 1

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

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none) uwtable
define dso_local i32 @f(i32 noundef %a) local_unnamed_addr #1 {
entry:
  %add = add nsw i32 %a, 1
  ret i32 %add
}

; Function Attrs: nofree nounwind uwtable
define dso_local i32 @hello(i32 noundef %a, i32 noundef %b, i32 noundef %c) local_unnamed_addr #2 {
entry:
  tail call void @_wp_begin()
  %add = add nsw i32 %c, %b
  %cmp = icmp sgt i32 %add, 2
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  %sub = add nsw i32 %add, -1
  %sub1 = add nsw i32 %b, -1
  %sub2 = add nsw i32 %c, -1
  %mul = mul i32 %sub2, %sub1
  %mul3 = mul i32 %mul, %sub
  br label %if.end

if.else:                                          ; preds = %entry
  %add.i = add nsw i32 %add, 1
  %mul4 = mul nsw i32 %add.i, %b
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  %b.addr.0 = phi i32 [ %mul3, %if.then ], [ %mul4, %if.else ]
  %mul5 = mul i32 %add, %c
  %mul6 = mul i32 %mul5, %b.addr.0
  %call7 = tail call ptr @_wp_end(ptr noundef nonnull @.str.1)
  ret i32 %mul6
}

; Function Attrs: nofree nounwind
declare noundef i32 @putchar(i32 noundef) local_unnamed_addr #3

attributes #0 = { nofree noinline nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { nofree nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{!"Debian clang version 16.0.6 (15~deb12u1)"}
