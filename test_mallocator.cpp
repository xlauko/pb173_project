#include "Mallocator.h"
#include "catch.hpp"

TEST_CASE("Mallocator Test") {
    allocators::Mallocator allocator;
    Block block1 = allocator.allocate(128);
    Block block2 = allocator.allocate(256);
    REQUIRE(block1.ptr != nullptr);
    REQUIRE(block2.ptr != nullptr);
    REQUIRE(block1.size == 128);
    REQUIRE(block2.size == 256);
    REQUIRE(block1 != block2);
    REQUIRE(allocator.owns(block2));
    REQUIRE(allocator.owns(block1));
    allocator.deallocate(block1);
    allocator.deallocate(block2);
    REQUIRE(block1.size == 0);
}
