; ModuleID = 'switch_tax_calc.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [2 x i8] c".\00", align 1
@.str.1 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@switch.table = private unnamed_addr constant [12 x double] [double 1.500000e-01, double 2.500000e-01, double 2.500000e-01, double 2.800000e-01, double 2.800000e-01, double 2.800000e-01, double 3.300000e-01, double 3.300000e-01, double 3.300000e-01, double 3.300000e-01, double 3.300000e-01, double 3.300000e-01]

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
define double @calc_tax(double %taxRate, double %salary) #1 {
entry:
  tail call void @_wp_begin()
  %cmp = fcmp ogt double %salary, 0.000000e+00
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %conv = fptosi double %salary to i32
  %div = sdiv i32 %conv, 25000
  %0 = icmp ult i32 %div, 12
  br i1 %0, label %switch.lookup, label %sw.epilog

switch.lookup:                                    ; preds = %if.then
  %1 = sext i32 %div to i64
  %switch.gep = getelementptr inbounds [12 x double], [12 x double]* @switch.table, i64 0, i64 %1
  %switch.load = load double, double* %switch.gep, align 8
  br label %sw.epilog

sw.epilog:                                        ; preds = %if.then, %switch.lookup
  %taxRate.addr.0 = phi double [ %switch.load, %switch.lookup ], [ 3.500000e-01, %if.then ]
  %mul = fmul double %taxRate.addr.0, %salary
  br label %if.end

if.end:                                           ; preds = %entry, %sw.epilog
  %incomeTax.0 = phi double [ %mul, %sw.epilog ], [ -1.000000e+00, %entry ]
  %call = tail call i8* @_wp_end(i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str.1, i64 0, i64 0))
  ret double %incomeTax.0
}

; Function Attrs: nounwind
declare i32 @putchar(i32) #2

attributes #0 = { noinline nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.7.1 (tags/RELEASE_371/final)"}
