# In block if.end, function hello
| LLVM Instruction | Precondition |
|-----|-----|
|   %mul6 = mul i32 %mul5, %b.addr.0 | ((mul5 * b.addr.0) < 1234567) |
|   %mul5 = mul i32 %add, %c | (((add * c) * b.addr.0) < 1234567) |
|   %b.addr.0 = phi i32 [ %mul3, %if.then ], [ %mul4, %if.else ] | ((if.then and (((add * c) * mul3) < )) or (if.else and (((add * c) * mul4) < ))) |
# In block if.then, function hello
| LLVM Instruction | Precondition |
|-----|-----|
|   br label %if.end | ((if.then and (((add * c) * mul3) < )) or (if.else and (((add * c) * mul4) < ))) |
|   %mul3 = mul i32 %mul, %sub | ((if.then and (((add * c) * (mul * sub)) < )) or (if.else and (((add * c) * mul4) < ))) |
|   %mul = mul i32 %sub2, %sub1 | ((if.then and (((add * c) * ((sub2 * sub1) * sub)) < )) or (if.else and (((add * c) * mul4) < ))) |
|   %sub2 = add nsw i32 %c, -1 | ((if.then and (((add * c) * (((c + -1) * sub1) * sub)) < )) or (if.else and (((add * c) * mul4) < ))) |
|   %sub1 = add nsw i32 %b, -1 | ((if.then and (((add * c) * (((c + -1) * (b + -1)) * sub)) < )) or (if.else and (((add * c) * mul4) < ))) |
|   %sub = add nsw i32 %add, -1 | ((if.then and (((add * c) * (((c + -1) * (b + -1)) * (add + -1))) < )) or (if.else and (((add * c) * mul4) < ))) |
# In block if.else, function hello
| LLVM Instruction | Precondition |
|-----|-----|
|   br label %if.end | ((if.then and (((add * c) * (((c + -1) * (b + -1)) * (add + -1))) < )) or (if.else and (((add * c) * mul4) < ))) |
|   %mul4 = mul nsw i32 %add.i, %b | ((if.then and (((add * c) * (((c + -1) * (b + -1)) * (add + -1))) < )) or (if.else and (((add * c) * (add.i * b)) < ))) |
|   %add.i = add nsw i32 %add, 1 | ((if.then and (((add * c) * (((c + -1) * (b + -1)) * (add + -1))) < )) or (if.else and (((add * c) * ((add + 1) * b)) < ))) |
# In block entry, function hello
| LLVM Instruction | Precondition |
|-----|-----|
|   br i1 %cmp, label %if.then, label %if.else | ((cmp and (((add * c) * (((c + -1) * (b + -1)) * (add + -1))) < )) or ((not cmp) and (((add * c) * ((add + 1) * b)) < ))) |
|   %cmp = icmp sgt i32 %add, 2 | (((add > 2) and (((add * c) * (((c + -1) * (b + -1)) * (add + -1))) < )) or ((not (add > 2)) and (((add * c) * ((add + 1) * b)) < ))) |
|   %add = add nsw i32 %c, %b | ((((c + b) > 2) and ((((c + b) * c) * (((c + -1) * (b + -1)) * ((c + b) + -1))) < )) or ((not ((c + b) > 2)) and ((((c + b) * c) * (((c + b) + 1) * b)) < ))) |
