cmake_minimum_required(VERSION 3.4)
project(pb173_project)

add_compile_options(-std=c++14)

include(ExternalProject)
ExternalProject_Add(google-benchmark
    GIT_SUBMODULES "${PROJECT_SOURCE_DIR}/google-benchmark"
    SOURCE_DIR "${PROJECT_SOURCE_DIR}/google-benchmark"
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gb
    INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/gb
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/gb
    UPDATE_COMMAND "")


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
    src/BitSet
    src/BitmappedBlock
    src/Bucketizer
    src/StatisticCollector)
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
    test/test_statistic.cpp)
target_link_libraries(unit_tests allocators)


link_directories(${CMAKE_CURRENT_BINARY_DIR}/gb/lib)
add_executable(bench
    bench/main.cpp
    bench/bench_mallocator.cpp)
target_link_libraries(bench benchmark pthread)
add_dependencies(bench google-benchmark)
target_compile_options(bench PRIVATE -O3 -fno-omit-frame-pointer)
target_include_directories(bench
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/gb/include)
