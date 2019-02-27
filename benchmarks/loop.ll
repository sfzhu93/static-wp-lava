; ModuleID = 'loop.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [2 x i8] c".\00", align 1
@.str.1 = private unnamed_addr constant [4 x i8] c"119\00", align 1

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
  %call = tail call i32 @hello(i32 100, i32 100000, i32 100)
  ret i32 %call
}

; Function Attrs: nounwind uwtable
define i32 @hello(i32 %a, i32 %b, i32 %c) #1 {
entry:
  tail call void @_wp_begin()
  %cmp.19 = icmp slt i32 %a, %b
  br i1 %cmp.19, label %for.body.lr.ph, label %for.cond.cleanup

for.body.lr.ph:                                   ; preds = %entry
  %add = add nsw i32 %b, %a
  %div = sdiv i32 %add, 2
  br label %for.body

for.cond.cleanup:                                 ; preds = %for.inc, %entry
  %c.addr.0.lcssa = phi i32 [ %c, %entry ], [ %c.addr.1, %for.inc ]
  %call6 = tail call i8* @_wp_end(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i64 0, i64 0))
  ret i32 %c.addr.0.lcssa

for.body:                                         ; preds = %for.inc, %for.body.lr.ph
  %i.021 = phi i32 [ %a, %for.body.lr.ph ], [ %inc, %for.inc ]
  %c.addr.020 = phi i32 [ %c, %for.body.lr.ph ], [ %c.addr.1, %for.inc ]
  %cmp1 = icmp sgt i32 %i.021, %div
  br i1 %cmp1, label %if.then, label %if.else

if.then:                                          ; preds = %for.body
  %sub = add nsw i32 %i.021, -1
  %call = tail call i32 @f(i32 %i.021, i32 %sub)
  %mul = mul nsw i32 %call, %a
  br label %for.inc

if.else:                                          ; preds = %for.body
  %add3 = add nsw i32 %i.021, 1
  %call4 = tail call i32 @f(i32 %i.021, i32 %add3)
  br label %for.inc

for.inc:                                          ; preds = %if.then, %if.else
  %mul.pn = phi i32 [ %mul, %if.then ], [ %call4, %if.else ]
  %c.addr.1 = add nsw i32 %mul.pn, %c.addr.020
  %inc = add nsw i32 %i.021, 1
  %exitcond = icmp eq i32 %inc, %b
  br i1 %exitcond, label %for.cond.cleanup, label %for.body
}

; Function Attrs: nounwind readnone uwtable
define i32 @f(i32 %a, i32 %b) #2 {
entry:
  %div = sdiv i32 %a, %b
  %rem = srem i32 %a, %b
  %add = add nsw i32 %rem, %div
  ret i32 %add
}

; Function Attrs: nounwind
declare i32 @putchar(i32) #3

attributes #0 = { noinline nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind readnone uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.7.1 (tags/RELEASE_371/final)"}
