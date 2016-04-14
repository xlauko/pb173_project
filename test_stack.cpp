#include "StackAllocator.h"
#include "catch.hpp"

TEST_CASE("StackAllocatorTest") {
    allocators::StackAllocator<1024> allocator;
    Block block1 = allocator.allocate(128);
    Block block2 = allocator.allocate(256);
    REQUIRE(block1.ptr != nullptr);
    REQUIRE(block2.ptr != nullptr);
    REQUIRE(block1.size == 128);
    REQUIRE(block2.size == 256);
    REQUIRE(block1 != block2);
    REQUIRE(allocator.owns(block2));
    
    allocator.deallocate(block2);
    REQUIRE(block2.size == 0);
    REQUIRE(allocator.owns(block1));
    
    allocator.deallocate(block1);
    Block block3 = allocator.allocate(1024);
    REQUIRE(block3.size == 1024);
    Block block4 = allocator.allocate(100);
    REQUIRE(!block4);
}
