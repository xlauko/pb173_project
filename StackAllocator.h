//
// Created by kejsty on 12.4.16.
//

#ifndef PB173_PROJECT_STACKALLOCATOR_H
#define PB173_PROJECT_STACKALLOCATOR_H
#pragma once

#include <cstdio>
#include <queue>
#include <assert.h>
#include "base.h"


namespace allocators {
    template<size_t size>
    struct StackAllocator {

        StackAllocator() { }

        StackAllocator(const StackAllocator &) = delete;
        StackAllocator(StackAllocator &&) = delete;
        StackAllocator &operator=(const StackAllocator &) = delete;
        StackAllocator &operator=(StackAllocator &&) = delete;

        Block allocate(size_t n) noexcept {
            Block blk;

            if (_top + n > _data + size || n == 0)
                return blk;

            blk._ptr = _top;
            blk._size = n;
            _top = _top + n;
            return blk;
        }

        void deallocate(Block &blk) {
            assert(isLast(blk));
            _top = static_cast<char *>(blk._ptr);
            blk.reset();
        }

        bool owns(const Block &blk) {
            return blk._ptr >= _data && blk._ptr <= _data + size;
        }

        bool operator==(const StackAllocator &other) {
            return false;
        }

        bool operator!=(const StackAllocator &other) {
            return false;
        }

    private:
        char _data[size];
        char *_top = _data;

        bool isLast(Block &blk) {
            return static_cast<char*>(blk._ptr) + blk._size == static_cast<char *>(_top);
        }
    };
}

#endif //PB173_PROJECT_STACKALLOCATOR_H
