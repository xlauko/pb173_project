#pragma once

#include "Block.h"
#include <cassert>

namespace allocators {

struct NullAllocator : Eq {
    Block allocate(size_t) noexcept { return {nullptr, 0}; }

    void deallocate(Block& blk) noexcept { assert(blk); }

    bool owns(const Block& blk) const noexcept {
        return blk.ptr == nullptr && blk.size == 0;
    }

    bool operator==(NullAllocator const&) const noexcept { return true; }
};
}
