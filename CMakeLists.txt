cmake_minimum_required(VERSION 3.4)
project(pb173_project)

add_compile_options(-std=c++14)

add_library(allocators STATIC
    src/type_classes
    src/Block
    src/NullAllocator
    src/StackAllocator
    src/FallbackAllocator
    src/Mallocator
    src/Freelist
    src/Segregator
    src/AffixAllocator
    src/Bucketizer)
set_target_properties(allocators PROPERTIES
    LINKER_LANGUAGE CXX)
target_include_directories(allocators INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

add_executable(unit_tests
    test/main.cpp
    test/test_common
    test/test_null.cpp
    test/test_stack.cpp
    test/test_mallocator.cpp
    test/test_fallback.cpp
    test/test_freelist.cpp
    test/test_segregator.cpp
    test/test_affix.cpp
#    test/test_bucketizer.cpp
)
target_link_libraries(unit_tests allocators)
