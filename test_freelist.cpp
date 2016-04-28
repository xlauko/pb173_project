#include "Freelist.h"
#include "Mallocator.h"
#include "catch.hpp"
#include "common.h"

TEST_CASE("Freelist allocator allocation") {
    allocators::Freelist<allocators::Mallocator, 32, 64, 1024> alloc8r;

    SECTION("simple") {
        Block blk1 = alloc8r.allocate(32);
        Block blk2 = alloc8r.allocate(48);
        test_block(blk1, 64);
        test_block(blk2, 64);

        REQUIRE(blk1 != blk2);
    }
}

TEST_CASE("Freelist allocator deallocate") {}

TEST_CASE("Freelist allocator comparison") {
    allocators::Freelist<allocators::Mallocator, 32, 64, 1024> fst, snd;
    REQUIRE_FALSE(fst != snd);
    REQUIRE(fst == snd);
}
