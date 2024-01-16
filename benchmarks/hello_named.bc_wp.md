# In block entry, function hello
| LLVM Instruction | Precondition |
|-----|-----|
|   %mul4 = mul i32 %mul3, %mul | ((mul3 * mul) < 1234567) |
|   %mul3 = mul i32 %mul2, %mul | (((mul2 * mul) * mul) < 1234567) |
|   %mul2 = mul i32 %add1, %c | ((((add1 * c) * mul) * mul) < 1234567) |
|   %mul = mul nsw i32 %add1, %b | ((((add1 * c) * (add1 * b)) * (add1 * b)) < 1234567) |
|   %add1 = add nsw i32 %add, %b | (((((add + b) * c) * ((add + b) * b)) * ((add + b) * b)) < 1234567) |
|   %add = add nsw i32 %a, 5 | ((((((a + 5) + b) * c) * (((a + 5) + b) * b)) * (((a + 5) + b) * b)) < 1234567) |
