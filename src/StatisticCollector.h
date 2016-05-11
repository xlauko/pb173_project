#pragma once

#include <iostream>
#include "Block.h"

namespace allocators {
typedef enum {
    numOwns = 1,
    numAllocate = 2,
    numAllocateOk = 4,
    numDeallocate = 8,
    numAll = 10, // means allocations + deallocations
    bytesAllocated = 16,
    bytesHighTide = 32
} Options;

template <typename Allocator, int Option = Options::numAllocate>
struct StatisticCollector : Eq {

    StatisticCollector() : _result(0) {}

    StatisticCollector(const StatisticCollector&) = delete;
    StatisticCollector(StatisticCollector&&) = default;
    StatisticCollector& operator=(const StatisticCollector&) = delete;
    StatisticCollector& operator=(StatisticCollector&&) = default;

    bool operator==(const StatisticCollector& other) {
        return this->parent == other.parent;
    }

    Block allocate(size_t n) noexcept {
        inc(Options::numAllocate, _result);
        Block block = parent.allocate(n);
        if (block)
            inc(Options::numAllocateOk, _result);
        add(Options::bytesAllocated, _result, static_cast<int>(block.size));
        max(Options::bytesHighTide, _result, n);
        return block;
    }

    void deallocate(Block& blk) noexcept {
        add(Options::bytesAllocated, _result, static_cast<int>(blk.size) * -1);
        inc(Options::numDeallocate, _result);
        parent.deallocate(blk);
    }

    bool owns(const Block& block) const noexcept {
        inc(Options::numOwns, _result);
        return parent.owns(block);
    }

    size_t result() const noexcept { return _result; }

private:
    Allocator parent;
    size_t _result;
    void inc(Options option, const size_t& count) const {
        if (option & Option) {
            const_cast<size_t&>(count) = count + 1;
        }
    }

    void add(Options option, size_t& bytes, int value) const {
        if (option & Option) {
            bytes += value;
        }
    }

    void max(Options option, size_t& bites, size_t value) const {
        if (option & Option) {
            bites = bites > value ? bites : value;
        }
    }
};
}