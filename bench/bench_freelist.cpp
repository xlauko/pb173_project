#include <benchmark/benchmark.h>
#include "bench_common.h"
#include "../src/Freelist.h"
#include "../src/Mallocator.h"
#include <random>
#include <array>

using namespace allocators;

static void bench_freelist_alloc(benchmark::State& s) {
    Freelist<Mallocator, 8, 4 * 1024> alloc;
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

static void bench_freelist_dealloc(benchmark::State& s) {
    Freelist<Mallocator, 8, 4 * 1024> alloc;
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

BENCHMARK(bench_freelist_alloc)->Range(8, 1024 * 1024);
BENCHMARK(bench_freelist_dealloc)->Range(8, 1024 * 1024);
