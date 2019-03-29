; ModuleID = 'branch.bc'
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
define i32 @f(i32 %a) #1 {
entry:
  %add = add nsw i32 %a, 1
  ret i32 %add
}

; Function Attrs: nounwind uwtable
define i32 @hello(i32 %a, i32 %b, i32 %c) #2 {
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
  %call = tail call i32 @f(i32 %add)
  %mul4 = mul nsw i32 %call, %b
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  %b.addr.0 = phi i32 [ %mul3, %if.then ], [ %mul4, %if.else ]
  %mul5 = mul i32 %add, %c
  %mul6 = mul i32 %mul5, %b.addr.0
  %call7 = tail call i8* @_wp_end(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.1, i64 0, i64 0))
  ret i32 %mul6
}

; Function Attrs: nounwind
declare i32 @putchar(i32) #3

attributes #0 = { noinline nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.7.1 (tags/RELEASE_371/final)"}
