; ModuleID = 'branch_reachability.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [2 x i8] c".\00", align 1
@.str.1 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1

; Function Attrs: noinline nounwind uwtable
define void @_wp_begin() #0 {
entry:
  %putchar = tail call i32 @putchar(i32 46) #2
  ret void
}

; Function Attrs: noinline nounwind uwtable
define i8* @_wp_end(i8* readnone %a) #0 {
entry:
  %putchar = tail call i32 @putchar(i32 46) #2
  ret i8* %a
}

; Function Attrs: nounwind uwtable
define i32 @hello(i32 %a, i32 %b, i32 %c) #1 {
entry:
  tail call void @_wp_begin()
  %cmp = icmp slt i32 %a, 0
  br i1 %cmp, label %cleanup, label %if.end

if.end:                                           ; preds = %entry
  %add = add nsw i32 %b, %a
  %cmp1 = icmp sgt i32 %add, 0
  br i1 %cmp1, label %cleanup, label %if.end.3

if.end.3:                                         ; preds = %if.end
  %cmp4 = icmp slt i32 %c, 100
  br i1 %cmp4, label %if.then.5, label %if.end.13

if.then.5:                                        ; preds = %if.end.3
  %mul = mul nsw i32 %c, %b
  %cmp6 = icmp sgt i32 %mul, 10
  br i1 %cmp6, label %cleanup, label %if.else

if.else:                                          ; preds = %if.then.5
  %add8 = add nsw i32 %a, 10
  %div = sdiv i32 %c, %b
  %cmp9 = icmp sgt i32 %div, 3
  br i1 %cmp9, label %cleanup, label %if.end.13

if.end.13:                                        ; preds = %if.else, %if.end.3
  %a.addr.0 = phi i32 [ %add8, %if.else ], [ %a, %if.end.3 ]
  %add14 = add i32 %c, %b
  %add15 = add i32 %add14, %a.addr.0
  %call = tail call i8* @_wp_end(i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str.1, i64 0, i64 0))
  br label %cleanup

cleanup:                                          ; preds = %if.else, %if.then.5, %if.end, %entry, %if.end.13
  %retval.0 = phi i32 [ %add15, %if.end.13 ], [ 0, %entry ], [ -1, %if.end ], [ 0, %if.then.5 ], [ 0, %if.else ]
  ret i32 %retval.0
}

; Function Attrs: nounwind uwtable
define i32 @hello2(i32 %a, i32 %b, i32 %c) #1 {
entry:
  tail call void @_wp_begin()
  %cmp = icmp slt i32 %a, 0
  br i1 %cmp, label %cleanup, label %if.end

if.end:                                           ; preds = %entry
  %add = add nsw i32 %b, %a
  %cmp1 = icmp sgt i32 %add, 0
  br i1 %cmp1, label %cleanup, label %if.end.3

if.end.3:                                         ; preds = %if.end
  %cmp4 = icmp slt i32 %c, 100
  br i1 %cmp4, label %if.then.5, label %if.end.10

if.then.5:                                        ; preds = %if.end.3
  %mul = mul nsw i32 %c, %b
  %cmp6 = icmp sgt i32 %mul, 10
  br i1 %cmp6, label %cleanup, label %if.end.8

if.end.8:                                         ; preds = %if.then.5
  %add9 = add nsw i32 %a, 10
  br label %if.end.10

if.end.10:                                        ; preds = %if.end.8, %if.end.3
  %a.addr.0 = phi i32 [ %add9, %if.end.8 ], [ %a, %if.end.3 ]
  %add11 = add i32 %c, %b
  %add12 = add i32 %add11, %a.addr.0
  %call = tail call i8* @_wp_end(i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str.1, i64 0, i64 0))
  br label %cleanup

cleanup:                                          ; preds = %if.then.5, %if.end, %entry, %if.end.10
  %retval.0 = phi i32 [ %add12, %if.end.10 ], [ 0, %entry ], [ -1, %if.end ], [ 0, %if.then.5 ]
  ret i32 %retval.0
}

; Function Attrs: nounwind uwtable
define i32 @hello3(i32 %a, i32 %b, i32 %c) #1 {
entry:
  tail call void @_wp_begin()
  %cmp = icmp slt i32 %a, 0
  br i1 %cmp, label %cleanup, label %if.end

if.end:                                           ; preds = %entry
  %add = add nsw i32 %b, %a
  %cmp1 = icmp sgt i32 %add, 0
  br i1 %cmp1, label %cleanup, label %if.end.3

if.end.3:                                         ; preds = %if.end
  %cmp4 = icmp slt i32 %c, 100
  br i1 %cmp4, label %if.then.5, label %if.end.10

if.then.5:                                        ; preds = %if.end.3
  %mul = mul nsw i32 %c, %b
  %cmp6 = icmp sgt i32 %mul, 10
  %.b = select i1 %cmp6, i32 1, i32 %b
  %add9 = add nsw i32 %a, 10
  br label %if.end.10

if.end.10:                                        ; preds = %if.then.5, %if.end.3
  %b.addr.1 = phi i32 [ %.b, %if.then.5 ], [ %b, %if.end.3 ]
  %a.addr.0 = phi i32 [ %add9, %if.then.5 ], [ %a, %if.end.3 ]
  %add11 = add i32 %b.addr.1, %c
  %add12 = add i32 %add11, %a.addr.0
  %call = tail call i8* @_wp_end(i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str.1, i64 0, i64 0))
  br label %cleanup

cleanup:                                          ; preds = %if.end, %entry, %if.end.10
  %retval.0 = phi i32 [ %add12, %if.end.10 ], [ 0, %entry ], [ -1, %if.end ]
  ret i32 %retval.0
}

; Function Attrs: nounwind
declare i32 @putchar(i32) #2

attributes #0 = { noinline nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.7.1 (tags/RELEASE_371/final)"}
