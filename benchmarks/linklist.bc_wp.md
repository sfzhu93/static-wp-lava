# In block entry, function sum
| LLVM Instruction | Precondition |
|-----|-----|
|   ret i32 %add6 | UPred(add6) |
|   %add6 = add nsw i32 %add, %tmp4 | UPred((add + tmp4)) |
|   %add = add nsw i32 %mul5, %mul | UPred(((mul5 + mul) + tmp4)) |
|   %mul5 = mul nsw i32 %tmp2, 100 | UPred((((tmp2 * 100) + mul) + tmp4)) |
|   %mul = mul nsw i32 %tmp, 10000 | UPred((((tmp2 * 100) + (tmp * 10000)) + tmp4)) |
|   %tmp4 = load i32* %value4, align 4, !tbaa !1 | UPred((((tmp2 * 100) + (tmp * 10000)) + value4)) |
|   %value4 = getelementptr inbounds %struct.node* %tmp3, i64 0, i32 0 | UPred((((tmp2 * 100) + (tmp * 10000)) + _)) |
|   %tmp3 = load %struct.node** %next3, align 8, !tbaa !7 | UPred((((tmp2 * 100) + (tmp * 10000)) + _)) |
|   %next3 = getelementptr inbounds %struct.node* %tmp1, i64 0, i32 1 | UPred((((tmp2 * 100) + (tmp * 10000)) + _)) |
|   %tmp2 = load i32* %value1, align 4, !tbaa !1 | UPred((((value1 * 100) + (tmp * 10000)) + _)) |
|   %value1 = getelementptr inbounds %struct.node* %tmp1, i64 0, i32 0 | UPred((((_ * 100) + (tmp * 10000)) + _)) |
|   %tmp1 = load %struct.node** %next, align 8, !tbaa !7 | UPred((((_ * 100) + (tmp * 10000)) + _)) |
|   %next = getelementptr inbounds %struct.node* %head, i64 0, i32 1 | UPred((((_ * 100) + (tmp * 10000)) + _)) |
|   %tmp = load i32* %value, align 4, !tbaa !1 | UPred((((_ * 100) + (value * 10000)) + _)) |
|   %value = getelementptr inbounds %struct.node* %head, i64 0, i32 0 | UPred((((_ * 100) + (_ * 10000)) + _)) |
# In block entry, function doit
| LLVM Instruction | Precondition |
|-----|-----|
|   %call1 = tail call i32 @sum(%struct.node* %call) | ((((_ * 100) + (_ * 10000)) + _) < 1234567) |
# In block entry, function hello
| LLVM Instruction | Precondition |
|-----|-----|
|   ret %struct.node* %tmp | UPred(tmp) |
|   store %struct.node* null, %struct.node** %next12, align 8, !tbaa !7 | UPred(tmp) |
|   %next12 = getelementptr inbounds %struct.node* %tmp8, i64 0, i32 1 | UPred(tmp) |
|   %tmp8 = load %struct.node** %next11, align 8, !tbaa !7 | UPred(tmp) |
|   %next11 = getelementptr inbounds %struct.node* %tmp7, i64 0, i32 1 | UPred(tmp) |
|   %tmp7 = load %struct.node** %tmp1, align 8, !tbaa !7 | UPred(tmp) |
|   store i32 %c, i32* %value9, align 4, !tbaa !1 | UPred(tmp) |
|   %value9 = getelementptr inbounds %struct.node* %tmp6, i64 0, i32 0 | UPred(tmp) |
|   %tmp6 = load %struct.node** %next8, align 8, !tbaa !7 | UPred(tmp) |
|   %next8 = getelementptr inbounds %struct.node* %tmp5, i64 0, i32 1 | UPred(tmp) |
|   %tmp5 = load %struct.node** %tmp1, align 8, !tbaa !7 | UPred(tmp) |
|   store i8* %call4, i8** %tmp4, align 8, !tbaa !7 | UPred(tmp) |
|   %tmp4 = bitcast %struct.node** %next6 to i8** | UPred(tmp) |
|   %next6 = getelementptr inbounds %struct.node* %tmp3, i64 0, i32 1 | UPred(tmp) |
|   %tmp3 = load %struct.node** %tmp1, align 8, !tbaa !7 | UPred(tmp) |
|   store i32 %b, i32* %value3, align 4, !tbaa !1 | UPred(tmp) |
|   %value3 = bitcast i8* %call1 to i32* | UPred(tmp) |
|   store i8* %call1, i8** %tmp2, align 8, !tbaa !7 | UPred(tmp) |
|   %tmp2 = bitcast i8* %next to i8** | UPred(tmp) |
|   %tmp1 = bitcast i8* %next to %struct.node** | UPred(tmp) |
|   %next = getelementptr inbounds i8* %call, i64 8 | UPred(tmp) |
|   store i32 %a, i32* %value, align 4, !tbaa !1 | UPred(tmp) |
|   %value = bitcast i8* %call to i32* | UPred(tmp) |
|   %tmp = bitcast i8* %call to %struct.node* | UPred(tmp) |
# In block entry, function doit
| LLVM Instruction | Precondition |
|-----|-----|
|   %call = tail call %struct.node* @hello(i32 %a, i32 %b, i32 %c) | ((((_ * 100) + (_ * 10000)) + _) < 1234567) |
