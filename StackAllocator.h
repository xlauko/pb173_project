#pragma once

#include "base.h"
#include "type_classes.h"
#include <cassert>

namespace allocators {
template <size_t size> struct StackAllocator : Eq {
    Block allocate(size_t n) noexcept {
        Block blk;

        if (_top + n > _data + size || n == 0)
            return blk;

        blk.ptr = _top;
        blk.size = n;
        _top = _top + n;
        return blk;
    }

    void deallocate(Block& blk) {
        assert(isLast(blk));
        _top = reinterpret_cast<char*>(blk.ptr);
        blk.reset();
    }

    bool owns(const Block& blk) const { return blk.ptr >= _data && blk.ptr < _top; }

    bool operator==(const StackAllocator& b) const { return _top == b._top; }

private:
    char _data[size];
    char* _top = _data;

    bool isLast(Block& blk) { return reinterpret_cast<char*>(blk.ptr) + blk.size == _top; }
};
}
