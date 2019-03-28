# In block if.end, function hello
| LLVM Instruction | Precondition |
|-----|-----|
|   %mul6 = mul i32 %mul5, %b.addr.0 | ((mul5 * b.addr.0) < 1234567) |
|   %mul5 = mul i32 %add, %c | (((add * c) * b.addr.0) < 1234567) |
|   %b.addr.0 = phi i32 [ %mul3, %if.then ], [ %mul4, %if.else ] | ((if.then and (((add * c) * mul3) < 1234567)) or (if.else and (((add * c) * mul4) < 1234567))) |
# In block if.then, function hello
| LLVM Instruction | Precondition |
|-----|-----|
|   br label %if.end | ((if.then and (((add * c) * mul3) < 1234567)) or (if.else and (((add * c) * mul4) < 1234567))) |
|   %mul3 = mul i32 %mul, %sub | ((if.then and (((add * c) * (mul * sub)) < 1234567)) or (if.else and (((add * c) * mul4) < 1234567))) |
|   %mul = mul i32 %sub2, %sub1 | ((if.then and (((add * c) * ((sub2 * sub1) * sub)) < 1234567)) or (if.else and (((add * c) * mul4) < 1234567))) |
|   %sub2 = add nsw i32 %c, -1 | ((if.then and (((add * c) * (((c + -1) * sub1) * sub)) < 1234567)) or (if.else and (((add * c) * mul4) < 1234567))) |
|   %sub1 = add nsw i32 %b, -1 | ((if.then and (((add * c) * (((c + -1) * (b + -1)) * sub)) < 1234567)) or (if.else and (((add * c) * mul4) < 1234567))) |
|   %sub = add nsw i32 %add, -1 | ((if.then and (((add * c) * (((c + -1) * (b + -1)) * (add + -1))) < 1234567)) or (if.else and (((add * c) * mul4) < 1234567))) |
# In block if.else, function hello
| LLVM Instruction | Precondition |
|-----|-----|
|   br label %if.end | ((if.then and (((add * c) * (((c + -1) * (b + -1)) * (add + -1))) < 1234567)) or (if.else and (((add * c) * mul4) < 1234567))) |
|   %mul4 = mul nsw i32 %call, %b | ((if.then and (((add * c) * (((c + -1) * (b + -1)) * (add + -1))) < 1234567)) or (if.else and (((add * c) * (call * b)) < 1234567))) |
# In block entry, function f
| LLVM Instruction | Precondition |
|-----|-----|
|   ret i32 %add | ((if.then and (((add * c) * (((c + -1) * (b + -1)) * (add + -1))) < 1234567)) or (if.else and (((add * c) * (add * b)) < 1234567))) |
|   %add = add nsw i32 %a, 1 | ((if.then and ((((a + 1) * c) * (((c + -1) * (b + -1)) * ((a + 1) + -1))) < 1234567)) or (if.else and ((((a + 1) * c) * ((a + 1) * b)) < 1234567))) |
|   %call = tail call i32 @f(i32 %add) | ((if.then and ((((add + 1) * c) * (((c + -1) * (b + -1)) * ((add + 1) + -1))) < 1234567)) or (if.else and ((((add + 1) * c) * ((add + 1) * b)) < 1234567))) |
# In block entry, function f
| LLVM Instruction | Precondition |
|-----|-----|
|   br i1 %cmp, label %if.then, label %if.else | ((cmp and ((((add + 1) * c) * (((c + -1) * (b + -1)) * ((add + 1) + -1))) < 1234567)) or ((NOT cmp) and ((((add + 1) * c) * ((add + 1) * b)) < 1234567))) |
|   %cmp = icmp sgt i32 %add, 2 | (((add > 2) and ((((add + 1) * c) * (((c + -1) * (b + -1)) * ((add + 1) + -1))) < 1234567)) or ((NOT (add > 2)) and ((((add + 1) * c) * ((add + 1) * b)) < 1234567))) |
|   %add = add nsw i32 %c, %b | ((((c + b) > 2) and (((((c + b) + 1) * c) * (((c + -1) * (b + -1)) * (((c + b) + 1) + -1))) < 1234567)) or ((NOT ((c + b) > 2)) and (((((c + b) + 1) * c) * (((c + b) + 1) * b)) < 1234567))) |
