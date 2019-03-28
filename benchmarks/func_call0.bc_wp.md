# In block if.end, function hello
| LLVM Instruction | Precondition |
|-----|-----|
|   %ret.0 = phi i32 [ %call, %if.then ], [ %sub3, %if.else ] | ((if.then and (call < 1234567)) or (if.else and (sub3 < 1234567))) |
# In block if.then, function hello
| LLVM Instruction | Precondition |
|-----|-----|
|   br label %if.end | ((if.then and (call < 1234567)) or (if.else and (sub3 < 1234567))) |
# In block entry, function max2
| LLVM Instruction | Precondition |
|-----|-----|
|   ret i32 %a.b | UPred( a.b ) |
|   %a.b = select i1 %cmp, i32 %a, i32 %b | ((cmp and UPred( a )) or ((not cmp) and UPred( b ))) |
|   %cmp = icmp sgt i32 %a, %b | (((a > b) and UPred( a )) or ((not (a > b)) and UPred( b ))) |
|   %call = tail call i32 @max2(i32 %a, i32 %b) | ((if.then and (call < 1234567)) or (if.else and (sub3 < 1234567))) |
# In block if.else, function max2
| LLVM Instruction | Precondition |
|-----|-----|
|   br label %if.end | ((if.then and (call < 1234567)) or (if.else and (sub3 < 1234567))) |
|   %sub3 = sub nsw i32 0, %call2 | ((if.then and (call < 1234567)) or (if.else and ((0 - call2) < 1234567))) |
# In block entry, function max2
| LLVM Instruction | Precondition |
|-----|-----|
|   ret i32 %a.b | UPred( a.b ) |
|   %a.b = select i1 %cmp, i32 %a, i32 %b | ((cmp and UPred( a )) or ((not cmp) and UPred( b ))) |
|   %cmp = icmp sgt i32 %a, %b | (((a > b) and UPred( a )) or ((not (a > b)) and UPred( b ))) |
|   %call2 = tail call i32 @max2(i32 %sub, i32 %sub1) | ((if.then and (call < 1234567)) or (if.else and ((0 - call2) < 1234567))) |
|   %sub1 = sub nsw i32 0, %b | ((if.then and (call < 1234567)) or (if.else and ((0 - call2) < 1234567))) |
|   %sub = sub nsw i32 0, %a | ((if.then and (call < 1234567)) or (if.else and ((0 - call2) < 1234567))) |
# In block entry, function max2
| LLVM Instruction | Precondition |
|-----|-----|
|   br i1 %cmp, label %if.then, label %if.else | ((cmp and (call < 1234567)) or ((NOT cmp) and ((0 - call2) < 1234567))) |
|   %cmp = icmp sgt i32 %c, 0 | (((c > 0) and (call < 1234567)) or ((NOT (c > 0)) and ((0 - call2) < 1234567))) |
