; ModuleID = 'herons_formula.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [2 x i8] c".\00", align 1
@.str.1 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1

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
define double @heron(double %a, double %b, double %c) #1 {
entry:
  tail call void @_wp_begin()
  %add = fadd double %a, %b
  %cmp = fcmp ogt double %add, %c
  %add4 = fadd double %b, %c
  %cmp5 = fcmp ogt double %add4, %a
  %or.cond = and i1 %cmp, %cmp5
  %add8 = fadd double %a, %c
  %cmp9 = fcmp ogt double %add8, %b
  %or.cond40 = and i1 %cmp9, %or.cond
  br i1 %or.cond40, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %add1 = fadd double %add, %c
  %mul = fmul double %add1, 5.000000e-01
  %conv = fptrunc double %mul to float
  %conv11 = fpext float %conv to double
  %sub = fsub double %conv11, %a
  %mul13 = fmul double %conv11, %sub
  %sub15 = fsub double %conv11, %b
  %mul16 = fmul double %sub15, %mul13
  %sub18 = fsub double %conv11, %c
  %mul19 = fmul double %sub18, %mul16
  %call = tail call double @sqrt(double %mul19) #3
  %conv20 = fptrunc double %call to float
  %phitmp = fpext float %conv20 to double
  br label %if.end

if.end:                                           ; preds = %entry, %if.then
  %area.0 = phi double [ %phitmp, %if.then ], [ 0.000000e+00, %entry ]
  %call21 = tail call i8* @_wp_end(i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str.1, i64 0, i64 0))
  ret double %area.0
}

; Function Attrs: nounwind
declare double @sqrt(double) #2

; Function Attrs: nounwind
declare i32 @putchar(i32) #3

attributes #0 = { noinline nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.7.1 (tags/RELEASE_371/final)"}
