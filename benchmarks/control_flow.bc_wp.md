# In block entry, function hello
| LLVM Instruction | Precondition |
|-----|-----|
|   %mul6 = mul nsw i32 %mul5, %c.addr.0 | ((mul5 * c.addr.0) < 1234567) |
|   %mul5 = mul nsw i32 %b.addr.0, %add1 | (((b.addr.0 * add1) * c.addr.0) < 1234567) |
|   %c.addr.0 = select i1 %cmp, i32 %mul2, i32 %mul3 | ((cmp and (((b.addr.0 * add1) * mul2) < 1234567)) or ((not cmp) and (((b.addr.0 * add1) * mul3) < 1234567))) |
|   %b.addr.0 = select i1 %cmp, i32 %mul, i32 %mul4 | ((cmp and ((cmp and (((mul * add1) * mul2) < 1234567)) or ((not cmp) and (((mul * add1) * mul3) < 1234567)))) or ((not cmp) and ((cmp and (((mul4 * add1) * mul2) < 1234567)) or ((not cmp) and (((mul4 * add1) * mul3) < 1234567))))) |
|   %mul4 = mul nsw i32 %mul3, %add1 | ((cmp and ((cmp and (((mul * add1) * mul2) < 1234567)) or ((not cmp) and (((mul * add1) * mul3) < 1234567)))) or ((not cmp) and ((cmp and ((((mul3 * add1) * add1) * mul2) < 1234567)) or ((not cmp) and ((((mul3 * add1) * add1) * mul3) < 1234567))))) |
|   %mul3 = mul nsw i32 %c, %b | ((cmp and ((cmp and (((mul * add1) * mul2) < 1234567)) or ((not cmp) and (((mul * add1) * (c * b)) < 1234567)))) or ((not cmp) and ((cmp and (((((c * b) * add1) * add1) * mul2) < 1234567)) or ((not cmp) and (((((c * b) * add1) * add1) * (c * b)) < 1234567))))) |
|   %mul2 = mul nsw i32 %mul, %c | ((cmp and ((cmp and (((mul * add1) * (mul * c)) < 1234567)) or ((not cmp) and (((mul * add1) * (c * b)) < 1234567)))) or ((not cmp) and ((cmp and (((((c * b) * add1) * add1) * (mul * c)) < 1234567)) or ((not cmp) and (((((c * b) * add1) * add1) * (c * b)) < 1234567))))) |
|   %mul = mul nsw i32 %add1, %b | ((cmp and ((cmp and ((((add1 * b) * add1) * ((add1 * b) * c)) < 1234567)) or ((not cmp) and ((((add1 * b) * add1) * (c * b)) < 1234567)))) or ((not cmp) and ((cmp and (((((c * b) * add1) * add1) * ((add1 * b) * c)) < 1234567)) or ((not cmp) and (((((c * b) * add1) * add1) * (c * b)) < 1234567))))) |
|   %cmp = icmp sgt i32 %add1, 2 | (((add1 > 2) and (((add1 > 2) and ((((add1 * b) * add1) * ((add1 * b) * c)) < 1234567)) or ((not (add1 > 2)) and ((((add1 * b) * add1) * (c * b)) < 1234567)))) or ((not (add1 > 2)) and (((add1 > 2) and (((((c * b) * add1) * add1) * ((add1 * b) * c)) < 1234567)) or ((not (add1 > 2)) and (((((c * b) * add1) * add1) * (c * b)) < 1234567))))) |
|   %add1 = add nsw i32 %add, %b | ((((add + b) > 2) and ((((add + b) > 2) and (((((add + b) * b) * (add + b)) * (((add + b) * b) * c)) < 1234567)) or ((not ((add + b) > 2)) and (((((add + b) * b) * (add + b)) * (c * b)) < 1234567)))) or ((not ((add + b) > 2)) and ((((add + b) > 2) and (((((c * b) * (add + b)) * (add + b)) * (((add + b) * b) * c)) < 1234567)) or ((not ((add + b) > 2)) and (((((c * b) * (add + b)) * (add + b)) * (c * b)) < 1234567))))) |
|   %add = add nsw i32 %a, 5 | (((((a + 5) + b) > 2) and (((((a + 5) + b) > 2) and ((((((a + 5) + b) * b) * ((a + 5) + b)) * ((((a + 5) + b) * b) * c)) < 1234567)) or ((not (((a + 5) + b) > 2)) and ((((((a + 5) + b) * b) * ((a + 5) + b)) * (c * b)) < 1234567)))) or ((not (((a + 5) + b) > 2)) and (((((a + 5) + b) > 2) and (((((c * b) * ((a + 5) + b)) * ((a + 5) + b)) * ((((a + 5) + b) * b) * c)) < 1234567)) or ((not (((a + 5) + b) > 2)) and (((((c * b) * ((a + 5) + b)) * ((a + 5) + b)) * (c * b)) < 1234567))))) |