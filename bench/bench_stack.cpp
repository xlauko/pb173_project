#include <benchmark/benchmark.h>
#include "bench_common.h"
#include "../src/StackAllocator.h"
using namespace allocators;

static void bench_stack_alloc(benchmark::State& s) {
    StackAllocator<1024 * 1024 * batch_size, 8> alloc;
    std::array<Block, batch_size> blocks;

    while (s.KeepRunning()) {
        for (auto& blk : blocks) {
            blk = alloc.allocate(s.range_x());
        }

        s.PauseTiming();
        for (auto& blk : blocks) {
            alloc.deallocate(blk);
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

        for (auto& blk : blocks) {
            alloc.deallocate(blk);
        }
    }
}

BENCHMARK(bench_stack_alloc)->Range(8, 1024 * 1024);
BENCHMARK(bench_stack_dealloc)->Range(8, 1024 * 1024);
