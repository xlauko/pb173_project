#pragma once

#include "Block.h"
#include "dynamic_size.h"
#include "type_classes.h"
#include <cassert>

namespace allocators {

struct NullAllocator : Eq, UndefinedBlkSize {
    Block allocate(size_t) noexcept { return {nullptr, 0}; }

    void deallocate(Block& blk, size_t) noexcept { assert(blk); }

    bool owns(const Block& blk) const {
        return blk.ptr == nullptr && blk.size == 0;
    }

    bool operator==(NullAllocator const&) const { return true; }
};
}
