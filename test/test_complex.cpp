#include "Allocators.h"
#include "test_common.h"

using namespace allocators;

TEST_CASE("Composble segregator") {
    using Allocator =
            Segregator<1024, Segregator<128, Freelist<Mallocator, 0, 128>,
                                        Freelist<Mallocator, 128, 1024>>,
                       Mallocator>;
    Allocator allocator;
    SECTION("simple") {
        Block blk1 = allocator.allocate(128);
        Block blk2 = allocator.allocate(1024);
        test_block(blk1, 128);
        test_block(blk2, 1024);

        REQUIRE(blk1 != blk2);

        allocator.deallocate(blk1);
        allocator.deallocate(blk2);

        test_null(blk1);
        test_null(blk2);
    }
}

TEST_CASE("Composble fallback") {
    using Allocator = FallbackAllocator<
            StackAllocator<256>,
            FallbackAllocator<StackAllocator<256>, NullAllocator>>;

    Allocator allocator;

    SECTION("simple") {
        Block blk1 = allocator.allocate(128);
        Block blk2 = allocator.allocate(256);
        Block blk3 = allocator.allocate(64);
        Block blk4 = allocator.allocate(128);

        test_block(blk1, 128);
        test_block(blk2, 256);
        test_block(blk3, 64);
        test_null(blk4);
    }
}
