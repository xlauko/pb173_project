//
// Created by kejsty on 12.4.16.
//

#ifndef PB173_PROJECT_NULLALLOCATOR_H
#define PB173_PROJECT_NULLALLOCATOR_H
#pragma once

#include <iostream>
#include "base.h"

namespace allocators {
    struct NullAllocator {
        NullAllocator() { }

        NullAllocator(const NullAllocator &) = default;

        NullAllocator &operator=(const NullAllocator &) = default;

        NullAllocator(NullAllocator &&) = default;

        NullAllocator &operator=(NullAllocator &&) = default;

        Block allocate(size_t n) noexcept {
            Block blk;
            return blk;
        }

        void deallocate(Block &blk, size_t n) noexcept { return; }

        bool operator==(const NullAllocator &) const {
            return true;
        }

        bool operator!=(const NullAllocator &) const {
            return false;
        }

        bool owns(Block &blk) {
            return blk._ptr == nullptr && blk._size == 0;
        }

    };
}

#endif //PB173_PROJECT_NULLALLOCATOR_H
