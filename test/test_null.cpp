#include "NullAllocator.h"
#include "test_common.h"
#include "catch.hpp"

using namespace allocators;

TEST_CASE("Null allocator allocation") {
    NullAllocator allocator;
    Block block = allocator.allocate(40);
    REQUIRE(!block);
    REQUIRE(block.ptr == nullptr);
    REQUIRE(block.size == 0);
}

TEST_CASE("Null allocator comparison") {
    NullAllocator fst, snd;
    REQUIRE(fst == snd);
    REQUIRE_FALSE(fst != snd);
}

TEST_CASE("Null deallocate") {
    NullAllocator alloc8r;
    Block blk = alloc8r.allocate(42);
    test_null(blk);
    alloc8r.deallocate(blk);
}

TEST_CASE("Null allocator owns") {
    NullAllocator allocator;
    Block block = allocator.allocate(10);
    REQUIRE(allocator.owns(block));
}
