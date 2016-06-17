#pragma once

#include "Block.h"

namespace allocators {
struct Mallocator : Eq {
    Block allocate(size_t n) noexcept {
        return {::operator new(n, std::nothrow), n};
    }

    void deallocate(Block& blk) noexcept {
        ::operator delete(blk.ptr);
        blk.reset();
    }

    bool operator==(Mallocator const&) const noexcept { return true; }
};
} // namespace allocators
