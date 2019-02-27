#!/usr/bin/env bash
clang -c -O1 -emit-llvm  benchmarks/hello.c -o benchmarks/hello.bc
llvm-dis benchmarks/hello.bc
opt -load cmake-build-debug/libstatic_wp_lava.so -wpgen benchmarks/hello.bc -o /dev/null
