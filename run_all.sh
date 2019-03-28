#!/usr/bin/env bash
SAMPLE=sort3
#SAMPLE=lava1
#SAMPLE=switch_tax_calc
#SAMPLE=herons_formula
#SAMPLE=branch_reachability
#LLVM_VERSION_ATTR=-3.7
LLVM_VERSION_ATTR=""
#BUILD_DIR=cmake-build-debug-llvm-37
BUILD_DIR=cmake-build-debug
cd benchmarks
for SAMPLE in *.c; do
name=$(echo "$SAMPLE" | cut -f 1 -d '.')
echo "working on ${name}"
clang${LLVM_VERSION_ATTR} -c -O1 -emit-llvm  ${name}.c -o ${name}.bc
llvm-dis${LLVM_VERSION_ATTR} ${name}.bc
#instnamer names each variable
opt${LLVM_VERSION_ATTR} -instnamer -load ../${BUILD_DIR}/libstatic_wp_lava.so -wpgen ${name}.bc -o /dev/null > /dev/null
done
