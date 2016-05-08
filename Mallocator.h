#pragma once

#include "base.h"
#include "dynamic_size.h"
#include "type_classes.h"
#include <cassert>
#include <set>

namespace allocators {
struct Mallocator : Eq, UndefinedBlkSize {
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

    bool owns(const Block& blk) const noexcept {
        return (_allocated.find(blk.ptr) != _allocated.end());
    }

    bool operator==(Mallocator const&) const { return true; }

private:
    std::set<void*> _allocated;
};
}
