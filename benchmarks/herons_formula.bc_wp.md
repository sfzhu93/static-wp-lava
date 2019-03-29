# In block if.end, function heron
| LLVM Instruction | Precondition |
|-----|-----|
|   %area.0 = phi double [ %phitmp, %if.then ], [ 0.000000e+00, %entry ] | ((if.then and (phitmp < 1234567)) or (entry and (0.000000 < 1234567))) |
# In block if.then, function heron
| LLVM Instruction | Precondition |
|-----|-----|
|   br label %if.end | ((if.then and (phitmp < 1234567)) or (entry and (0.000000 < 1234567))) |
|   %phitmp = fpext float %conv20 to double | ((if.then and (conv20 < 1234567)) or (entry and (0.000000 < 1234567))) |
|   %conv20 = fptrunc double %call to float | ((if.then and (call < 1234567)) or (entry and (0.000000 < 1234567))) |
|   %mul19 = fmul double %sub18, %mul16 | ((if.then and (call < 1234567)) or (entry and (0.000000 < 1234567))) |
|   %sub18 = fsub double %conv11, %c | ((if.then and (call < 1234567)) or (entry and (0.000000 < 1234567))) |
|   %mul16 = fmul double %sub15, %mul13 | ((if.then and (call < 1234567)) or (entry and (0.000000 < 1234567))) |
|   %sub15 = fsub double %conv11, %b | ((if.then and (call < 1234567)) or (entry and (0.000000 < 1234567))) |
|   %mul13 = fmul double %conv11, %sub | ((if.then and (call < 1234567)) or (entry and (0.000000 < 1234567))) |
|   %sub = fsub double %conv11, %a | ((if.then and (call < 1234567)) or (entry and (0.000000 < 1234567))) |
|   %conv11 = fpext float %conv to double | ((if.then and (call < 1234567)) or (entry and (0.000000 < 1234567))) |
|   %conv = fptrunc double %mul to float | ((if.then and (call < 1234567)) or (entry and (0.000000 < 1234567))) |
|   %mul = fmul double %add1, 5.000000e-01 | ((if.then and (call < 1234567)) or (entry and (0.000000 < 1234567))) |
|   %add1 = fadd double %add, %c | ((if.then and (call < 1234567)) or (entry and (0.000000 < 1234567))) |
# In block entry, function heron
| LLVM Instruction | Precondition |
|-----|-----|
|   br i1 %or.cond40, label %if.then, label %if.end | ((or.cond40 and (call < 1234567)) or (entry and (0.000000 < 1234567))) |
|   %or.cond40 = and i1 %cmp9, %or.cond | (((cmp9 and or.cond) and (call < 1234567)) or (entry and (0.000000 < 1234567))) |
|   %cmp9 = fcmp ogt double %add8, %b | ((((add8 ogt b) and or.cond) and (call < 1234567)) or (entry and (0.000000 < 1234567))) |
|   %add8 = fadd double %a, %c | (((((a + c) ogt b) and or.cond) and (call < 1234567)) or (entry and (0.000000 < 1234567))) |
|   %or.cond = and i1 %cmp, %cmp5 | (((((a + c) ogt b) and (cmp and cmp5)) and (call < 1234567)) or (entry and (0.000000 < 1234567))) |
|   %cmp5 = fcmp ogt double %add4, %a | (((((a + c) ogt b) and (cmp and (add4 ogt a))) and (call < 1234567)) or (entry and (0.000000 < 1234567))) |
|   %add4 = fadd double %b, %c | (((((a + c) ogt b) and (cmp and ((b + c) ogt a))) and (call < 1234567)) or (entry and (0.000000 < 1234567))) |
|   %cmp = fcmp ogt double %add, %c | (((((a + c) ogt b) and ((add ogt c) and ((b + c) ogt a))) and (call < 1234567)) or (entry and (0.000000 < 1234567))) |
|   %add = fadd double %a, %b | (((((a + c) ogt b) and (((a + b) ogt c) and ((b + c) ogt a))) and (call < 1234567)) or (entry and (0.000000 < 1234567))) |
