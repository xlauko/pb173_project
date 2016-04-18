//
// Created by kejsty on 12.4.16.
//

#ifndef PB173_PROJECT_FALLBACKALLOCATOR_H
#define PB173_PROJECT_FALLBACKALLOCATOR_H
#pragma once

#include "base.h"
namespace allocators {

    template<typename Primary, typename Fallback>
    struct FallbackAllocator {
        Block allocate(size_t size) {
            Block ptr = primary.allocate(size);
            return ptr ? ptr : fallback.allocate(size);
        }

        void deallocate(Block &blk) noexcept {
            if (primary.owns(blk))
                primary.deallocate(blk);
            else
                fallback.deallocate(blk);
        }

        bool owns(const Block& blk) const {
            return primary.owns(blk) || fallback.owns(blk);
        }

    private:
        Primary primary;
        Fallback fallback;
    };
}
#endif //PB173_PROJECT_FALLBACKALLOCATOR_H
