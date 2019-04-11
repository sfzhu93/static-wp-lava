# In block if.end, function calc_tax
| LLVM Instruction | Precondition |
|-----|-----|
|   %incomeTax.0 = phi double [ %mul, %sw.epilog ], [ -1.000000e+00, %entry ] | ((sw.epilog and (mul < 1234567)) or (entry and (-1.000000 < 1234567))) |
# In block sw.epilog, function calc_tax
| LLVM Instruction | Precondition |
|-----|-----|
|   br label %if.end | ((sw.epilog and (mul < 1234567)) or (entry and (-1.000000 < 1234567))) |
|   %mul = fmul double %taxRate.addr.0, %salary | ((sw.epilog and ((taxRate.addr.0 * salary) < 1234567)) or (entry and (-1.000000 < 1234567))) |
|   %taxRate.addr.0 = phi double [ %switch.load, %switch.lookup ], [ 3.500000e-01, %if.then ] | ((switch.lookup and ((sw.epilog and ((switch.load * salary) < 1234567)) or (entry and (-1.000000 < 1234567)))) or (if.then and ((sw.epilog and ((0.350000 * salary) < 1234567)) or (entry and (-1.000000 < 1234567))))) |
# In block switch.lookup, function calc_tax
| LLVM Instruction | Precondition |
|-----|-----|
|   br label %sw.epilog | ((switch.lookup and ((sw.epilog and ((switch.load * salary) < 1234567)) or (entry and (-1.000000 < 1234567)))) or (if.then and ((sw.epilog and ((0.350000 * salary) < 1234567)) or (entry and (-1.000000 < 1234567))))) |
|   %switch.load = load double* %switch.gep, align 8 | ((switch.lookup and ((sw.epilog and ((switch.gep * salary) < 1234567)) or (entry and (-1.000000 < 1234567)))) or (if.then and ((sw.epilog and ((0.350000 * salary) < 1234567)) or (entry and (-1.000000 < 1234567))))) |
|   %switch.gep = getelementptr inbounds [12 x double]* @switch.table, i64 0, i64 %tmp1 | ((switch.lookup and ((sw.epilog and ((_ * salary) < 1234567)) or (entry and (-1.000000 < 1234567)))) or (if.then and ((sw.epilog and ((0.350000 * salary) < 1234567)) or (entry and (-1.000000 < 1234567))))) |
|   %tmp1 = sext i32 %div to i64 | ((switch.lookup and ((sw.epilog and ((_ * salary) < 1234567)) or (entry and (-1.000000 < 1234567)))) or (if.then and ((sw.epilog and ((0.350000 * salary) < 1234567)) or (entry and (-1.000000 < 1234567))))) |
# In block if.then, function calc_tax
| LLVM Instruction | Precondition |
|-----|-----|
|   br i1 %tmp, label %switch.lookup, label %sw.epilog | ((tmp and (((not tmp) and ((_ * salary) < 1234567)) or (entry and (-1.000000 < 1234567)))) or (if.then and (((not tmp) and ((0.350000 * salary) < 1234567)) or (entry and (-1.000000 < 1234567))))) |
|   %tmp = icmp ult i32 %div, 12 | (((div < 12) and (((not (div < 12)) and ((_ * salary) < 1234567)) or (entry and (-1.000000 < 1234567)))) or (if.then and (((not (div < 12)) and ((0.350000 * salary) < 1234567)) or (entry and (-1.000000 < 1234567))))) |
|   %div = sdiv i32 %conv, 25000 | ((((conv / 25000) < 12) and (((not ((conv / 25000) < 12)) and ((_ * salary) < 1234567)) or (entry and (-1.000000 < 1234567)))) or (if.then and (((not ((conv / 25000) < 12)) and ((0.350000 * salary) < 1234567)) or (entry and (-1.000000 < 1234567))))) |
|   %conv = fptosi double %salary to i32 | (((((to_int salary) / 25000) < 12) and (((not (((to_int salary) / 25000) < 12)) and ((_ * salary) < 1234567)) or (entry and (-1.000000 < 1234567)))) or (if.then and (((not (((to_int salary) / 25000) < 12)) and ((0.350000 * salary) < 1234567)) or (entry and (-1.000000 < 1234567))))) |
# In block entry, function calc_tax
| LLVM Instruction | Precondition |
|-----|-----|
|   br i1 %cmp, label %if.then, label %if.end | (((((to_int salary) / 25000) < 12) and (((not (((to_int salary) / 25000) < 12)) and ((_ * salary) < 1234567)) or (entry and (-1.000000 < 1234567)))) or (cmp and (((not (((to_int salary) / 25000) < 12)) and ((0.350000 * salary) < 1234567)) or (entry and (-1.000000 < 1234567))))) |
|   %cmp = fcmp ogt double %salary, 0.000000e+00 | (((((to_int salary) / 25000) < 12) and (((not (((to_int salary) / 25000) < 12)) and ((_ * salary) < 1234567)) or (entry and (-1.000000 < 1234567)))) or ((salary ogt 0.000000) and (((not (((to_int salary) / 25000) < 12)) and ((0.350000 * salary) < 1234567)) or (entry and (-1.000000 < 1234567))))) |
