#pragma once

#include "base.h"
namespace allocators {

template <typename Primary, typename Fallback> struct FallbackAllocator {
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

    bool operator==(const FallbackAllocator& other) const {
        return primary == other.primary && fallback == other.fallback;
    }

    bool operator!=(const FallbackAllocator& other) const {
        return !(*this == other);
    }

private:
    Primary primary;
    Fallback fallback;
};
}
