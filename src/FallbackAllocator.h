#pragma once

#include "Block.h"
#include "type_classes.h"

namespace allocators {

template <typename Primary, typename Fallback>
struct FallbackAllocator : Eq {
    Block allocate(size_t size) {
        Block ptr = primary.allocate(size);
        return ptr ? ptr : fallback.allocate(size);
    }

    void deallocate(Block& blk) noexcept {
        if (primary.owns(blk))
            primary.deallocate(blk);
        else
            fallback.deallocate(blk);
    }

    bool owns(const Block& blk) const {
        return primary.owns(blk) || fallback.owns(blk);
    }

    bool operator==(const FallbackAllocator& b) const {
        return primary == b.primary && fallback == b.fallback;
    }

private:
    Primary primary;
    Fallback fallback;
};
}