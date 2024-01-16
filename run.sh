#!/usr/bin/env bash
#SAMPLE=func_call1
# SAMPLE=branch
SAMPLE=hello
#SAMPLE=switch_tax_calc
#SAMPLE=herons_formula
#SAMPLE=branch_reachability
#LLVM_VERSION_ATTR=-3.7
LLVM_VERSION_ATTR="-16"
#BUILD_DIR=cmake-build-debug-llvm-37
# BUILD_DIR=cmake-build-debug
BUILD_DIR=build
clang${LLVM_VERSION_ATTR} -c -O1 -fno-discard-value-names -emit-llvm benchmarks/${SAMPLE}.c -o benchmarks/${SAMPLE}.bc
llvm-dis${LLVM_VERSION_ATTR} benchmarks/${SAMPLE}.bc
#instnamer names each variable
# opt${LLVM_VERSION_ATTR} -dot-cfg -instnamer -load ${BUILD_DIR}/libstatic_wp_lava.so -wpgen benchmarks/${SAMPLE}.bc -o /dev/null
opt${LLVM_VERSION_ATTR} -p=instnamer benchmarks/${SAMPLE}.bc -o benchmarks/${SAMPLE}_named.bc
opt${LLVM_VERSION_ATTR} benchmarks/${SAMPLE}.bc -o benchmarks/${SAMPLE}_named.bc
llvm-dis${LLVM_VERSION_ATTR} benchmarks/${SAMPLE}_named.bc
opt${LLVM_VERSION_ATTR} -load-pass-plugin=${BUILD_DIR}/libstatic_wp_lava.so -p=instnamer -p=wpgen benchmarks/${SAMPLE}_named.bc -o /dev/null
# dot -Tpng .hello.dot -o hello.png
