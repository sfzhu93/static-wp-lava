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
|   %call = tail call i32 @max2(i32 %a, i32 %b) | (((a > b) and ((if.then and (a < 1234567)) or (if.else and (sub3 < 1234567)))) or ((not (a > b)) and ((if.then and (b < 1234567)) or (if.else and (sub3 < 1234567))))) |
# In block if.else, function max2
| LLVM Instruction | Precondition |
|-----|-----|
|   br label %if.end | (((a > b) and ((if.then and (a < 1234567)) or (if.else and (sub3 < 1234567)))) or ((not (a > b)) and ((if.then and (b < 1234567)) or (if.else and (sub3 < 1234567))))) |
|   %sub3 = sub nsw i32 0, %call2 | (((a > b) and ((if.then and (a < 1234567)) or (if.else and ((0 - call2) < 1234567)))) or ((not (a > b)) and ((if.then and (b < 1234567)) or (if.else and ((0 - call2) < 1234567))))) |
# In block entry, function max2
| LLVM Instruction | Precondition |
|-----|-----|
|   ret i32 %a.b | UPred( a.b ) |
|   %a.b = select i1 %cmp, i32 %a, i32 %b | ((cmp and UPred( a )) or ((not cmp) and UPred( b ))) |
|   %cmp = icmp sgt i32 %a, %b | (((a > b) and UPred( a )) or ((not (a > b)) and UPred( b ))) |
|   %call2 = tail call i32 @max2(i32 %sub, i32 %sub1) | (((sub > sub1) and (((a > b) and ((if.then and (a < 1234567)) or (if.else and ((0 - sub) < 1234567)))) or ((not (a > b)) and ((if.then and (b < 1234567)) or (if.else and ((0 - sub) < 1234567)))))) or ((not (sub > sub1)) and (((a > b) and ((if.then and (a < 1234567)) or (if.else and ((0 - sub1) < 1234567)))) or ((not (a > b)) and ((if.then and (b < 1234567)) or (if.else and ((0 - sub1) < 1234567))))))) |
|   %sub1 = sub nsw i32 0, %b | (((sub > (0 - b)) and (((a > b) and ((if.then and (a < 1234567)) or (if.else and ((0 - sub) < 1234567)))) or ((not (a > b)) and ((if.then and (b < 1234567)) or (if.else and ((0 - sub) < 1234567)))))) or ((not (sub > (0 - b))) and (((a > b) and ((if.then and (a < 1234567)) or (if.else and ((0 - (0 - b)) < 1234567)))) or ((not (a > b)) and ((if.then and (b < 1234567)) or (if.else and ((0 - (0 - b)) < 1234567))))))) |
|   %sub = sub nsw i32 0, %a | ((((0 - a) > (0 - b)) and (((a > b) and ((if.then and (a < 1234567)) or (if.else and ((0 - (0 - a)) < 1234567)))) or ((not (a > b)) and ((if.then and (b < 1234567)) or (if.else and ((0 - (0 - a)) < 1234567)))))) or ((not ((0 - a) > (0 - b))) and (((a > b) and ((if.then and (a < 1234567)) or (if.else and ((0 - (0 - b)) < 1234567)))) or ((not (a > b)) and ((if.then and (b < 1234567)) or (if.else and ((0 - (0 - b)) < 1234567))))))) |
# In block entry, function max2
| LLVM Instruction | Precondition |
|-----|-----|
|   br i1 %cmp, label %if.then, label %if.else | ((((0 - a) > (0 - b)) and (((a > b) and ((cmp and (a < 1234567)) or ((NOT cmp) and ((0 - (0 - a)) < 1234567)))) or ((not (a > b)) and ((cmp and (b < 1234567)) or ((NOT cmp) and ((0 - (0 - a)) < 1234567)))))) or ((not ((0 - a) > (0 - b))) and (((a > b) and ((cmp and (a < 1234567)) or ((NOT cmp) and ((0 - (0 - b)) < 1234567)))) or ((not (a > b)) and ((cmp and (b < 1234567)) or ((NOT cmp) and ((0 - (0 - b)) < 1234567))))))) |
|   %cmp = icmp sgt i32 %c, 0 | ((((0 - a) > (0 - b)) and (((a > b) and (((c > 0) and (a < 1234567)) or ((NOT (c > 0)) and ((0 - (0 - a)) < 1234567)))) or ((not (a > b)) and (((c > 0) and (b < 1234567)) or ((NOT (c > 0)) and ((0 - (0 - a)) < 1234567)))))) or ((not ((0 - a) > (0 - b))) and (((a > b) and (((c > 0) and (a < 1234567)) or ((NOT (c > 0)) and ((0 - (0 - b)) < 1234567)))) or ((not (a > b)) and (((c > 0) and (b < 1234567)) or ((NOT (c > 0)) and ((0 - (0 - b)) < 1234567))))))) |
