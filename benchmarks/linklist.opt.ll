; ModuleID = 'linklist.opt.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.node = type { i32, %struct.node* }

@.str = private unnamed_addr constant [2 x i8] c".\00", align 1
@.str.1 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1

; Function Attrs: noinline nounwind uwtable
define void @_wp_begin() #0 {
entry:
  %putchar = tail call i32 @putchar(i32 46) #4
  ret void
}

; Function Attrs: noinline nounwind uwtable
define i8* @_wp_end(i8* readnone %a) #0 {
entry:
  %putchar = tail call i32 @putchar(i32 46) #4
  ret i8* %a
}

; Function Attrs: nounwind uwtable
define noalias %struct.node* @hello(i32 %a, i32 %b, i32 %c) #1 {
entry:
  %call = tail call noalias i8* @malloc(i64 16) #4
  %0 = bitcast i8* %call to %struct.node*
  %value = bitcast i8* %call to i32*
  store i32 %a, i32* %value, align 4, !tbaa !1
  %call1 = tail call noalias i8* @malloc(i64 16) #4
  %next = getelementptr inbounds i8, i8* %call, i64 8
  %1 = bitcast i8* %next to %struct.node**
  %2 = bitcast i8* %next to i8**
  store i8* %call1, i8** %2, align 8, !tbaa !7
  %value3 = bitcast i8* %call1 to i32*
  store i32 %b, i32* %value3, align 4, !tbaa !1
  %call4 = tail call noalias i8* @malloc(i64 16) #4
  %next6 = getelementptr inbounds i8, i8* %call1, i64 8
  %3 = bitcast i8* %next6 to i8**
  store i8* %call4, i8** %3, align 8, !tbaa !7
  %4 = load %struct.node*, %struct.node** %1, align 8, !tbaa !7
  %next8 = getelementptr inbounds %struct.node, %struct.node* %4, i64 0, i32 1
  %5 = load %struct.node*, %struct.node** %next8, align 8, !tbaa !7
  %value9 = getelementptr inbounds %struct.node, %struct.node* %5, i64 0, i32 0
  store i32 %c, i32* %value9, align 4, !tbaa !1
  %6 = load %struct.node*, %struct.node** %next8, align 8, !tbaa !7
  %next12 = getelementptr inbounds %struct.node, %struct.node* %6, i64 0, i32 1
  store %struct.node* null, %struct.node** %next12, align 8, !tbaa !7
  ret %struct.node* %0
}

; Function Attrs: nounwind
declare noalias i8* @malloc(i64) #2

; Function Attrs: nounwind readonly uwtable
define i32 @sum(%struct.node* nocapture readonly %head) #3 {
entry:
  %value = getelementptr inbounds %struct.node, %struct.node* %head, i64 0, i32 0
  %0 = load i32, i32* %value, align 4, !tbaa !1
  %next = getelementptr inbounds %struct.node, %struct.node* %head, i64 0, i32 1
  %1 = load %struct.node*, %struct.node** %next, align 8, !tbaa !7
  %value1 = getelementptr inbounds %struct.node, %struct.node* %1, i64 0, i32 0
  %2 = load i32, i32* %value1, align 4, !tbaa !1
  %next3 = getelementptr inbounds %struct.node, %struct.node* %1, i64 0, i32 1
  %3 = load %struct.node*, %struct.node** %next3, align 8, !tbaa !7
  %value4 = getelementptr inbounds %struct.node, %struct.node* %3, i64 0, i32 0
  %4 = load i32, i32* %value4, align 4, !tbaa !1
  %mul = mul nsw i32 %0, 10000
  %mul5 = mul nsw i32 %2, 100
  %add = add nsw i32 %mul5, %mul
  %add6 = add nsw i32 %add, %4
  ret i32 %add6
}

; Function Attrs: nounwind uwtable
define i32 @doit(i32 %a, i32 %b, i32 %c) #1 {
entry:
  tail call void @_wp_begin()
  %call = tail call %struct.node* @hello(i32 %a, i32 %b, i32 %c)
  %call1 = tail call i32 @sum(%struct.node* %call)
  %call2 = tail call i8* @_wp_end(i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str.1, i64 0, i64 0))
  ret i32 %call1
}

; Function Attrs: nounwind
declare i32 @putchar(i32) #4

attributes #0 = { noinline nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind readonly uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { nounwind }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.7.1 (tags/RELEASE_371/final)"}
!1 = !{!2, !3, i64 0}
!2 = !{!"node", !3, i64 0, !6, i64 8}
!3 = !{!"int", !4, i64 0}
!4 = !{!"omnipotent char", !5, i64 0}
!5 = !{!"Simple C/C++ TBAA"}
!6 = !{!"any pointer", !4, i64 0}
!7 = !{!2, !6, i64 8}
