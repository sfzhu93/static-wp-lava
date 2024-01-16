# In block , function hello
| LLVM Instruction | Precondition |
|-----|-----|
|   %18 = mul i32 %17, %16 | (( * ) < 1234567) |
|   %17 = mul i32 %4, %2 | ((( * ) * ) < 1234567) |
|   %16 = phi i32 [ %11, %6 ], [ %14, %12 ] | (( and ((( * ) * ) < )) or ( and ((( * ) * ) < ))) |
# In block , function hello
| LLVM Instruction | Precondition |
|-----|-----|
|   br label %15 | (( and ((( * ) * ) < )) or ( and ((( * ) * ) < ))) |
|   %11 = mul i32 %10, %7 | (( and ((( * ) * ( * )) < )) or ( and ((( * ) * ) < ))) |
|   %10 = mul i32 %9, %8 | (( and ((( * ) * (( * ) * )) < )) or ( and ((( * ) * ) < ))) |
|   %9 = add nsw i32 %2, -1 | (( and ((( * ) * ((( + -1) * ) * )) < )) or ( and ((( * ) * ) < ))) |
|   %8 = add nsw i32 %1, -1 | (( and ((( * ) * ((( + -1) * ( + -1)) * )) < )) or ( and ((( * ) * ) < ))) |
|   %7 = add nsw i32 %4, -1 | (( and ((( * ) * ((( + -1) * ( + -1)) * ( + -1))) < )) or ( and ((( * ) * ) < ))) |
# In block , function hello
| LLVM Instruction | Precondition |
|-----|-----|
|   br label %15 | (( and ((( * ) * ((( + -1) * ( + -1)) * ( + -1))) < )) or ( and ((( * ) * ) < ))) |
|   %14 = mul nsw i32 %13, %1 | (( and ((( * ) * ((( + -1) * ( + -1)) * ( + -1))) < )) or ( and ((( * ) * ( * )) < ))) |
|   %13 = add nsw i32 %4, 1 | (( and ((( * ) * ((( + -1) * ( + -1)) * ( + -1))) < )) or ( and ((( * ) * (( + 1) * )) < ))) |
# In block , function hello
| LLVM Instruction | Precondition |
|-----|-----|
|   br i1 %5, label %6, label %12 | (( and ((( * ) * ((( + -1) * ( + -1)) * ( + -1))) < )) or ((not ) and ((( * ) * (( + 1) * )) < ))) |
|   %5 = icmp sgt i32 %4, 2 | ((( > 2) and ((( * ) * ((( + -1) * ( + -1)) * ( + -1))) < )) or ((not ( > 2)) and ((( * ) * (( + 1) * )) < ))) |
|   %4 = add nsw i32 %2, %1 | (((( + ) > 2) and (((( + ) * ) * ((( + -1) * ( + -1)) * (( + ) + -1))) < )) or ((not (( + ) > 2)) and (((( + ) * ) * ((( + ) + 1) * )) < ))) |
