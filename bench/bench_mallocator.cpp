#include <benchmark/benchmark.h>
#include "bench_common.h"
#include "../src/Mallocator.h"
#include <random>
#include <array>

using namespace allocators;

static void bench_mallocator_alloc(benchmark::State& s) {
    Mallocator alloc;
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

static void bench_mallocator_dealloc(benchmark::State& s) {
    Mallocator alloc;
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

BENCHMARK(bench_mallocator_alloc)->Range(8, 1024 * 1024);
BENCHMARK(bench_mallocator_dealloc)->Range(8, 1024 * 1024);
