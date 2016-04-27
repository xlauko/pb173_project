#pragma once

#include "base.h"
#include "type_classes.h"
#include <cassert>

namespace allocators {
struct NullAllocator : Eq {
    Block allocate(size_t) noexcept { return {nullptr, 0}; }

    void deallocate(Block& blk, size_t) noexcept { assert(blk.ptr == nullptr); }

    bool owns(const Block& blk) const { return blk.ptr == nullptr && blk.size == 0; }

    bool operator==(NullAllocator const&) const { return true; }
};
}
