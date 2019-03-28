# In block entry, function hello
| LLVM Instruction | Precondition |
|-----|-----|
|   %add6 = add nsw i32 %mul5, %mul4 | ((mul5 + mul4) < 1234567) |
|   %mul5 = select i1 %cmp, i32 1234567, i32 0 | ((cmp and ((1234567 + mul4) < 1234567)) or ((not cmp) and ((0 + mul4) < 1234567))) |
|   %cmp = icmp eq i32 %call, 10 | (((call = 10) and ((1234567 + mul4) < 1234567)) or ((not (call = 10)) and ((0 + mul4) < 1234567))) |
# In block if.end, function lava_get
| LLVM Instruction | Precondition |
|-----|-----|
|   ret i32 %tmp1 | (((tmp1 = 10) and ((1234567 + mul4) < 1234567)) or ((not (tmp1 = 10)) and ((0 + mul4) < 1234567))) |
|   %tmp1 = load i32, i32* %arrayidx, align 4, !tbaa !1 | (((arrayidx = 10) and ((1234567 + mul4) < 1234567)) or ((not (arrayidx = 10)) and ((0 + mul4) < 1234567))) |
# In block if.then, function lava_get
| LLVM Instruction | Precondition |
|-----|-----|
|   br label %if.end | (((arrayidx = 10) and ((1234567 + mul4) < 1234567)) or ((not (arrayidx = 10)) and ((0 + mul4) < 1234567))) |
# In block entry, function lava_get
| LLVM Instruction | Precondition |
|-----|-----|
|   br i1 %or.cond, label %if.then, label %if.end | (((arrayidx = 10) and ((1234567 + mul4) < 1234567)) or ((not (arrayidx = 10)) and ((0 + mul4) < 1234567))) |
|   %or.cond = or i1 %cmp, %cmp12 | (((arrayidx = 10) and ((1234567 + mul4) < 1234567)) or ((not (arrayidx = 10)) and ((0 + mul4) < 1234567))) |
|   %cmp12 = icmp eq i32 %or9, %tmp | (((arrayidx = 10) and ((1234567 + mul4) < 1234567)) or ((not (arrayidx = 10)) and ((0 + mul4) < 1234567))) |
|   %cmp = icmp eq i32 %sub, %tmp | (((arrayidx = 10) and ((1234567 + mul4) < 1234567)) or ((not (arrayidx = 10)) and ((0 + mul4) < 1234567))) |
|   %tmp = load i32, i32* %arrayidx, align 4, !tbaa !1 | (((arrayidx = 10) and ((1234567 + mul4) < 1234567)) or ((not (arrayidx = 10)) and ((0 + mul4) < 1234567))) |
|   %arrayidx = getelementptr inbounds [1000000 x i32], [1000000 x i32]* @lava_val, i64 0, i64 %idxprom | ((((lava_val [] idxprom) = 10) and ((1234567 + mul4) < 1234567)) or ((not ((lava_val [] idxprom) = 10)) and ((0 + mul4) < 1234567))) |
|   %idxprom = zext i32 %bug_num to i64 | ((((lava_val [] bug_num) = 10) and ((1234567 + mul4) < 1234567)) or ((not ((lava_val [] bug_num) = 10)) and ((0 + mul4) < 1234567))) |
|   %sub = sub i32 1818326625, %bug_num | ((((lava_val [] bug_num) = 10) and ((1234567 + mul4) < 1234567)) or ((not ((lava_val [] bug_num) = 10)) and ((0 + mul4) < 1234567))) |
|   %call = tail call i32 @lava_get(i32 1) | ((((lava_val [] 1) = 10) and ((1234567 + mul4) < 1234567)) or ((not ((lava_val [] 1) = 10)) and ((0 + mul4) < 1234567))) |
|   %mul4 = mul i32 %mul3, %mul | ((((lava_val [] 1) = 10) and ((1234567 + (mul3 * mul)) < 1234567)) or ((not ((lava_val [] 1) = 10)) and ((0 + (mul3 * mul)) < 1234567))) |
|   %mul3 = mul i32 %mul2, %mul | ((((lava_val [] 1) = 10) and ((1234567 + ((mul2 * mul) * mul)) < 1234567)) or ((not ((lava_val [] 1) = 10)) and ((0 + ((mul2 * mul) * mul)) < 1234567))) |
|   %mul2 = mul i32 %add1, %c | ((((lava_val [] 1) = 10) and ((1234567 + (((add1 * c) * mul) * mul)) < 1234567)) or ((not ((lava_val [] 1) = 10)) and ((0 + (((add1 * c) * mul) * mul)) < 1234567))) |
|   %mul = mul nsw i32 %add1, %b | ((((lava_val [] 1) = 10) and ((1234567 + (((add1 * c) * (add1 * b)) * (add1 * b))) < 1234567)) or ((not ((lava_val [] 1) = 10)) and ((0 + (((add1 * c) * (add1 * b)) * (add1 * b))) < 1234567))) |
# In block entry, function lava_set
| LLVM Instruction | Precondition |
|-----|-----|
|   ret void | ((((lava_val [] 1) = 10) and ((1234567 + (((add1 * c) * (add1 * b)) * (add1 * b))) < 1234567)) or ((not ((lava_val [] 1) = 10)) and ((0 + (((add1 * c) * (add1 * b)) * (add1 * b))) < 1234567))) |
|   store i32 %val, i32* %arrayidx, align 4, !tbaa !1 | ((((lava_val [] 1) = 10) and ((1234567 + (((add1 * c) * (add1 * b)) * (add1 * b))) < 1234567)) or ((not ((lava_val [] 1) = 10)) and ((0 + (((add1 * c) * (add1 * b)) * (add1 * b))) < 1234567))) |
|   %arrayidx = getelementptr inbounds [1000000 x i32], [1000000 x i32]* @lava_val, i64 0, i64 %idxprom | ((((lava_val [] 1) = 10) and ((1234567 + (((add1 * c) * (add1 * b)) * (add1 * b))) < 1234567)) or ((not ((lava_val [] 1) = 10)) and ((0 + (((add1 * c) * (add1 * b)) * (add1 * b))) < 1234567))) |
|   %idxprom = zext i32 %bug_num to i64 | ((((lava_val [] 1) = 10) and ((1234567 + (((add1 * c) * (add1 * b)) * (add1 * b))) < 1234567)) or ((not ((lava_val [] 1) = 10)) and ((0 + (((add1 * c) * (add1 * b)) * (add1 * b))) < 1234567))) |
|   tail call void @lava_set(i32 1, i32 %c) | ((((lava_val [] 1) = 10) and ((1234567 + (((add1 * c) * (add1 * b)) * (add1 * b))) < 1234567)) or ((not ((lava_val [] 1) = 10)) and ((0 + (((add1 * c) * (add1 * b)) * (add1 * b))) < 1234567))) |
|   %add1 = add nsw i32 %add, %b | ((((lava_val [] 1) = 10) and ((1234567 + ((((add + b) * c) * ((add + b) * b)) * ((add + b) * b))) < 1234567)) or ((not ((lava_val [] 1) = 10)) and ((0 + ((((add + b) * c) * ((add + b) * b)) * ((add + b) * b))) < 1234567))) |
|   %add = add nsw i32 %a, 5 | ((((lava_val [] 1) = 10) and ((1234567 + (((((a + 5) + b) * c) * (((a + 5) + b) * b)) * (((a + 5) + b) * b))) < 1234567)) or ((not ((lava_val [] 1) = 10)) and ((0 + (((((a + 5) + b) * c) * (((a + 5) + b) * b)) * (((a + 5) + b) * b))) < 1234567))) |
