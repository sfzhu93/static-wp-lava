# In block if.end.13, function hello
| LLVM Instruction | Precondition |
|-----|-----|
|   %add15 = add i32 %add14, %a.addr.0 | ((add14 + a.addr.0) < 1234567) |
|   %add14 = add i32 %c, %b | (((c + b) + a.addr.0) < 1234567) |
|   %a.addr.0 = phi i32 [ %add8, %if.else ], [ %a, %if.end.3 ] | ((if.else and (((c + b) + add8) < 1234567)) or (if.end.3 and (((c + b) + a) < 1234567))) |
# In block if.else, function hello
| LLVM Instruction | Precondition |
|-----|-----|
|   br i1 %cmp9, label %cleanup, label %if.end.13 | ((if.else and (((c + b) + add8) < 1234567)) or (if.end.3 and (((c + b) + a) < 1234567))) |
|   %cmp9 = icmp sgt i32 %div, 3 | ((if.else and (((c + b) + add8) < 1234567)) or (if.end.3 and (((c + b) + a) < 1234567))) |
|   %div = sdiv i32 %c, %b | ((if.else and (((c + b) + add8) < 1234567)) or (if.end.3 and (((c + b) + a) < 1234567))) |
|   %add8 = add nsw i32 %a, 10 | ((if.else and (((c + b) + (a + 10)) < 1234567)) or (if.end.3 and (((c + b) + a) < 1234567))) |
# In block if.then.5, function hello
| LLVM Instruction | Precondition |
|-----|-----|
|   br i1 %cmp6, label %cleanup, label %if.else | (((NOT cmp6) and (((c + b) + (a + 10)) < 1234567)) or (if.end.3 and (((c + b) + a) < 1234567))) |
|   %cmp6 = icmp sgt i32 %mul, 10 | (((NOT (mul > 10)) and (((c + b) + (a + 10)) < 1234567)) or (if.end.3 and (((c + b) + a) < 1234567))) |
|   %mul = mul nsw i32 %c, %b | (((NOT ((c * b) > 10)) and (((c + b) + (a + 10)) < 1234567)) or (if.end.3 and (((c + b) + a) < 1234567))) |
# In block if.end.3, function hello
| LLVM Instruction | Precondition |
|-----|-----|
|   br i1 %cmp4, label %if.then.5, label %if.end.13 | (((NOT ((c * b) > 10)) and (((c + b) + (a + 10)) < 1234567)) or (if.end.3 and (((c + b) + a) < 1234567))) |
|   %cmp4 = icmp slt i32 %c, 100 | (((NOT ((c * b) > 10)) and (((c + b) + (a + 10)) < 1234567)) or (if.end.3 and (((c + b) + a) < 1234567))) |
# In block if.end, function hello
| LLVM Instruction | Precondition |
|-----|-----|
|   br i1 %cmp1, label %cleanup, label %if.end.3 | (((NOT ((c * b) > 10)) and (((c + b) + (a + 10)) < 1234567)) or ((NOT cmp1) and (((c + b) + a) < 1234567))) |
|   %cmp1 = icmp sgt i32 %add, 0 | (((NOT ((c * b) > 10)) and (((c + b) + (a + 10)) < 1234567)) or ((NOT (add > 0)) and (((c + b) + a) < 1234567))) |
|   %add = add nsw i32 %b, %a | (((NOT ((c * b) > 10)) and (((c + b) + (a + 10)) < 1234567)) or ((NOT ((b + a) > 0)) and (((c + b) + a) < 1234567))) |
# In block entry, function hello
| LLVM Instruction | Precondition |
|-----|-----|
|   br i1 %cmp, label %cleanup, label %if.end | (((NOT ((c * b) > 10)) and (((c + b) + (a + 10)) < 1234567)) or ((NOT ((b + a) > 0)) and (((c + b) + a) < 1234567))) |
|   %cmp = icmp slt i32 %a, 0 | (((NOT ((c * b) > 10)) and (((c + b) + (a + 10)) < 1234567)) or ((NOT ((b + a) > 0)) and (((c + b) + a) < 1234567))) |
# In block if.end.10, function hello2
| LLVM Instruction | Precondition |
|-----|-----|
|   %add12 = add i32 %add11, %a.addr.0 | ((add11 + a.addr.0) < 1234567) |
|   %add11 = add i32 %c, %b | (((c + b) + a.addr.0) < 1234567) |
|   %a.addr.0 = phi i32 [ %add9, %if.end.8 ], [ %a, %if.end.3 ] | ((if.end.8 and (((c + b) + add9) < 1234567)) or (if.end.3 and (((c + b) + a) < 1234567))) |
# In block if.end.8, function hello2
| LLVM Instruction | Precondition |
|-----|-----|
|   br label %if.end.10 | ((if.end.8 and (((c + b) + add9) < 1234567)) or (if.end.3 and (((c + b) + a) < 1234567))) |
|   %add9 = add nsw i32 %a, 10 | ((if.end.8 and (((c + b) + (a + 10)) < 1234567)) or (if.end.3 and (((c + b) + a) < 1234567))) |
# In block if.then.5, function hello2
| LLVM Instruction | Precondition |
|-----|-----|
|   br i1 %cmp6, label %cleanup, label %if.end.8 | (((NOT cmp6) and (((c + b) + (a + 10)) < 1234567)) or (if.end.3 and (((c + b) + a) < 1234567))) |
|   %cmp6 = icmp sgt i32 %mul, 10 | (((NOT (mul > 10)) and (((c + b) + (a + 10)) < 1234567)) or (if.end.3 and (((c + b) + a) < 1234567))) |
|   %mul = mul nsw i32 %c, %b | (((NOT ((c * b) > 10)) and (((c + b) + (a + 10)) < 1234567)) or (if.end.3 and (((c + b) + a) < 1234567))) |
# In block if.end.3, function hello2
| LLVM Instruction | Precondition |
|-----|-----|
|   br i1 %cmp4, label %if.then.5, label %if.end.10 | (((NOT ((c * b) > 10)) and (((c + b) + (a + 10)) < 1234567)) or (if.end.3 and (((c + b) + a) < 1234567))) |
|   %cmp4 = icmp slt i32 %c, 100 | (((NOT ((c * b) > 10)) and (((c + b) + (a + 10)) < 1234567)) or (if.end.3 and (((c + b) + a) < 1234567))) |
# In block if.end, function hello2
| LLVM Instruction | Precondition |
|-----|-----|
|   br i1 %cmp1, label %cleanup, label %if.end.3 | (((NOT ((c * b) > 10)) and (((c + b) + (a + 10)) < 1234567)) or ((NOT cmp1) and (((c + b) + a) < 1234567))) |
|   %cmp1 = icmp sgt i32 %add, 0 | (((NOT ((c * b) > 10)) and (((c + b) + (a + 10)) < 1234567)) or ((NOT (add > 0)) and (((c + b) + a) < 1234567))) |
|   %add = add nsw i32 %b, %a | (((NOT ((c * b) > 10)) and (((c + b) + (a + 10)) < 1234567)) or ((NOT ((b + a) > 0)) and (((c + b) + a) < 1234567))) |
# In block entry, function hello2
| LLVM Instruction | Precondition |
|-----|-----|
|   br i1 %cmp, label %cleanup, label %if.end | (((NOT ((c * b) > 10)) and (((c + b) + (a + 10)) < 1234567)) or ((NOT ((b + a) > 0)) and (((c + b) + a) < 1234567))) |
|   %cmp = icmp slt i32 %a, 0 | (((NOT ((c * b) > 10)) and (((c + b) + (a + 10)) < 1234567)) or ((NOT ((b + a) > 0)) and (((c + b) + a) < 1234567))) |
# In block if.end.10, function hello3
| LLVM Instruction | Precondition |
|-----|-----|
|   %add12 = add i32 %add11, %a.addr.0 | ((add11 + a.addr.0) < 1234567) |
|   %add11 = add i32 %b.addr.1, %c | (((b.addr.1 + c) + a.addr.0) < 1234567) |
|   %a.addr.0 = phi i32 [ %add9, %if.then.5 ], [ %a, %if.end.3 ] | ((if.then.5 and (((b.addr.1 + c) + add9) < 1234567)) or (if.end.3 and (((b.addr.1 + c) + a) < 1234567))) |
|   %b.addr.1 = phi i32 [ %.b, %if.then.5 ], [ %b, %if.end.3 ] | ((if.then.5 and ((if.then.5 and (((.b + c) + add9) < 1234567)) or (if.end.3 and (((.b + c) + a) < 1234567)))) or (if.end.3 and ((if.then.5 and (((b + c) + add9) < 1234567)) or (if.end.3 and (((b + c) + a) < 1234567))))) |
# In block if.then.5, function hello3
| LLVM Instruction | Precondition |
|-----|-----|
|   br label %if.end.10 | ((if.then.5 and ((if.then.5 and (((.b + c) + add9) < 1234567)) or (if.end.3 and (((.b + c) + a) < 1234567)))) or (if.end.3 and ((if.then.5 and (((b + c) + add9) < 1234567)) or (if.end.3 and (((b + c) + a) < 1234567))))) |
|   %add9 = add nsw i32 %a, 10 | ((if.then.5 and ((if.then.5 and (((.b + c) + (a + 10)) < 1234567)) or (if.end.3 and (((.b + c) + a) < 1234567)))) or (if.end.3 and ((if.then.5 and (((b + c) + (a + 10)) < 1234567)) or (if.end.3 and (((b + c) + a) < 1234567))))) |
|   %.b = select i1 %cmp6, i32 1, i32 %b | ((cmp6 and ((if.then.5 and ((if.then.5 and (((1 + c) + (a + 10)) < 1234567)) or (if.end.3 and (((1 + c) + a) < 1234567)))) or (if.end.3 and ((if.then.5 and (((b + c) + (a + 10)) < 1234567)) or (if.end.3 and (((b + c) + a) < 1234567)))))) or ((not cmp6) and ((if.then.5 and ((if.then.5 and (((b + c) + (a + 10)) < 1234567)) or (if.end.3 and (((b + c) + a) < 1234567)))) or (if.end.3 and ((if.then.5 and (((b + c) + (a + 10)) < 1234567)) or (if.end.3 and (((b + c) + a) < 1234567))))))) |
|   %cmp6 = icmp sgt i32 %mul, 10 | (((mul > 10) and ((if.then.5 and ((if.then.5 and (((1 + c) + (a + 10)) < 1234567)) or (if.end.3 and (((1 + c) + a) < 1234567)))) or (if.end.3 and ((if.then.5 and (((b + c) + (a + 10)) < 1234567)) or (if.end.3 and (((b + c) + a) < 1234567)))))) or ((not (mul > 10)) and ((if.then.5 and ((if.then.5 and (((b + c) + (a + 10)) < 1234567)) or (if.end.3 and (((b + c) + a) < 1234567)))) or (if.end.3 and ((if.then.5 and (((b + c) + (a + 10)) < 1234567)) or (if.end.3 and (((b + c) + a) < 1234567))))))) |
|   %mul = mul nsw i32 %c, %b | ((((c * b) > 10) and ((if.then.5 and ((if.then.5 and (((1 + c) + (a + 10)) < 1234567)) or (if.end.3 and (((1 + c) + a) < 1234567)))) or (if.end.3 and ((if.then.5 and (((b + c) + (a + 10)) < 1234567)) or (if.end.3 and (((b + c) + a) < 1234567)))))) or ((not ((c * b) > 10)) and ((if.then.5 and ((if.then.5 and (((b + c) + (a + 10)) < 1234567)) or (if.end.3 and (((b + c) + a) < 1234567)))) or (if.end.3 and ((if.then.5 and (((b + c) + (a + 10)) < 1234567)) or (if.end.3 and (((b + c) + a) < 1234567))))))) |
# In block if.end.3, function hello3
| LLVM Instruction | Precondition |
|-----|-----|
|   br i1 %cmp4, label %if.then.5, label %if.end.10 | ((((c * b) > 10) and ((cmp4 and ((cmp4 and (((1 + c) + (a + 10)) < 1234567)) or (if.end.3 and (((1 + c) + a) < 1234567)))) or (if.end.3 and ((cmp4 and (((b + c) + (a + 10)) < 1234567)) or (if.end.3 and (((b + c) + a) < 1234567)))))) or ((not ((c * b) > 10)) and ((cmp4 and ((cmp4 and (((b + c) + (a + 10)) < 1234567)) or (if.end.3 and (((b + c) + a) < 1234567)))) or (if.end.3 and ((cmp4 and (((b + c) + (a + 10)) < 1234567)) or (if.end.3 and (((b + c) + a) < 1234567))))))) |
|   %cmp4 = icmp slt i32 %c, 100 | ((((c * b) > 10) and (((c < 100) and (((c < 100) and (((1 + c) + (a + 10)) < 1234567)) or (if.end.3 and (((1 + c) + a) < 1234567)))) or (if.end.3 and (((c < 100) and (((b + c) + (a + 10)) < 1234567)) or (if.end.3 and (((b + c) + a) < 1234567)))))) or ((not ((c * b) > 10)) and (((c < 100) and (((c < 100) and (((b + c) + (a + 10)) < 1234567)) or (if.end.3 and (((b + c) + a) < 1234567)))) or (if.end.3 and (((c < 100) and (((b + c) + (a + 10)) < 1234567)) or (if.end.3 and (((b + c) + a) < 1234567))))))) |
# In block if.end, function hello3
| LLVM Instruction | Precondition |
|-----|-----|
|   br i1 %cmp1, label %cleanup, label %if.end.3 | ((((c * b) > 10) and (((c < 100) and (((c < 100) and (((1 + c) + (a + 10)) < 1234567)) or ((NOT cmp1) and (((1 + c) + a) < 1234567)))) or ((NOT cmp1) and (((c < 100) and (((b + c) + (a + 10)) < 1234567)) or ((NOT cmp1) and (((b + c) + a) < 1234567)))))) or ((not ((c * b) > 10)) and (((c < 100) and (((c < 100) and (((b + c) + (a + 10)) < 1234567)) or ((NOT cmp1) and (((b + c) + a) < 1234567)))) or ((NOT cmp1) and (((c < 100) and (((b + c) + (a + 10)) < 1234567)) or ((NOT cmp1) and (((b + c) + a) < 1234567))))))) |
|   %cmp1 = icmp sgt i32 %add, 0 | ((((c * b) > 10) and (((c < 100) and (((c < 100) and (((1 + c) + (a + 10)) < 1234567)) or ((NOT (add > 0)) and (((1 + c) + a) < 1234567)))) or ((NOT (add > 0)) and (((c < 100) and (((b + c) + (a + 10)) < 1234567)) or ((NOT (add > 0)) and (((b + c) + a) < 1234567)))))) or ((not ((c * b) > 10)) and (((c < 100) and (((c < 100) and (((b + c) + (a + 10)) < 1234567)) or ((NOT (add > 0)) and (((b + c) + a) < 1234567)))) or ((NOT (add > 0)) and (((c < 100) and (((b + c) + (a + 10)) < 1234567)) or ((NOT (add > 0)) and (((b + c) + a) < 1234567))))))) |
|   %add = add nsw i32 %b, %a | ((((c * b) > 10) and (((c < 100) and (((c < 100) and (((1 + c) + (a + 10)) < 1234567)) or ((NOT ((b + a) > 0)) and (((1 + c) + a) < 1234567)))) or ((NOT ((b + a) > 0)) and (((c < 100) and (((b + c) + (a + 10)) < 1234567)) or ((NOT ((b + a) > 0)) and (((b + c) + a) < 1234567)))))) or ((not ((c * b) > 10)) and (((c < 100) and (((c < 100) and (((b + c) + (a + 10)) < 1234567)) or ((NOT ((b + a) > 0)) and (((b + c) + a) < 1234567)))) or ((NOT ((b + a) > 0)) and (((c < 100) and (((b + c) + (a + 10)) < 1234567)) or ((NOT ((b + a) > 0)) and (((b + c) + a) < 1234567))))))) |
# In block entry, function hello3
| LLVM Instruction | Precondition |
|-----|-----|
|   br i1 %cmp, label %cleanup, label %if.end | ((((c * b) > 10) and (((c < 100) and (((c < 100) and (((1 + c) + (a + 10)) < 1234567)) or ((NOT ((b + a) > 0)) and (((1 + c) + a) < 1234567)))) or ((NOT ((b + a) > 0)) and (((c < 100) and (((b + c) + (a + 10)) < 1234567)) or ((NOT ((b + a) > 0)) and (((b + c) + a) < 1234567)))))) or ((not ((c * b) > 10)) and (((c < 100) and (((c < 100) and (((b + c) + (a + 10)) < 1234567)) or ((NOT ((b + a) > 0)) and (((b + c) + a) < 1234567)))) or ((NOT ((b + a) > 0)) and (((c < 100) and (((b + c) + (a + 10)) < 1234567)) or ((NOT ((b + a) > 0)) and (((b + c) + a) < 1234567))))))) |
|   %cmp = icmp slt i32 %a, 0 | ((((c * b) > 10) and (((c < 100) and (((c < 100) and (((1 + c) + (a + 10)) < 1234567)) or ((NOT ((b + a) > 0)) and (((1 + c) + a) < 1234567)))) or ((NOT ((b + a) > 0)) and (((c < 100) and (((b + c) + (a + 10)) < 1234567)) or ((NOT ((b + a) > 0)) and (((b + c) + a) < 1234567)))))) or ((not ((c * b) > 10)) and (((c < 100) and (((c < 100) and (((b + c) + (a + 10)) < 1234567)) or ((NOT ((b + a) > 0)) and (((b + c) + a) < 1234567)))) or ((NOT ((b + a) > 0)) and (((c < 100) and (((b + c) + (a + 10)) < 1234567)) or ((NOT ((b + a) > 0)) and (((b + c) + a) < 1234567))))))) |
