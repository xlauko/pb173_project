#include <random>
#include <vector>
#include <iostream>

#include "StackAllocator.h"
#include "catch.hpp"
#include "test_common.h"

using namespace allocators;

TEST_CASE("Stack allocator allocation") {
    StackAllocator<1024> allocator;

    SECTION("alignment") {
        Block block1 = allocator.allocate(1);
        Block block2 = allocator.allocate(1);
        REQUIRE(static_cast<char*>(block2.ptr) -
                        static_cast<char*>(block1.ptr) ==
                8);
        allocator.deallocate(block2);
        allocator.deallocate(block1);
    }

    SECTION("simple") {
        Block blk1 = allocator.allocate(128);
        Block blk2 = allocator.allocate(256);
        test_block(blk1, 128);
        test_block(blk2, 256);

        REQUIRE(blk1 != blk2);
    }

    SECTION("full") {
        Block blk;
        for (int i = 0; i < 1024 / 8; ++i) {
            blk = allocator.allocate(1);
            test_block(blk, 1);
        }
        blk = allocator.allocate(1);
        test_null(blk);
    }

    SECTION("too big") {
        Block blk = allocator.allocate(1025);
        test_null(blk);
    }

    SECTION("fill with one block") {
        Block blk = allocator.allocate(1024);
        test_block(blk, 1024);
    }
}

TEST_CASE("Stack allocator deallocate") {
    const size_t stack_size = 512;
    StackAllocator<stack_size> allocator1;
    StackAllocator<stack_size> allocator2;

    SECTION("basic") {
        Block blk1 = allocator1.allocate(256);
        Block blk2 = allocator1.allocate(42);
        Block blk3 = allocator2.allocate(64);

        REQUIRE(allocator1.owns(blk1));
        REQUIRE(allocator1.owns(blk2));
        REQUIRE_FALSE(allocator1.owns(blk3));
        REQUIRE(allocator2.owns(blk3));

        allocator1.deallocate(blk2);
        REQUIRE_FALSE(allocator1.owns(blk2));
        test_null(blk2);
        test_block(blk1, 256);
        test_block(blk3, 64);
    }

    SECTION("allocation chunks of 1") {
        std::vector<Block> blocks;
        Block block = allocator1.allocate(1);

        while (block) {
            test_block(block, 1);
            blocks.push_back(block);
            block = allocator1.allocate(1);
        }

        REQUIRE(blocks.size() >= (stack_size / 8));
        while (!blocks.empty()) {
            block = blocks.back();
            allocator1.deallocate(block);
            test_null(block);
            blocks.pop_back();
        }
    }

    SECTION("complex") {
        std::mt19937 gen;
        std::uniform_int_distribution<> dist(1, stack_size / 8);

        std::vector<Block> blocks;

        for (int i = 0; i < 100; ++i) {
            int alloc = dist(gen);
            for (int a = 0; a < alloc; ++a) {
                if (blocks.size() < stack_size / 8) {
                    blocks.push_back(allocator1.allocate(1));
                } else {
                    allocator1.allocate(1);
                }
            }

            test_block(blocks.back(), 1);

            int d = dist(gen);
            while (d > 0 && !blocks.empty()) {
                allocator1.deallocate(blocks.back());
                blocks.pop_back();
                --d;
            }
        }
    }
}

TEST_CASE("Stack allocator comparison") {
    StackAllocator<256> fst, snd;
    REQUIRE_FALSE(fst == snd);
    REQUIRE(fst != snd);
}
