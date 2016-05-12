#pragma once

#include "Block.h"
#include "type_classes.h"

namespace allocators {

template <typename Primary, typename Fallback> struct FallbackAllocator : Eq {

    Block allocate(size_t size) {
        Block ptr = _primary.allocate(size);
        return ptr ? ptr : _fallback.allocate(size);
    }

    void deallocate(Block& blk) noexcept {
        if (_primary.owns(blk))
            _primary.deallocate(blk);
        else
            _fallback.deallocate(blk);
    }

    bool owns(const Block& blk) const noexcept {
        return _primary.owns(blk) || _fallback.owns(blk);
    }

    bool operator==(const FallbackAllocator& b) const noexcept {
        return _primary == b._primary && _fallback == b._fallback;
    }

private:
    Primary _primary;
    Fallback _fallback;
};
}
