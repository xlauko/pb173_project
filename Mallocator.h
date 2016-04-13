//
// Created by kejsty on 12.4.16.
//

#ifndef PB173_PROJECT_MALLOCATOR_H
#define PB173_PROJECT_MALLOCATOR_H
#pragma once

#include <set>
#include <assert.h>
#include "base.h"

namespace allocators {
    struct Mallocator {
        Mallocator() { }

        Mallocator(const Mallocator &) = delete;
        Mallocator(Mallocator &&) = delete;
        Mallocator &operator=(const Mallocator &) = delete;
        Mallocator &operator=(Mallocator &&) = delete;

        Block allocate(size_t n) noexcept {
            Block blk;
            void *ptr = malloc(n);
            if (ptr) {
                blk._ptr = ptr;
                blk._size = n;
                _allocated.insert(ptr);
            }
            return blk;
        }

        void deallocate(Block &blk) noexcept {
            auto ptr = _allocated.find(blk._ptr);
            assert(ptr != _allocated.end());
            _allocated.erase(ptr);
            free(blk._ptr);
            blk.reset();
        }

        bool owns(Block &blk) {
            return (_allocated.find(blk._ptr) != _allocated.end());
        }

    private:
        std::set<void *> _allocated;
    };
}
#endif //PB173_PROJECT_MALLOCATOR_H
