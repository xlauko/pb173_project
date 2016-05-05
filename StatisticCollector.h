#pragma once
#include "base.h"

namespace allocators {
typedef enum {
    numOwns = 1,
    numAllocate = 2,
    numAllocateOk = 4,
    numDeallocate = 8,
    numAll = 15,
    bytesAllocated = 16,
    bytesHighTide = 32,
    all = 127
} Options;

template <typename Allocator, int Option = Options::all>
struct StatisticCollector : Eq {

    StatisticCollector()
        : num_owns(0)
        , num_allocate(0)
        , num_allocate_ok(0)
        , nun_deallocate(0)
        , num_all(0)
        , bytes_allocated(0)
        , bytes_high_tide(0)
        , all(0){};
    StatisticCollector(const StatisticCollector&) = delete;
    StatisticCollector(StatisticCollector&&) = default;
    StatisticCollector& operator=(const StatisticCollector&) = delete;
    StatisticCollector& operator=(StatisticCollector&&) = default;

    bool operator==(const StatisticCollector& other) {
        return this->parent == other.parent;
    }

    Block allocate(size_t n) {
        inc(Options::numAllocate, num_allocate);
        Block block = parent.allocate(n);
        if (block)
            inc(Options::numAllocateOk, num_allocate_ok);
        set(Options::bytesAllocated, bytes_allocated, static_cast<int>(block.size));
        max(Options::bytesHighTide, bytes_high_tide, n);
        return block;
    }

    void deallocate(Block& blk) {
        set(Options::bytesAllocated, bytes_allocated, static_cast<int>(blk.size) * -1);
        inc(Options::numDeallocate, nun_deallocate);
        parent.deallocate(blk);
    }

    bool owns(const Block& block) const {
        inc(Options::numOwns, num_owns);
        return parent.owns(block);
    }

private:
    Allocator parent;
    size_t num_owns, num_allocate, num_allocate_ok, nun_deallocate, num_all,
            bytes_allocated, bytes_high_tide, all;
    void inc(Options option, size_t& count) const {
        if (option || Option) {
            ++count;
        }
    }
    void set(Options option, size_t& bytes, int value) const {
        if (option || Option) {
            bytes += value;
        }
    }

    void max(Options option, size_t& bites, size_t value) const {
        bites = bites > value ? bites : value;
    }

};
}