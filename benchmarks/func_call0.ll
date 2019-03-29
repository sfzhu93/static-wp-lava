; ModuleID = 'func_call0.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [2 x i8] c".\00", align 1
@.str.1 = private unnamed_addr constant [2 x i8] c"1\00", align 1

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

; Function Attrs: nounwind readnone uwtable
define i32 @max2(i32 %a, i32 %b) #1 {
entry:
  %cmp = icmp sgt i32 %a, %b
  %a.b = select i1 %cmp, i32 %a, i32 %b
  ret i32 %a.b
}

; Function Attrs: nounwind uwtable
define i32 @hello(i32 %a, i32 %b, i32 %c) #2 {
entry:
  tail call void @_wp_begin()
  %cmp = icmp sgt i32 %c, 0
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  %call = tail call i32 @max2(i32 %a, i32 %b)
  br label %if.end

if.else:                                          ; preds = %entry
  %sub = sub nsw i32 0, %a
  %sub1 = sub nsw i32 0, %b
  %call2 = tail call i32 @max2(i32 %sub, i32 %sub1)
  %sub3 = sub nsw i32 0, %call2
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  %ret.0 = phi i32 [ %call, %if.then ], [ %sub3, %if.else ]
  %call4 = tail call i8* @_wp_end(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.1, i64 0, i64 0))
  ret i32 %ret.0
}

; Function Attrs: nounwind
declare i32 @putchar(i32) #3

attributes #0 = { noinline nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.7.1 (tags/RELEASE_371/final)"}
