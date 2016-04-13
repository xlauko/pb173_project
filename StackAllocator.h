//
// Created by kejsty on 12.4.16.
//

#ifndef PB173_PROJECT_STACKALLOCATOR_H
#define PB173_PROJECT_STACKALLOCATOR_H
#pragma once

#include "base.h"
#include <cassert>

namespace allocators {
    template <size_t size> struct StackAllocator {
        Block allocate(size_t n) noexcept
        {
            Block blk;

            if (_top + n > _data + size || n == 0)
                return blk;

            blk.ptr = _top;
            blk.size = n;
            _top = _top + n;
            return blk;
        }

        void deallocate(Block& blk)
        {
            assert(isLast(blk));
            _top = reinterpret_cast<char*>(blk.ptr);
            blk.reset();
        }

        bool owns(const Block& blk)
        {
            return blk.ptr >= _data && blk.ptr <= _data + size;
        }

        bool operator==(const StackAllocator&) { return false; }
        bool operator!=(const StackAllocator&) { return false; }

    private:
        char _data[size];
        char* _top = _data;

        bool isLast(Block& blk)
        {
            return reinterpret_cast<char*>(blk.ptr) + blk.size == _top;
        }
    };
}

#endif // PB173_PROJECT_STACKALLOCATOR_H
