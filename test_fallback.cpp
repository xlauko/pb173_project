#include "FallbackAllocator.h"
#include "StackAllocator.h"
#include "Mallocator.h"
#include "catch.hpp"

TEST_CASE("Fallback allocator Test") {
    allocators::FallbackAllocator<allocators::StackAllocator<256>, allocators::Mallocator> allocator;
    Block block1 = allocator.allocate(250);
    Block block2 = allocator.allocate(250);
    REQUIRE(block1.size == 250);
    REQUIRE(block2.size == 250);
    REQUIRE(allocator.owns(block1));
    REQUIRE(allocator.owns(block2));
    allocator.deallocate(block2);
    allocator.deallocate(block1);
}
