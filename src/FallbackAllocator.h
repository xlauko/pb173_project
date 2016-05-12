#pragma once

#include "Block.h"
#include "type_classes.h"

namespace allocators {

template <typename Primary, typename Fallback> struct FallbackAllocator : Eq {

    FallbackAllocator() = default;
    FallbackAllocator(const FallbackAllocator&) = delete;
    FallbackAllocator(FallbackAllocator&&) = default;
    FallbackAllocator& operator=(const FallbackAllocator&) = delete;
    FallbackAllocator& operator=(FallbackAllocator&&) = default;

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

    bool owns(const Block& blk) const noexcept {
        return primary.owns(blk) || fallback.owns(blk);
    }

    bool operator==(const FallbackAllocator& b) const noexcept {
        return primary == b.primary && fallback == b.fallback;
    }

private:
    Primary primary;
    Fallback fallback;
};
}
