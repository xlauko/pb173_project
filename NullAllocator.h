//
// Created by kejsty on 12.4.16.
//

#ifndef PB173_PROJECT_NULLALLOCATOR_H
#define PB173_PROJECT_NULLALLOCATOR_H
#pragma once

#include <cassert>
#include "base.h"

namespace allocators {
    struct NullAllocator {
        Block allocate(size_t) noexcept {
            return { nullptr, 0 };
        }

        void deallocate(Block& blk, size_t) noexcept { assert(blk.ptr == nullptr); }

        bool operator==(const NullAllocator &) const {
            return true;
        }

        bool operator!=(const NullAllocator &other) const {
            return !(*this == other);
        }

        bool owns(Block &blk) {
            return blk.ptr == nullptr && blk.size == 0;
        }

    };
}

#endif //PB173_PROJECT_NULLALLOCATOR_H
