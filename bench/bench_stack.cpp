#include "../src/StackAllocator.h"
#include "bench_common.h"
#include <benchmark/benchmark.h>
#include <iostream>
#include <memory>

using namespace allocators;

static void bench_stack_alloc(benchmark::State& s) {
    auto alloc = std::make_unique<StackAllocator<1024 * 32 * batch_size, 8>>();
    std::array<Block, batch_size> blocks;

    while (s.KeepRunning()) {
        for (auto& blk : blocks) {
            blk = alloc->allocate(s.range_x());
        }
        s.PauseTiming();
        for (size_t i = batch_size - 1; i != size_t(-1); --i) {
            alloc->deallocate(blocks[i]);
        }
        s.ResumeTiming();
    }
}

static void bench_stack_dealloc(benchmark::State& s) {
    auto alloc = std::make_unique<StackAllocator<1024 * 32 * batch_size, 8>>();
    std::array<Block, batch_size> blocks;

    while (s.KeepRunning()) {
        s.PauseTiming();
        for (auto& blk : blocks) {
            blk = alloc->allocate(s.range_x());
        }
        s.ResumeTiming();

        for (size_t i = batch_size - 1; i != size_t(-1); --i) {
            alloc->deallocate(blocks[i]);
        }
    }
}

BENCHMARK(bench_stack_alloc)->Range(8, 1024 * 32);
BENCHMARK(bench_stack_dealloc)->Range(8, 1024 * 32);
