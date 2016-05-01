#pragma once

#include "base.h"
#include "type_classes.h"

namespace {
template <size_t threshold, typename SmallAllocator, typename LargeAllocator>
struct Segregator {
    Block allocate(size_t) {}

    void deallocate(Block& blk) noexcept {}

    bool owns(const Block& blk) const noexcept {}
    bool operator==(const Segregator& b) const {}
};
}
