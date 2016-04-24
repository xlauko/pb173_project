#pragma once

#include "base.h"
#include <cassert>
#include <set>

namespace allocators {
struct Mallocator {
    Block allocate(size_t n) {
        Block blk;
        void* ptr = ::operator new(n);
        if (ptr) {
            blk.ptr = ptr;
            blk.size = n;
            _allocated.insert(ptr);
        }
        return blk;
    }

    void deallocate(Block& blk) noexcept {
        auto ptr = _allocated.find(blk.ptr);
        assert(ptr != _allocated.end());
        _allocated.erase(ptr);
        ::operator delete(blk.ptr);
        blk.reset();
    }

    bool owns(const Block& blk) const {
        return (_allocated.find(blk.ptr) != _allocated.end());
    }

    bool operator==(const Mallocator& other) const { return true; }
    bool operator!=(const Mallocator& other) const { return !(*this == other); }

private:
    std::set<void*> _allocated;
};
}
