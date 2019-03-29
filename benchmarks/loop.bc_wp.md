# In block for.end, function hello
| LLVM Instruction | Precondition |
|-----|-----|
|   %c.addr.0.lcssa = phi i32 [ %c, %entry ], [ %c.addr.1, %for.inc ] | ((entry and (c < 1234567)) or (for.inc and (c.addr.1 < 1234567))) |
# In block if.else, function hello
| LLVM Instruction | Precondition |
|-----|-----|
|   br label %for.inc | ((entry and (c < 1234567)) or (for.inc and (c.addr.1 < 1234567))) |
# In block entry, function f
| LLVM Instruction | Precondition |
|-----|-----|
|   ret i32 %add | UPred( add ) |
|   %add = add nsw i32 %rem, %div | UPred(rem + div) |
|   %rem = srem i32 %a, %b | UPred(rem + div) |
|   %div = sdiv i32 %a, %b | UPred(rem + div) |
|   %call4 = tail call i32 @f(i32 %i.020, i32 %add3) | ((entry and (c < 1234567)) or (for.inc and (c.addr.1 < 1234567))) |
|   %add3 = add nsw i32 %i.020, 1 | ((entry and (c < 1234567)) or (for.inc and (c.addr.1 < 1234567))) |
# In block for.inc, function f
| LLVM Instruction | Precondition |
|-----|-----|
|   br i1 %exitcond, label %for.end, label %for.body | ((entry and (c < 1234567)) or (for.inc and (c.addr.1 < 1234567))) |
|   %exitcond = icmp eq i32 %i.020, %tmp | ((entry and (c < 1234567)) or (for.inc and (c.addr.1 < 1234567))) |
|   %inc = add nsw i32 %i.020, 1 | ((entry and (c < 1234567)) or (for.inc and (c.addr.1 < 1234567))) |
|   %c.addr.1 = add nsw i32 %mul.pn, %c.addr.019 | ((entry and (c < 1234567)) or (for.inc and ((mul.pn + c.addr.019) < 1234567))) |
|   %mul.pn = phi i32 [ %mul, %if.then ], [ %call4, %if.else ] | ((if.then and ((entry and (c < 1234567)) or (for.inc and ((mul + c.addr.019) < 1234567)))) or (if.else and ((entry and (c < 1234567)) or (for.inc and ((call4 + c.addr.019) < 1234567))))) |
# In block if.then, function f
| LLVM Instruction | Precondition |
|-----|-----|
|   br label %for.inc | ((if.then and ((entry and (c < 1234567)) or (for.inc and ((mul + c.addr.019) < 1234567)))) or (if.else and ((entry and (c < 1234567)) or (for.inc and ((call4 + c.addr.019) < 1234567))))) |
|   %mul = mul nsw i32 %call, %a | ((if.then and ((entry and (c < 1234567)) or (for.inc and (((call * a) + c.addr.019) < 1234567)))) or (if.else and ((entry and (c < 1234567)) or (for.inc and ((call4 + c.addr.019) < 1234567))))) |
# In block entry, function f
| LLVM Instruction | Precondition |
|-----|-----|
|   ret i32 %add | UPred( add ) |
|   %add = add nsw i32 %rem, %div | UPred(rem + div) |
|   %rem = srem i32 %a, %b | UPred(rem + div) |
|   %div = sdiv i32 %a, %b | UPred(rem + div) |
|   %call = tail call i32 @f(i32 %i.020, i32 %sub) | ((if.then and ((entry and (c < 1234567)) or (for.inc and (((+ * a) + c.addr.019) < 1234567)))) or (if.else and ((entry and (c < 1234567)) or (for.inc and ((call4 + c.addr.019) < 1234567))))) |
|   %sub = add nsw i32 %i.020, -1 | ((if.then and ((entry and (c < 1234567)) or (for.inc and (((+ * a) + c.addr.019) < 1234567)))) or (if.else and ((entry and (c < 1234567)) or (for.inc and ((call4 + c.addr.019) < 1234567))))) |
# In block for.body, function f
| LLVM Instruction | Precondition |
|-----|-----|
|   br i1 %cmp1, label %if.then, label %if.else | ((cmp1 and ((entry and (c < 1234567)) or (for.inc and (((+ * a) + c.addr.019) < 1234567)))) or ((NOT cmp1) and ((entry and (c < 1234567)) or (for.inc and ((call4 + c.addr.019) < 1234567))))) |
|   %cmp1 = icmp sgt i32 %i.020, %div | (((i.020 > div) and ((entry and (c < 1234567)) or (for.inc and (((+ * a) + c.addr.019) < 1234567)))) or ((NOT (i.020 > div)) and ((entry and (c < 1234567)) or (for.inc and ((call4 + c.addr.019) < 1234567))))) |
|   %c.addr.019 = phi i32 [ %c, %for.body.lr.ph ], [ %c.addr.1, %for.inc ] | ((for.body.lr.ph and (((i.020 > div) and ((entry and (c < 1234567)) or (for.inc and (((+ * a) + c) < 1234567)))) or ((NOT (i.020 > div)) and ((entry and (c < 1234567)) or (for.inc and ((call4 + c) < 1234567)))))) or (for.inc and (((i.020 > div) and ((entry and (c < 1234567)) or (for.inc and (((+ * a) + c.addr.1) < 1234567)))) or ((NOT (i.020 > div)) and ((entry and (c < 1234567)) or (for.inc and ((call4 + c.addr.1) < 1234567))))))) |
|   %i.020 = phi i32 [ %a, %for.body.lr.ph ], [ %inc, %for.inc ] | ((for.body.lr.ph and ((for.body.lr.ph and (((a > div) and ((entry and (c < 1234567)) or (for.inc and (((+ * a) + c) < 1234567)))) or ((NOT (a > div)) and ((entry and (c < 1234567)) or (for.inc and ((call4 + c) < 1234567)))))) or (for.inc and (((a > div) and ((entry and (c < 1234567)) or (for.inc and (((+ * a) + c.addr.1) < 1234567)))) or ((NOT (a > div)) and ((entry and (c < 1234567)) or (for.inc and ((call4 + c.addr.1) < 1234567)))))))) or (for.inc and ((for.body.lr.ph and (((inc > div) and ((entry and (c < 1234567)) or (for.inc and (((+ * a) + c) < 1234567)))) or ((NOT (inc > div)) and ((entry and (c < 1234567)) or (for.inc and ((call4 + c) < 1234567)))))) or (for.inc and (((inc > div) and ((entry and (c < 1234567)) or (for.inc and (((+ * a) + c.addr.1) < 1234567)))) or ((NOT (inc > div)) and ((entry and (c < 1234567)) or (for.inc and ((call4 + c.addr.1) < 1234567))))))))) |
# In block for.body.lr.ph, function f
| LLVM Instruction | Precondition |
|-----|-----|
|   br label %for.body | ((for.body.lr.ph and ((for.body.lr.ph and (((a > div) and ((entry and (c < 1234567)) or (for.inc and (((+ * a) + c) < 1234567)))) or ((NOT (a > div)) and ((entry and (c < 1234567)) or (for.inc and ((call4 + c) < 1234567)))))) or (for.inc and (((a > div) and ((entry and (c < 1234567)) or (for.inc and (((+ * a) + c.addr.1) < 1234567)))) or ((NOT (a > div)) and ((entry and (c < 1234567)) or (for.inc and ((call4 + c.addr.1) < 1234567)))))))) or (for.inc and ((for.body.lr.ph and (((inc > div) and ((entry and (c < 1234567)) or (for.inc and (((+ * a) + c) < 1234567)))) or ((NOT (inc > div)) and ((entry and (c < 1234567)) or (for.inc and ((call4 + c) < 1234567)))))) or (for.inc and (((inc > div) and ((entry and (c < 1234567)) or (for.inc and (((+ * a) + c.addr.1) < 1234567)))) or ((NOT (inc > div)) and ((entry and (c < 1234567)) or (for.inc and ((call4 + c.addr.1) < 1234567))))))))) |
|   %tmp = add i32 %b, -1 | ((for.body.lr.ph and ((for.body.lr.ph and (((a > div) and ((entry and (c < 1234567)) or (for.inc and (((+ * a) + c) < 1234567)))) or ((NOT (a > div)) and ((entry and (c < 1234567)) or (for.inc and ((call4 + c) < 1234567)))))) or (for.inc and (((a > div) and ((entry and (c < 1234567)) or (for.inc and (((+ * a) + c.addr.1) < 1234567)))) or ((NOT (a > div)) and ((entry and (c < 1234567)) or (for.inc and ((call4 + c.addr.1) < 1234567)))))))) or (for.inc and ((for.body.lr.ph and (((inc > div) and ((entry and (c < 1234567)) or (for.inc and (((+ * a) + c) < 1234567)))) or ((NOT (inc > div)) and ((entry and (c < 1234567)) or (for.inc and ((call4 + c) < 1234567)))))) or (for.inc and (((inc > div) and ((entry and (c < 1234567)) or (for.inc and (((+ * a) + c.addr.1) < 1234567)))) or ((NOT (inc > div)) and ((entry and (c < 1234567)) or (for.inc and ((call4 + c.addr.1) < 1234567))))))))) |
|   %div = sdiv i32 %add, 2 | ((for.body.lr.ph and ((for.body.lr.ph and (((a > (add / 2)) and ((entry and (c < 1234567)) or (for.inc and (((+ * a) + c) < 1234567)))) or ((NOT (a > (add / 2))) and ((entry and (c < 1234567)) or (for.inc and ((call4 + c) < 1234567)))))) or (for.inc and (((a > (add / 2)) and ((entry and (c < 1234567)) or (for.inc and (((+ * a) + c.addr.1) < 1234567)))) or ((NOT (a > (add / 2))) and ((entry and (c < 1234567)) or (for.inc and ((call4 + c.addr.1) < 1234567)))))))) or (for.inc and ((for.body.lr.ph and (((inc > (add / 2)) and ((entry and (c < 1234567)) or (for.inc and (((+ * a) + c) < 1234567)))) or ((NOT (inc > (add / 2))) and ((entry and (c < 1234567)) or (for.inc and ((call4 + c) < 1234567)))))) or (for.inc and (((inc > (add / 2)) and ((entry and (c < 1234567)) or (for.inc and (((+ * a) + c.addr.1) < 1234567)))) or ((NOT (inc > (add / 2))) and ((entry and (c < 1234567)) or (for.inc and ((call4 + c.addr.1) < 1234567))))))))) |
|   %add = add nsw i32 %b, %a | ((for.body.lr.ph and ((for.body.lr.ph and (((a > ((b + a) / 2)) and ((entry and (c < 1234567)) or (for.inc and (((+ * a) + c) < 1234567)))) or ((NOT (a > ((b + a) / 2))) and ((entry and (c < 1234567)) or (for.inc and ((call4 + c) < 1234567)))))) or (for.inc and (((a > ((b + a) / 2)) and ((entry and (c < 1234567)) or (for.inc and (((+ * a) + c.addr.1) < 1234567)))) or ((NOT (a > ((b + a) / 2))) and ((entry and (c < 1234567)) or (for.inc and ((call4 + c.addr.1) < 1234567)))))))) or (for.inc and ((for.body.lr.ph and (((inc > ((b + a) / 2)) and ((entry and (c < 1234567)) or (for.inc and (((+ * a) + c) < 1234567)))) or ((NOT (inc > ((b + a) / 2))) and ((entry and (c < 1234567)) or (for.inc and ((call4 + c) < 1234567)))))) or (for.inc and (((inc > ((b + a) / 2)) and ((entry and (c < 1234567)) or (for.inc and (((+ * a) + c.addr.1) < 1234567)))) or ((NOT (inc > ((b + a) / 2))) and ((entry and (c < 1234567)) or (for.inc and ((call4 + c.addr.1) < 1234567))))))))) |
# In block entry, function f
| LLVM Instruction | Precondition |
|-----|-----|
|   br i1 %cmp18, label %for.body.lr.ph, label %for.end | ((cmp18 and ((cmp18 and (((a > ((b + a) / 2)) and ((entry and (c < 1234567)) or (for.inc and (((+ * a) + c) < 1234567)))) or ((NOT (a > ((b + a) / 2))) and ((entry and (c < 1234567)) or (for.inc and ((call4 + c) < 1234567)))))) or (for.inc and (((a > ((b + a) / 2)) and ((entry and (c < 1234567)) or (for.inc and (((+ * a) + c.addr.1) < 1234567)))) or ((NOT (a > ((b + a) / 2))) and ((entry and (c < 1234567)) or (for.inc and ((call4 + c.addr.1) < 1234567)))))))) or (for.inc and ((cmp18 and (((inc > ((b + a) / 2)) and ((entry and (c < 1234567)) or (for.inc and (((+ * a) + c) < 1234567)))) or ((NOT (inc > ((b + a) / 2))) and ((entry and (c < 1234567)) or (for.inc and ((call4 + c) < 1234567)))))) or (for.inc and (((inc > ((b + a) / 2)) and ((entry and (c < 1234567)) or (for.inc and (((+ * a) + c.addr.1) < 1234567)))) or ((NOT (inc > ((b + a) / 2))) and ((entry and (c < 1234567)) or (for.inc and ((call4 + c.addr.1) < 1234567))))))))) |
|   %cmp18 = icmp slt i32 %a, %b | (((a < b) and (((a < b) and (((a > ((b + a) / 2)) and ((entry and (c < 1234567)) or (for.inc and (((+ * a) + c) < 1234567)))) or ((NOT (a > ((b + a) / 2))) and ((entry and (c < 1234567)) or (for.inc and ((call4 + c) < 1234567)))))) or (for.inc and (((a > ((b + a) / 2)) and ((entry and (c < 1234567)) or (for.inc and (((+ * a) + c.addr.1) < 1234567)))) or ((NOT (a > ((b + a) / 2))) and ((entry and (c < 1234567)) or (for.inc and ((call4 + c.addr.1) < 1234567)))))))) or (for.inc and (((a < b) and (((inc > ((b + a) / 2)) and ((entry and (c < 1234567)) or (for.inc and (((+ * a) + c) < 1234567)))) or ((NOT (inc > ((b + a) / 2))) and ((entry and (c < 1234567)) or (for.inc and ((call4 + c) < 1234567)))))) or (for.inc and (((inc > ((b + a) / 2)) and ((entry and (c < 1234567)) or (for.inc and (((+ * a) + c.addr.1) < 1234567)))) or ((NOT (inc > ((b + a) / 2))) and ((entry and (c < 1234567)) or (for.inc and ((call4 + c.addr.1) < 1234567))))))))) |
