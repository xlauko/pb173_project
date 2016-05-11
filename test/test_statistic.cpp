#include <Mallocator.h>
#include "StackAllocator.h"
#include "StatisticCollector.h"
#include "catch.hpp"
#include "test_common.h"

TEST_CASE("basic") {
    using namespace allocators;
    StatisticCollector<Mallocator> allocator;
    REQUIRE(allocator.result() == 0);
    Block block = allocator.allocate(5);
    test_block(block, 5);
    REQUIRE(allocator.result() == 1);
    allocator.deallocate(block);
    test_null(block);
    REQUIRE(allocator.result() == 1);
}

TEST_CASE("Options") {
    using namespace allocators;
    SECTION("Allocate") {
        StatisticCollector<Mallocator, Options::numAllocate> allocator;
        Block blocks[100];
        for (int i = 0; i < 100; ++i) {
            blocks[i] = allocator.allocate(i + 1);
            test_block(blocks[i], i + 1);
            REQUIRE(allocator.result() == i + 1);
        }
        for (int j = 0; j < 100; ++j) {
            allocator.deallocate(blocks[j]);
        }
    }

    SECTION("Deallocate") {
        StatisticCollector<Mallocator, Options::numDeallocate> allocator;
        Block blocks[100];
        for (int i = 0; i < 100; ++i) {
            blocks[i] = allocator.allocate(i + 1);
            test_block(blocks[i], i + 1);
            REQUIRE(allocator.result() == 0);
        }
        for (int j = 0; j < 100; ++j) {
            allocator.deallocate(blocks[j]);
            REQUIRE(allocator.result() == j + 1);
            test_null(blocks[j]);
        }
    }

    SECTION("AllocateOk") {
        StatisticCollector<StackAllocator<128>, Options::numAllocateOk>
                allocator;
        for (int i = 0; i < 30; ++i) {
            Block blk = allocator.allocate(1);
            i < 16 ? test_block(blk, 1) : test_null(blk);
        }
        REQUIRE(allocator.result() == 16);
    }

    SECTION("Owns") {
        StatisticCollector<StackAllocator<128>, Options::numOwns> allocator;
        Block blk = allocator.allocate(110);
        REQUIRE(allocator.result() == 0);
        for (int i = 0; i < 10; ++i) {
            REQUIRE(allocator.owns(blk));
        }
        REQUIRE(allocator.result() == 10);
        for (int j = 0; j < 10; ++j) {
            allocator.owns(Block());
        }
        REQUIRE(allocator.result() == 20);
    }

    SECTION("NumAll") {
        StatisticCollector<Mallocator, Options::numAll> allocator;
        Block blocks[100];
        for (int i = 0; i < 100; ++i) {
            blocks[i] = allocator.allocate(i + 1);
            test_block(blocks[i], i + 1);
            REQUIRE(allocator.result() == i + 1);
        }
        for (int j = 0; j < 100; ++j) {
            allocator.deallocate(blocks[j]);
            REQUIRE(allocator.result() == 100 + j + 1);
            test_null(blocks[j]);
        }
    }

    SECTION("BytesAllocated") {
        StatisticCollector<Mallocator, Options::bytesAllocated> allocator;
        Block blocks[100];
        int count = 0;
        for (int i = 0; i < 100; ++i) {
            count += i + 1;
            blocks[i] = allocator.allocate(i + 1);
            test_block(blocks[i], i + 1);
            REQUIRE(allocator.result() == count);
        }
        for (int j = 0; j < 100; ++j) {
            allocator.deallocate(blocks[j]);
            count -= j + 1;
            REQUIRE(allocator.result() == count);
            test_null(blocks[j]);
        }
    }

    SECTION("bytesHighTide") {
        StatisticCollector<Mallocator, Options::bytesHighTide> allocator;
        Block blocks[100];
        for (int i = 0; i < 100; ++i) {
            blocks[i] = allocator.allocate(i + 1);
            test_block(blocks[i], i + 1);
            REQUIRE(allocator.result() == i + 1);
        }
        for (int j = 0; j < 100; ++j) {
            allocator.deallocate(blocks[j]);
            REQUIRE(allocator.result() == 100);
            test_null(blocks[j]);
        }
    }
}