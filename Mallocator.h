//
// Created by kejsty on 12.4.16.
//

#ifndef PB173_PROJECT_MALLOCATOR_H
#define PB173_PROJECT_MALLOCATOR_H
#pragma once

#include <set>
#include <cassert>
#include "base.h"

namespace allocators {
    struct Mallocator {
        Block allocate(size_t n) {
            Block blk;
            void *ptr = ::operator new(n);
            if (ptr) {
                blk.ptr = ptr;
                blk.size = n;
                _allocated.insert(ptr);
            }
            return blk;
        }

        void deallocate(Block &blk) noexcept {
            auto ptr = _allocated.find(blk.ptr);
            assert(ptr != _allocated.end());
            _allocated.erase(ptr);
            ::operator delete(blk.ptr);
            blk.reset();
        }

        bool owns(Block &blk) {
            return (_allocated.find(blk.ptr) != _allocated.end());
        }

    private:
        std::set<void *> _allocated;
    };
}
#endif //PB173_PROJECT_MALLOCATOR_H
