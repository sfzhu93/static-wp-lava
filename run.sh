#!/usr/bin/env bash
clang -c -O1 -emit-llvm  benchmarks/loop.c -o benchmarks/loop.bc
llvm-dis benchmarks/loop.bc
opt -load cmake-build-debug/libstatic_wp_lava.so -wpgen benchmarks/loop.bc -o /dev/null
