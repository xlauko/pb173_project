#include "Bucketizer.h"
#include "catch.hpp"
#include "common.h"

#include "Mallocator.h"
#include "Freelist.h"

using namespace allocators;

TEST_CASE("Bucketizer allocate") {
    Bucketizer<Freelist<Mallocator, 33, 128, 1024>, 33, 128, 32> alloc8r;

    SECTION("Buckets") {
        REQUIRE(alloc8r.number_of_buckets == 3);
        REQUIRE(alloc8r._buckets[0].min_size() == 33);
        REQUIRE(alloc8r._buckets[0].max_size() == 128);

        REQUIRE(alloc8r._buckets[1].min_size() == 33); // TODO
        REQUIRE(alloc8r._buckets[0].max_size() == 128);

        REQUIRE(alloc8r._buckets[2].min_size() == 33); // TODO
        REQUIRE(alloc8r._buckets[0].max_size() == 128);
    }

    SECTION("Simple") {
        Block blk1 = alloc8r.allocate(64);
        Block blk2 = alloc8r.allocate(96);
        test_block(blk1, 128);
        test_block(blk2, 128);
        REQUIRE(blk1 != blk2);

        // alloc8r.deallocate(blk1);
        // alloc8r.deallocate(blk2);
    }
}

TEST_CASE("Bucketizer comparison") {}
