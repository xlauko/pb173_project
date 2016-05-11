#pragma once

#include "Block.h"
#include <cassert>

namespace allocators {
template <class T> struct size_trait { const static size_t value = sizeof(T); };
template <> struct size_trait<void> { const static size_t value = 0; };

template <typename Allocator, typename Prefix, typename Suffix = void>
struct AffixAllocator : Eq {

    AffixAllocator() = default;
    AffixAllocator(const AffixAllocator&) = delete;
    AffixAllocator(AffixAllocator&&) = default;
    AffixAllocator& operator=(const AffixAllocator&) = delete;
    AffixAllocator& operator=(AffixAllocator&&) = default;

    Block allocate(size_t n) noexcept {
        return n == 0 ? Block{} : reduce(allocator.allocate(alloc_size(n)));
    }

    void deallocate(Block& blk) noexcept {
        Block block = gain(blk);
        allocator.deallocate(block);
        blk.reset();
    }

    bool owns(const Block& blk) const noexcept {
        return blk && allocator.owns(gain(blk));
    }

    bool operator==(const AffixAllocator& other) const noexcept {
        return other.allocator == allocator;
    }

    Prefix* prefix(const Block& blk) {
        assert(owns(blk));
        return blk ? reinterpret_cast<Prefix*>(static_cast<char*>(blk.ptr) -
                                               prefix_size)
                   : nullptr;
    }

    Suffix* suffix(const Block& blk) {
        assert(owns(blk));
        return blk ? reinterpret_cast<Suffix*>(static_cast<char*>(blk.ptr) +
                                               blk.size)
                   : nullptr;
    }

private:
    Allocator allocator;

    size_t alloc_size(size_t n) { return prefix_size + suffix_size + n; }

    size_t prefix_size = size_trait<Prefix>::value;

    size_t suffix_size = size_trait<Suffix>::value;

    Block gain(const Block& blk) const {
        Block block;
        if (!blk)
            return block;
        block.ptr = static_cast<char*>(blk.ptr) - prefix_size;
        block.size = blk.size + prefix_size + suffix_size;
        return block;
    }

    Block reduce(const Block& blk) const {
        Block block;
        if (!blk)
            return block;
        block.ptr = static_cast<char*>(blk.ptr) + prefix_size;
        block.size = blk.size - prefix_size - suffix_size;
        return block;
    }
};
}
