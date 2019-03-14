#!/usr/bin/env bash
SAMPLE=lava1
#LLVM_VERSION_ATTR=-3.7
LLVM_VERSION_ATTR=""
#BUILD_DIR=cmake-build-debug-llvm-37
BUILD_DIR=cmake-build-debug
clang${LLVM_VERSION_ATTR} -c -O1 -emit-llvm  benchmarks/${SAMPLE}.c -o benchmarks/${SAMPLE}.bc
llvm-dis${LLVM_VERSION_ATTR} benchmarks/${SAMPLE}.bc
#instnamer names each variable
opt${LLVM_VERSION_ATTR} -instnamer -load ${BUILD_DIR}/libstatic_wp_lava.so -wpgen benchmarks/${SAMPLE}.bc -o /dev/null
