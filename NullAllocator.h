//
// Created by kejsty on 12.4.16.
//
#pragma once

#include "base.h"
#include <cassert>

namespace allocators {
struct NullAllocator {
    Block allocate(size_t) noexcept { return {nullptr, 0}; }

    void deallocate(Block& blk, size_t) noexcept { assert(blk.ptr == nullptr); }

    bool owns(const Block& blk) const {
        return blk.ptr == nullptr && blk.size == 0;
    }

    bool operator==(const NullAllocator&) const { return true; }
    bool operator!=(const NullAllocator& other) const {
        return !(*this == other);
    }
};
}
