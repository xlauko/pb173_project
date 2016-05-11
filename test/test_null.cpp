#include "NullAllocator.h"
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

TEST_CASE("Null allocator owns") {
    NullAllocator allocator;
    Block block = allocator.allocate(10);
    REQUIRE(allocator.owns(block));
}
