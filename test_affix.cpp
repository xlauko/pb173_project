//
// Created by kejsty on 24.4.16.
//

#include "catch.hpp"
#include "AffixAllocator.h"
#include "StackAllocator.h"
#include "common.h"
#include "Mallocator.h"

using namespace allocators;

TEST_CASE("Basic") {
    AffixAllocator<StackAllocator<10>, int, int> allocator;
    Block blk1 = allocator.allocate(5);
    test_block(blk1, 5);
    *allocator.prefix(blk1) = 4;
    *allocator.suffix(blk1) = 6;
    char* text = reinterpret_cast<char*>(blk1.ptr);
    char tmp[] = "ahoj\0";
    text = tmp;
    REQUIRE(*allocator.prefix(blk1) == 4);
    REQUIRE(*allocator.suffix(blk1) == 6);
    allocator.deallocate(blk1);
    test_null(blk1);
}

TEST_CASE("More allocations on Mallocator") {
    AffixAllocator<Mallocator, double, char> allocator;
    Block blocks[6];
    for (size_t i = 0; i < 6; ++i) {
        blocks[i] = allocator.allocate(6);
        assert(blocks[i]);
        test_block(blocks[i], 6);
        *allocator.prefix(blocks[i]) = 4.0;
        *allocator.suffix(blocks[i]) = 'a';
        char* text = reinterpret_cast<char*>(blocks[i].ptr);
        char tmp[] = "@!#!!\0";
        text = tmp;
    }
    for (size_t i = 0; i < 6; ++i) {
        REQUIRE(*allocator.prefix(blocks[i]) == 4.0);
        REQUIRE(*allocator.suffix(blocks[i]) == 'a');
        allocator.deallocate(blocks[i]);
        test_null(blocks[i]);
    }
}

TEST_CASE("More allocations on StackAllocator") {
    AffixAllocator<StackAllocator<1024>, double, char> allocator;
    Block blocks[100];
    for (size_t i = 0; i < 100; ++i) {
        blocks[i] = allocator.allocate(6);
        assert(blocks[i]);
        test_block(blocks[i], 6);
        *allocator.prefix(blocks[i]) = 4.0;
        *allocator.suffix(blocks[i]) = 'a';
        char* text = reinterpret_cast<char*>(blocks[i].ptr);
        char tmp[] = "@$$^^\0";
        text = tmp;
    }

    SECTION("Deallocate") {
        for (size_t i = 0; i < 100; ++i) {
            test_block(blocks[i]);
            allocator.deallocate(blocks[i]);
            test_null(blocks[i]);
        }
    }

    SECTION("No overwrites") {
        for (size_t i = 0; i < 100; ++i) {
            REQUIRE(*allocator.prefix(blocks[i]) == 4.0);
            REQUIRE(*allocator.suffix(blocks[i]) == 'a');
            REQUIRE(*blocks[i].ptr == '@');
            allocator.deallocate(blocks[i]);
            test_null(blocks[i]);
        }
    }
}

TEST_CASE("Allocation with NullAllocator") {
    AffixAllocator<NullAllocator, double, char> allocator;
    Block block = allocator.allocate(10);
    test_null(block);
}
