cmake_minimum_required(VERSION 3.19)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(static_wp_lava)

find_package(LLVM 16 REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_definitions(${LLVM_DEFINITIONS})

include_directories(${LLVM_INCLUDE_DIRS})
include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_library(static_wp_lava SHARED ${SOURCES})
# add_llvm_pass_plugin(static_wp_lava)
target_compile_definitions(static_wp_lava PRIVATE ${LLVM_DEFINITIONS})

llvm_map_components_to_libnames(llvm_libs support core)
# target_link_libraries(static_wp_lava PRIVATE ${llvm_libs} z3)
target_link_libraries(static_wp_lava PRIVATE LLVM z3)

# Compiler options
if(NOT LLVM_ENABLE_RTTI)
set_target_properties(static_wp_lava PROPERTIES
    COMPILE_FLAGS "-fno-rtti"
)
endif()


# Test executable
# add_executable(test_expr tests/test_expr.cpp)
# target_link_libraries(test_expr PRIVATE static_wp_lava z3)