cmake_minimum_required(VERSION 3.13)
project(static_wp_lava)
find_package(LLVM REQUIRED CONFIG)

set(CMAKE_CXX_STANDARD 14)
add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})
link_libraries(z3)

link_directories(${LLVM_LIBRARY_DIRS})

add_library(static_wp_lava SHARED src/MainPass.cpp src/WpExpr.cpp src/WpExpr.h src/InsHandler.cpp src/InsHandler.h src/Miscs.cpp src/Miscs.h src/MyZ3Helper.h)

set_target_properties(static_wp_lava PROPERTIES
        COMPILE_FLAGS "-fno-rtti -fPIC"
        )

#add_executable(test_expr src/WpExpr.h src/WpExpr.cpp src/test_expr.cpp)

#set_target_properties(test_expr PROPERTIES
#        COMPILE_FLAGS "-fno-rtti"
#        )
