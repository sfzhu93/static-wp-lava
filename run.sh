#!/usr/bin/env bash
SAMPLE=lava1
clang-3.7 -c -O1 -emit-llvm  benchmarks/${SAMPLE}.c -o benchmarks/${SAMPLE}.bc
llvm-dis-3.7 benchmarks/${SAMPLE}.bc
#instnamer names each variable
opt-3.7 -instnamer -load cmake-build-debug-llvm-37/libstatic_wp_lava.so -wpgen benchmarks/${SAMPLE}.bc -o /dev/null
