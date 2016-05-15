#include <benchmark/benchmark.h>
#include "bench_common.h"
#include "../src/StackAllocator.h"
#include <iostream>

using namespace allocators;

static void bench_stack_alloc(benchmark::State& s) {
    StackAllocator<1024 * 1024 * batch_size, 8> alloc;
    std::array<Block, batch_size> blocks;

    while (s.KeepRunning()) {
        for (auto& blk : blocks) {
            blk = alloc.allocate(s.range_x());
        }
        s.PauseTiming();
        for (size_t i = batch_size; i-- > 0;) {
            alloc.deallocate(blocks[i]);
        }
        s.ResumeTiming();
    }
}

static void bench_stack_dealloc(benchmark::State& s) {
    StackAllocator<1024 * 1024 * batch_size, 8> alloc;
    std::array<Block, batch_size> blocks;

    while (s.KeepRunning()) {
        s.PauseTiming();
        for (auto& blk : blocks) {
            blk = alloc.allocate(s.range_x());
        }
        s.ResumeTiming();

        for (size_t i = batch_size; i-- > 0;) {
            alloc.deallocate(blocks[i]);
        }
    }
}

BENCHMARK(bench_stack_alloc)->Range(8, 1024 * 1024);
BENCHMARK(bench_stack_dealloc)->Range(8, 1024 * 1024);
