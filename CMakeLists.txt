cmake_minimum_required(VERSION 3.4)
project(pb173_project)

add_compile_options(-std=c++14)

set(SOURCE_FILES main.cpp
    type_classes
    Block
    NullAllocator
    StackAllocator
    FallbackAllocator
    Mallocator
    Freelist
    Segregator
    AffixAllocator
    Bucketizer
# unit tests
    test_common
    test_null.cpp 
    test_stack.cpp
    test_mallocator.cpp
    test_fallback.cpp
    test_freelist.cpp
    test_segregator.cpp
    test_affix.cpp
    test_bucketizer.cpp)
add_executable(pb173_project ${SOURCE_FILES})


