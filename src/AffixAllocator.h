#pragma once

#include "Block.h"
#include <cassert>
#include <cstddef>

namespace allocators {
template <class T> struct size_trait { const static size_t value = sizeof(T); };
template <> struct size_trait<void> { const static size_t value = 0; };

template <typename Allocator, typename Prefix, typename Suffix = void>
struct AffixAllocator : Eq {

    Block allocate(size_t n) noexcept {
        if (n == 0) {
            return Block{};
        }
        Block block = reduce(_allocator.allocate(alloc_size(n)));
        block.size = block.ptr ? n : 0;
        return block;
    }

    void deallocate(Block& blk) noexcept {
        Block block = gain(blk);
        _allocator.deallocate(block);
        blk.reset();
    }

    bool owns(const Block& blk) const noexcept {
        return blk && _allocator.owns(gain(blk));
    }

    bool operator==(const AffixAllocator& other) const noexcept {
        return other._allocator == _allocator;
    }

    Prefix* prefix(const Block& blk) {
        // assert(owns(blk));
        return blk ? reinterpret_cast<Prefix*>(
                static_cast<char*>(blk.ptr) - data_dist)
                   : nullptr;
    }

    Suffix* suffix(const Block& blk) {
        // assert(owns(blk));
        return blk ? reinterpret_cast<Suffix*>(static_cast<char*>(blk.ptr) +
                                               blk.size +( alignof(Suffix) - ((blk.size + data_align) % alignof(Suffix))))
                   : nullptr;
    }

private:
    Allocator _allocator;

    size_t alloc_size(size_t n) const { return n + addition(n); }

    const static size_t prefix_size = size_trait<Prefix>::value;

    const static size_t suffix_size = size_trait<Suffix>::value;

    const static size_t data_align = alignof(std::max_align_t);

    //space we need to add behind prefix to have aligned data
    const static size_t prefix_align_space = (data_align - ((prefix_size + alignof(Prefix)) % data_align));

    //real distance between allocated memory and users data
    const static size_t data_dist =
            prefix_size + prefix_align_space;

    //space we need to add behind data to have aligned suffix
    size_t suffix_align_size(size_t data_size) const {
        return ( alignof(Suffix) - ((data_size + data_align) % alignof(Suffix)));
    }

    //real addition needed
    size_t addition(size_t n) const {
        return prefix_size +  prefix_align_space /* space between prefix and data*/+
               suffix_size + suffix_align_size(n); /* space between data and suffix */;
    }

    Block gain(const Block& blk) const {
        Block block;
        if (!blk)
            return block;
        block.ptr = static_cast<char*>(blk.ptr) - data_dist;
        block.size = alloc_size(blk.size);
        return block;
    }

    Block reduce(const Block& blk) const {
        Block block;
        if (!blk)
            return block;
        block.ptr = static_cast<char*>(blk.ptr) + data_dist;
        block.size = blk.size;
        return block;
    }
};
}
