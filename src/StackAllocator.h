#pragma once

#include "Block.h"
#include <cassert>

namespace allocators {
template <size_t size, size_t alignment = 8> struct StackAllocator : Eq {
    StackAllocator() {}

    StackAllocator(const StackAllocator&) = delete;
    StackAllocator(StackAllocator&&) = default;
    StackAllocator& operator=(const StackAllocator&) = delete;
    StackAllocator& operator=(StackAllocator&&) = default;

    Block allocate(size_t n, size_t align = alignment) noexcept {
        assert(align == alignment);
        Block blk;
        if (_top + n > _data + size || n == 0)
            return blk;

        size_t space = size - (_top - _data);
        void* data = static_cast<void*>(_top);
        if (std::align(alignment, n, data, space)) {
            blk.ptr = data;
            _top = static_cast<char*>(data);
            _top = _top + n;
            blk.size = n;
            return blk;
        }
        return blk;
    }

    void deallocate(Block& blk) noexcept {
        assert(isLast(blk));
        _top = reinterpret_cast<char*>(blk.ptr);
        blk.reset();
    }

    bool owns(const Block& blk) const noexcept {
        return blk.ptr >= _data && blk.ptr < _top;
    }

    bool operator==(const StackAllocator& b) const noexcept {
        return _top == b._top;
    }

private:
    char _data[size];
    char* _top = _data;

    bool isLast(Block& blk) {
        return reinterpret_cast<char*>(blk.ptr) + blk.size + alignment >= _top;
    }
};
}
