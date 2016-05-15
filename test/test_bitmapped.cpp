#include "catch.hpp"
#include "test_common.h"
#include <BitmappedBlock.h>
#include <Mallocator.h>

using namespace allocators;

TEST_CASE("BitSet") {
    Mallocator alloc;
    BitSet<Mallocator> set(1024, alloc);

    for (std::size_t i = 0; i != 1024; ++i)
        REQUIRE_FALSE(set[i]);
    REQUIRE(set.find_first() >= set.size());

    set[0].flip();
    REQUIRE(set.find_first() == 0);
}

TEST_CASE("Bitmapped") {
    BitmappedBlock<Mallocator, 64> alloc(1024);

    auto a = alloc.allocate(64);
    test_block(a, 64);

    REQUIRE(alloc.owns(a));

    std::array<Block, 100> blocks;
    for (auto& block : blocks) {
        test_null(block);
        block = alloc.allocate(64);
        test_block(block, 64);

        REQUIRE(a.ptr != block.ptr);
    }

    test_block(a, 64);
    alloc.deallocate(a);
    test_null(a);

    REQUIRE_FALSE(alloc.owns(a));

    for (auto& block : blocks) {
        test_block(block, 64);
        alloc.deallocate(block);
    }
}
