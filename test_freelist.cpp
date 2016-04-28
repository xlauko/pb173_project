#include "Freelist.h"
#include "Mallocator.h"
#include "catch.hpp"
#include "common.h"

TEST_CASE("Freelist allocate") {
    allocators::Freelist<allocators::Mallocator, 32, 64, 1024> alloc8r;

    SECTION("Simple") {
        Block blk1 = alloc8r.allocate(32);
        Block blk2 = alloc8r.allocate(48);
        test_block(blk1, 64);
        test_block(blk2, 64);

        REQUIRE(blk1 != blk2);
        alloc8r.deallocate(blk1);
        alloc8r.deallocate(blk2);
    }

    SECTION("Use freelist") {
        Block blk = alloc8r.allocate(32);
        test_block(blk, 64);
        void* ptr1 = blk.ptr;

        alloc8r.deallocate(blk);
        blk = alloc8r.allocate(48);
        test_block(blk, 64);

        REQUIRE(ptr1 == blk.ptr);
        alloc8r.deallocate(blk);
    }

    SECTION("Big block") {
        Block blk = alloc8r.allocate(32);
        void* ptr = blk.ptr;
        test_block(blk, 64);
        alloc8r.deallocate(blk);

        Block big = alloc8r.allocate(128);
        void* ptr_b = big.ptr;
        test_block(big, 128);
        alloc8r.deallocate(big);

        Block small = alloc8r.allocate(48);
        REQUIRE(ptr_b != small.ptr);
        REQUIRE(ptr == small.ptr);
        alloc8r.deallocate(small);
    }

    SECTION("Fill freelist") {
        std::vector<Block> blks;

        for (int i = 0; i < 1025; ++i) {
            blks.push_back(alloc8r.allocate(64));
        }

        void* ptr = blks[1].ptr;
        for (int i = 0; i < 1025; ++i) {
            alloc8r.deallocate(blks.back());
            blks.pop_back();
        }

        Block test = alloc8r.allocate(32);
        REQUIRE(test.ptr == ptr);
        alloc8r.deallocate(test);
    }
}

TEST_CASE("Freelist comparison") {
    allocators::Freelist<allocators::Mallocator, 32, 64, 1024> fst, snd;
    REQUIRE_FALSE(fst != snd);
    REQUIRE(fst == snd);
}
