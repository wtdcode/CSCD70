cmake_minimum_required(VERSION 3.5)

# https://llvm.org/docs/CMake.html#developing-llvm-passes-out-of-source

find_package(LLVM REQUIRED CONFIG)

add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})

add_library(localopts MODULE pass.cpp)