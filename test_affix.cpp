//
// Created by kejsty on 24.4.16.
//

#include "catch.hpp"
#include "AffixAllocator.h"
#include "StackAllocator.h"
#include "common.h"
#include "Mallocator.h"
#include "NullAllocator.h"

using namespace allocators;

TEST_CASE("Basic") {
    AffixAllocator<StackAllocator<20>, int, int> allocator;
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
    AffixAllocator<Mallocator, char, char> allocator;
    Block blocks[6];
    for (size_t i = 0; i < 6; ++i) {
        blocks[i] = allocator.allocate(6);
        test_block(blocks[i], 6);
        *allocator.prefix(blocks[i]) = 'v';
        *allocator.suffix(blocks[i]) = 'a';
    }
    for (size_t i = 0; i < 6; ++i) {
        REQUIRE(*allocator.prefix(blocks[i]) == 'v');
        REQUIRE(*allocator.suffix(blocks[i]) == 'a');
        allocator.deallocate(blocks[i]);
        test_null(blocks[i]);
    }
}

TEST_CASE("More allocations on StackAllocator") {
    AffixAllocator<StackAllocator<16384>, double, double> allocator;
    Block blocks[20];
    for (size_t i = 1; i < 20; ++i) {
        blocks[i] = allocator.allocate(6);
        test_block(blocks[i], 6);
        *allocator.prefix(blocks[i]) = 4.0;
        *allocator.suffix(blocks[i]) = 6.0;
    }

    SECTION("Deallocate") {
        for (size_t i = 19; i != 0; --i) {
            test_block(blocks[i], 6);
            allocator.deallocate(blocks[i]);
            test_null(blocks[i]);
        }
    }

    SECTION("No overwrites") {
        for (size_t i = 19; i != 0; --i) {
            REQUIRE(*allocator.prefix(blocks[i]) == 4.0);
            REQUIRE(*allocator.suffix(blocks[i]) == 6.0);
            allocator.deallocate(blocks[i]);
            test_null(blocks[i]);
        }
    }
}

TEST_CASE("AffixAllocator with AffixAllocator") {
    AffixAllocator<AffixAllocator<Mallocator, char, char>, double, double>
            allocator;
    SECTION("Simple") {
        Block block1 = allocator.allocate(10);
        test_block(block1, 10);
        allocator.deallocate(block1);
        test_null(block1);
    }
    SECTION("Complex") {
        Block blocks[10];
        for (int i = 0; i < 10; ++i) {
            blocks[i] = allocator.allocate(10);
            *allocator.prefix(blocks[i]) = 4.0;
            *allocator.suffix(blocks[i]) = 7.0;
            test_block(blocks[i], 10);
        }
        for (int j = 0; j < 10; ++j) {
            REQUIRE(*allocator.prefix(blocks[j]) == 4.0);
            REQUIRE(*allocator.suffix(blocks[j]) == 7.0);
        }
        for (int k = 0; k < 10; ++k) {
            allocator.deallocate(blocks[k]);
            test_null(blocks[k]);
        }
    }
}

TEST_CASE("Allocation with NullAllocator") {
    AffixAllocator<NullAllocator, double, char> allocator;
    Block block = allocator.allocate(10);
    test_null(block);
}
