#include "Mallocator.h"
#include "catch.hpp"

#include "common.h"

TEST_CASE("Mallocator Test") {
    allocators::Mallocator allocator;
    Block block1 = allocator.allocate(128);
    Block block2 = allocator.allocate(256);
    test_block(block1, 128);
    test_block(block2, 256);
    REQUIRE(block1 != block2);
    REQUIRE(allocator.owns(block2));
    REQUIRE(allocator.owns(block1));
    allocator.deallocate(block1);
    allocator.deallocate(block2);
    test_null(block1);
    test_null(block2);
}
