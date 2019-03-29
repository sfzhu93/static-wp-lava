# In block if.end, function hello
| LLVM Instruction | Precondition |
|-----|-----|
|   %mul6 = mul nsw i32 %mul5, %c.addr.0 | ((mul5 * c.addr.0) < 1234567) |
|   %mul5 = mul nsw i32 %b.addr.0, %add1 | (((b.addr.0 * add1) * c.addr.0) < 1234567) |
|   %c.addr.0 = phi i32 [ %mul2, %if.then ], [ %mul3, %if.else ] | ((if.then and (((b.addr.0 * add1) * mul2) < 1234567)) or (if.else and (((b.addr.0 * add1) * mul3) < 1234567))) |
|   %b.addr.0 = phi i32 [ %mul, %if.then ], [ %mul4, %if.else ] | ((if.then and ((if.then and (((mul * add1) * mul2) < 1234567)) or (if.else and (((mul * add1) * mul3) < 1234567)))) or (if.else and ((if.then and (((mul4 * add1) * mul2) < 1234567)) or (if.else and (((mul4 * add1) * mul3) < 1234567))))) |
# In block if.then, function hello
| LLVM Instruction | Precondition |
|-----|-----|
|   br label %if.end | ((if.then and ((if.then and (((mul * add1) * mul2) < 1234567)) or (if.else and (((mul * add1) * mul3) < 1234567)))) or (if.else and ((if.then and (((mul4 * add1) * mul2) < 1234567)) or (if.else and (((mul4 * add1) * mul3) < 1234567))))) |
|   %mul2 = mul nsw i32 %mul, %c | ((if.then and ((if.then and (((mul * add1) * (mul * c)) < 1234567)) or (if.else and (((mul * add1) * mul3) < 1234567)))) or (if.else and ((if.then and (((mul4 * add1) * (mul * c)) < 1234567)) or (if.else and (((mul4 * add1) * mul3) < 1234567))))) |
|   %mul = mul nsw i32 %add1, %b | ((if.then and ((if.then and ((((add1 * b) * add1) * ((add1 * b) * c)) < 1234567)) or (if.else and ((((add1 * b) * add1) * mul3) < 1234567)))) or (if.else and ((if.then and (((mul4 * add1) * ((add1 * b) * c)) < 1234567)) or (if.else and (((mul4 * add1) * mul3) < 1234567))))) |
# In block if.else, function hello
| LLVM Instruction | Precondition |
|-----|-----|
|   br label %if.end | ((if.then and ((if.then and ((((add1 * b) * add1) * ((add1 * b) * c)) < 1234567)) or (if.else and ((((add1 * b) * add1) * mul3) < 1234567)))) or (if.else and ((if.then and (((mul4 * add1) * ((add1 * b) * c)) < 1234567)) or (if.else and (((mul4 * add1) * mul3) < 1234567))))) |
|   %mul4 = mul nsw i32 %mul3, %add1 | ((if.then and ((if.then and ((((add1 * b) * add1) * ((add1 * b) * c)) < 1234567)) or (if.else and ((((add1 * b) * add1) * mul3) < 1234567)))) or (if.else and ((if.then and ((((mul3 * add1) * add1) * ((add1 * b) * c)) < 1234567)) or (if.else and ((((mul3 * add1) * add1) * mul3) < 1234567))))) |
|   %mul3 = mul nsw i32 %c, %b | ((if.then and ((if.then and ((((add1 * b) * add1) * ((add1 * b) * c)) < 1234567)) or (if.else and ((((add1 * b) * add1) * (c * b)) < 1234567)))) or (if.else and ((if.then and (((((c * b) * add1) * add1) * ((add1 * b) * c)) < 1234567)) or (if.else and (((((c * b) * add1) * add1) * (c * b)) < 1234567))))) |
# In block entry, function hello
| LLVM Instruction | Precondition |
|-----|-----|
|   br i1 %cmp, label %if.then, label %if.else | ((cmp and ((cmp and ((((add1 * b) * add1) * ((add1 * b) * c)) < 1234567)) or ((NOT cmp) and ((((add1 * b) * add1) * (c * b)) < 1234567)))) or ((NOT cmp) and ((cmp and (((((c * b) * add1) * add1) * ((add1 * b) * c)) < 1234567)) or ((NOT cmp) and (((((c * b) * add1) * add1) * (c * b)) < 1234567))))) |
|   %cmp = icmp sgt i32 %add1, 2 | (((add1 > 2) and (((add1 > 2) and ((((add1 * b) * add1) * ((add1 * b) * c)) < 1234567)) or ((NOT (add1 > 2)) and ((((add1 * b) * add1) * (c * b)) < 1234567)))) or ((NOT (add1 > 2)) and (((add1 > 2) and (((((c * b) * add1) * add1) * ((add1 * b) * c)) < 1234567)) or ((NOT (add1 > 2)) and (((((c * b) * add1) * add1) * (c * b)) < 1234567))))) |
|   %add1 = add nsw i32 %add, %b | ((((add + b) > 2) and ((((add + b) > 2) and (((((add + b) * b) * (add + b)) * (((add + b) * b) * c)) < 1234567)) or ((NOT ((add + b) > 2)) and (((((add + b) * b) * (add + b)) * (c * b)) < 1234567)))) or ((NOT ((add + b) > 2)) and ((((add + b) > 2) and (((((c * b) * (add + b)) * (add + b)) * (((add + b) * b) * c)) < 1234567)) or ((NOT ((add + b) > 2)) and (((((c * b) * (add + b)) * (add + b)) * (c * b)) < 1234567))))) |
|   %add = add nsw i32 %a, 5 | (((((a + 5) + b) > 2) and (((((a + 5) + b) > 2) and ((((((a + 5) + b) * b) * ((a + 5) + b)) * ((((a + 5) + b) * b) * c)) < 1234567)) or ((NOT (((a + 5) + b) > 2)) and ((((((a + 5) + b) * b) * ((a + 5) + b)) * (c * b)) < 1234567)))) or ((NOT (((a + 5) + b) > 2)) and (((((a + 5) + b) > 2) and (((((c * b) * ((a + 5) + b)) * ((a + 5) + b)) * ((((a + 5) + b) * b) * c)) < 1234567)) or ((NOT (((a + 5) + b) > 2)) and (((((c * b) * ((a + 5) + b)) * ((a + 5) + b)) * (c * b)) < 1234567))))) |
