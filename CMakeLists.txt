cmake_minimum_required(VERSION 3.13)
project(static_wp_lava)
find_package(LLVM REQUIRED CONFIG)

set(CMAKE_CXX_STANDARD 14)
add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})

add_library(static_wp_lava SHARED src/MainPass.cpp src/MainPass.h)

set_target_properties(static_wp_lava PROPERTIES
        COMPILE_FLAGS "-fno-rtti"
        )