# In block entry, function hello
| LLVM Instruction | Precondition |
|-----|-----|
|   %add6 = add nsw i32 %mul5, %mul4 | ((mul5 + mul4) < 1234567) |
|   %mul5 = select i1 %cmp, i32 1234567, i32 0 | ((cmp and ((1234567 + mul4) < 1234567)) or ((not cmp) and ((0 + mul4) < 1234567))) |
|   %cmp = icmp eq i32 %call, 10 | (((call = 10) and ((1234567 + mul4) < 1234567)) or ((not (call = 10)) and ((0 + mul4) < 1234567))) |
# In block if.end, function lava_get
| LLVM Instruction | Precondition |
|-----|-----|
|   ret i32 %tmp1 | UPred(tmp1) |
|   %tmp1 = load i32* %arrayidx, align 4, !tbaa !1 | UPred(arrayidx) |
# In block if.then, function lava_get
| LLVM Instruction | Precondition |
|-----|-----|
|   br label %if.end | UPred(arrayidx) |
# In block entry, function lava_get
| LLVM Instruction | Precondition |
|-----|-----|
|   br i1 %or.cond, label %if.then, label %if.end | UPred(arrayidx) |
|   %or.cond = or i1 %cmp, %cmp12 | UPred(arrayidx) |
|   %cmp12 = icmp eq i32 %or9, %tmp | UPred(arrayidx) |
|   %cmp = icmp eq i32 %sub, %tmp | UPred(arrayidx) |
|   %tmp = load i32* %arrayidx, align 4, !tbaa !1 | UPred(arrayidx) |
|   %arrayidx = getelementptr inbounds [1000000 x i32]* @lava_val, i64 0, i64 %idxprom | UPred((lava_val [] idxprom)) |
|   %idxprom = zext i32 %bug_num to i64 | UPred((lava_val [] idxprom)) |
|   %sub = sub i32 1818326625, %bug_num | UPred((lava_val [] idxprom)) |
# In block entry, function hello
| LLVM Instruction | Precondition |
|-----|-----|
|   %call = tail call i32 @lava_get(i32 1) | ((((lava_val [] idxprom) = 10) and ((1234567 + mul4) < 1234567)) or ((not ((lava_val [] idxprom) = 10)) and ((0 + mul4) < 1234567))) |
|   %mul4 = mul i32 %mul3, %mul | ((((lava_val [] idxprom) = 10) and ((1234567 + (mul3 * mul)) < 1234567)) or ((not ((lava_val [] idxprom) = 10)) and ((0 + (mul3 * mul)) < 1234567))) |
|   %mul3 = mul i32 %mul2, %mul | ((((lava_val [] idxprom) = 10) and ((1234567 + ((mul2 * mul) * mul)) < 1234567)) or ((not ((lava_val [] idxprom) = 10)) and ((0 + ((mul2 * mul) * mul)) < 1234567))) |
|   %mul2 = mul i32 %add1, %c | ((((lava_val [] idxprom) = 10) and ((1234567 + (((add1 * c) * mul) * mul)) < 1234567)) or ((not ((lava_val [] idxprom) = 10)) and ((0 + (((add1 * c) * mul) * mul)) < 1234567))) |
|   %mul = mul nsw i32 %add1, %b | ((((lava_val [] idxprom) = 10) and ((1234567 + (((add1 * c) * (add1 * b)) * (add1 * b))) < 1234567)) or ((not ((lava_val [] idxprom) = 10)) and ((0 + (((add1 * c) * (add1 * b)) * (add1 * b))) < 1234567))) |
# In block entry, function hello
| LLVM Instruction | Precondition |
|-----|-----|
|   tail call void @lava_set(i32 1, i32 %c) | ((((lava_val [] idxprom) = 10) and ((1234567 + (((add1 * c) * (add1 * b)) * (add1 * b))) < 1234567)) or ((not ((lava_val [] idxprom) = 10)) and ((0 + (((add1 * c) * (add1 * b)) * (add1 * b))) < 1234567))) |
|   %add1 = add nsw i32 %add, %b | ((((lava_val [] idxprom) = 10) and ((1234567 + ((((add + b) * c) * ((add + b) * b)) * ((add + b) * b))) < 1234567)) or ((not ((lava_val [] idxprom) = 10)) and ((0 + ((((add + b) * c) * ((add + b) * b)) * ((add + b) * b))) < 1234567))) |
|   %add = add nsw i32 %a, 5 | ((((lava_val [] idxprom) = 10) and ((1234567 + (((((a + 5) + b) * c) * (((a + 5) + b) * b)) * (((a + 5) + b) * b))) < 1234567)) or ((not ((lava_val [] idxprom) = 10)) and ((0 + (((((a + 5) + b) * c) * (((a + 5) + b) * b)) * (((a + 5) + b) * b))) < 1234567))) |
