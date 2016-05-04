#pragma once

#include "base.h"
#include "type_classes.h"

namespace {
template <typename Allocator, size_t min, size_t max, size_t step>
struct Bucketizer : Eq {
    static_assert(min < max, "Minimal size must be smaller than maximal.");
    static_assert((max - min + 1) % step == 0, "Step size is incorrect.");

    static constexpr size_t number_of_buckets = ((max - min + 1) / step);

    Block allocate(size_t n) {}

    void deallocate(Block& blk) noexcept {}

    bool owns(const Block& blk) const noexcept {}
    bool operator==(const Bucketizer& b) const {}

private:
    Allocator _buckets[number_of_buckets];
};
}
