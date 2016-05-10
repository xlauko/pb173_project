#include "Segregator.h"
#include "catch.hpp"
#include "test_common.h"

#include "Mallocator.h"
#include "Freelist.h"
#include "NullAllocator.h"

using namespace allocators;

TEST_CASE("Segregator allocate") {
    Segregator<128, Mallocator, Freelist<Mallocator, 128, 256, 1024>> alloc8r;

    SECTION("Zero allocation") {
        Block blk = alloc8r.allocate(0);
        test_block(blk, 0);
        alloc8r.deallocate(blk);
        test_null(blk);
    }

    SECTION("Simple") {
        Block blk1 = alloc8r.allocate(64);
        Block blk2 = alloc8r.allocate(142);
        test_block(blk1, 64);
        test_block(blk2, 256);
        REQUIRE(blk1 != blk2);

        alloc8r.deallocate(blk1);
        alloc8r.deallocate(blk2);
        test_null(blk1);
    }
}

TEST_CASE("Segregator comparison") {
    Segregator<512, Mallocator, Freelist<Mallocator, 512, 1024, 1024>> fst, snd;
    REQUIRE_FALSE(fst != snd);
    REQUIRE(fst == snd);
    Segregator<512, NullAllocator, NullAllocator> n1, n2;
    REQUIRE(n1 == n2);
    REQUIRE_FALSE(n1 != n2);
}
