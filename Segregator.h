#pragma once

#include "base.h"
#include "type_classes.h"

namespace {
template <size_t threshold, typename SmallAllocator, typename LargeAllocator>
struct Segregator : Eq {
    Block allocate(size_t n) {
        if (n <= threshold)
            return _small.allocate(n);
        else
            return _large.allocate(n);
    }

    void deallocate(Block& blk) noexcept {
        if (blk.size <= threshold)
            return _small.deallocate(blk);
        else
            return _large.deallocate(blk);
    }

    bool owns(const Block& blk) const noexcept {
        if (blk.size <= threshold)
            return _small.owns(blk);
        else
            return _large.owns(blk);
    }
    bool operator==(const Segregator& b) const {
        return _small == b._small && _large == b._large;
    }

private:
    SmallAllocator _small;
    LargeAllocator _large;
};
}
