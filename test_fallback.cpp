#include "FallbackAllocator.h"
#include "Mallocator.h"
#include "StackAllocator.h"
#include "catch.hpp"
#include "common.h"

TEST_CASE("Fallback allocator allocation") {
    allocators::FallbackAllocator<allocators::StackAllocator<256>,
                                  allocators::StackAllocator<256>> allocator;

    SECTION("simple") {
        Block blk1 = allocator.allocate(128);
        Block blk2 = allocator.allocate(256);
        test_block(blk1, 128);
        test_block(blk2, 256);

        REQUIRE(blk1 != blk2);
    }

    SECTION("full") {
        Block blk;
        for (int i = 0; i < 512; ++i) {
            blk = allocator.allocate(1);
            test_block(blk, 1);
        }
        blk = allocator.allocate(1);
        test_null(blk);
    }
}

TEST_CASE("Fallback allocator deallocate") {
    const size_t stack_size = 256;
    allocators::FallbackAllocator<allocators::StackAllocator<stack_size>,
                                  allocators::StackAllocator<stack_size>>
            allocator;

    SECTION("simple") {
        Block blk1 = allocator.allocate(256);
        Block blk2 = allocator.allocate(128);
        Block blk3 = allocator.allocate(64);

        REQUIRE(allocator.owns(blk1));
        allocator.deallocate(blk1);
        test_null(blk1);
        REQUIRE_FALSE(allocator.owns(blk1));

        allocator.deallocate(blk3);
        REQUIRE_FALSE(allocator.owns(blk3));
        REQUIRE(allocator.owns(blk2));
        allocator.deallocate(blk2);
        test_null(blk2);
    }

    SECTION("complex") {
        std::mt19937 gen;
        std::uniform_int_distribution<> dist(1, stack_size);

        std::vector<Block> blocks1;
        std::vector<Block> blocks2;

        int alloc = dist(gen) * 2;
        for (int a = 0; a < alloc; ++a) {
            if (blocks1.size() < stack_size) {
                blocks1.push_back(allocator.allocate(1));
            } else {
                blocks2.push_back(allocator.allocate(1));
            }
        }
        test_block(blocks1.back(), 1);
        test_block(blocks2.back(), 1);

        int d = dist(gen);
        for (int i = 0; i < d && !blocks1.empty(); ++i) {
            allocator.deallocate(blocks1.back());
            blocks1.pop_back();
        }

        d = dist(gen);

        for (int i = 0; i < d && !blocks2.empty(); ++i) {
            allocator.deallocate(blocks2.back());
            blocks2.pop_back();
        }
    }
}

TEST_CASE("Fallback allocator comparison") {
    allocators::FallbackAllocator<allocators::StackAllocator<256>,
                                  allocators::Mallocator> fst,
            snd;
    REQUIRE_FALSE(fst == snd);
    REQUIRE(fst != snd);
}
