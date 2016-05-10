#pragma once

#include <cassert>
#include "base.h"
#include "dynamic_size.h"
#include "type_classes.h"

namespace {
template <typename Allocator, size_t min, size_t max, size_t step>
struct Bucketizer : Eq {
    static_assert(min < max, "Minimal size must be smaller than maximal.");
    static_assert((max - min + 1) % step == 0, "Step size is incorrect.");

    static constexpr size_t number_of_buckets = ((max - min + 1) / step);

    Bucketizer() {
        for (size_t i = 0; i < number_of_buckets; ++i) {
            _buckets[i].set_bounds(i * step + min, (i + 1) * step + min - 1);
        }
    }

    Bucketizer(const Bucketizer&) = delete;
    Bucketizer(Bucketizer&&) = default;
    Bucketizer& operator=(const Bucketizer&) = delete;
    Bucketizer& operator=(Bucketizer&&) = default;

    Block allocate(size_t n) {
        if (bucket_index(n) < number_of_buckets)
            return get_bucket_allocator(n)->allocate(n);
        return {};
    }

    void deallocate(Block& blk) noexcept {
        if (!blk)
            return;
        assert(owns(blk));
        get_bucket_allocator(blk.size)->deallocate(blk);
    }

    bool owns(const Block& blk) const noexcept {
        return blk && bucket_index(blk.size) < number_of_buckets &&
               _buckets[bucket_index(blk.size)].owns(blk);
    }

    bool operator==(const Bucketizer& b) const {
        return _buckets == b._buckets;
    }

    Allocator _buckets[number_of_buckets];

private:
    size_t bucket_index(size_t n) const noexcept { return (n / step) - 1; }

    Allocator* get_bucket_allocator(size_t n) noexcept {
        assert(min <= n && n < max);
        return &_buckets[bucket_index(n)];
    }
};

template <class Allocator, size_t min, size_t max, size_t step>
const size_t Bucketizer<Allocator, min, max, step>::number_of_buckets;
}
