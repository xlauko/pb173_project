#pragma once

#include "Block.h"
#include "catch.hpp"

inline void test_block(allocators::Block& blk, int n) {
    REQUIRE(blk.ptr != nullptr);
    REQUIRE(blk.size == n);
}

inline void test_null(allocators::Block& blk) {
    REQUIRE(blk.ptr == nullptr);
    REQUIRE(blk.size == 0);
}


