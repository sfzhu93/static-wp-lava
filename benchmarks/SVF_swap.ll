; ModuleID = 'SVF_swap.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: norecurse nounwind uwtable
define void @swap(i8** nocapture %p, i8** nocapture %q) #0 {
entry:
  %0 = bitcast i8** %p to i64*
  %1 = load i64, i64* %0, align 8, !tbaa !1
  %2 = bitcast i8** %q to i64*
  %3 = load i64, i64* %2, align 8, !tbaa !1
  store i64 %3, i64* %0, align 8, !tbaa !1
  store i64 %1, i64* %2, align 8, !tbaa !1
  ret void
}

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.start(i64, i8* nocapture) #1

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.end(i64, i8* nocapture) #1

; Function Attrs: norecurse nounwind uwtable
define i32 @main() #0 {
entry:
  %a1 = alloca i8, align 1
  %b1 = alloca i8, align 1
  %a = alloca i8*, align 8
  %b = alloca i8*, align 8
  call void @llvm.lifetime.start(i64 1, i8* nonnull %a1) #2
  call void @llvm.lifetime.start(i64 1, i8* nonnull %b1) #2
  %0 = bitcast i8** %a to i8*
  call void @llvm.lifetime.start(i64 8, i8* %0) #2
  store i8* %a1, i8** %a, align 8, !tbaa !1
  %1 = bitcast i8** %b to i8*
  call void @llvm.lifetime.start(i64 8, i8* %1) #2
  store i8* %b1, i8** %b, align 8, !tbaa !1
  call void @swap(i8** nonnull %a, i8** nonnull %b)
  call void @llvm.lifetime.end(i64 8, i8* %1) #2
  call void @llvm.lifetime.end(i64 8, i8* %0) #2
  call void @llvm.lifetime.end(i64 1, i8* nonnull %b1) #2
  call void @llvm.lifetime.end(i64 1, i8* nonnull %a1) #2
  ret i32 0
}

attributes #0 = { norecurse nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { argmemonly nounwind }
attributes #2 = { nounwind }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.1 (tags/RELEASE_381/final)"}
!1 = !{!2, !2, i64 0}
!2 = !{!"any pointer", !3, i64 0}
!3 = !{!"omnipotent char", !4, i64 0}
!4 = !{!"Simple C/C++ TBAA"}
