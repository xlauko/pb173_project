cmake_minimum_required(VERSION 3.4)
project(pb173_project)

add_compile_options(-std=c++14)

set(SOURCE_FILES main.cpp 
    test_null.cpp 
    test_stack.cpp
    test_mallocator.cpp
    test_fallback.cpp
    base NullAllocator StackAllocator FallbackAllocator Mallocator)
add_executable(pb173_project ${SOURCE_FILES})


