#include <benchmark/benchmark.h>
#include "../src/Mallocator.h"
#include <random>

using namespace allocators;

static void bench_mallocator_alloc(benchmark::State& s) {
    Mallocator alloc;

    while (s.KeepRunning()) {
        Block blk = alloc.allocate(s.range_x());
        s.PauseTiming();
        alloc.deallocate(blk);
        s.ResumeTiming();
    }
}

static void bench_mallocator_dealloc(benchmark::State& s) {
    Mallocator alloc;

    while (s.KeepRunning()) {
        s.PauseTiming();
        Block blk = alloc.allocate(s.range_x());
        s.ResumeTiming();
        alloc.deallocate(blk);
    }
}

static void bench_mallocator_randomValues(benchmark::State& s) {
    Mallocator alloc;
    std::mt19937 gen;
    std::uniform_int_distribution<int> dist(8, s.range_x());
    std::vector<Block> allocations(1000);

    while (s.KeepRunning()) {
        for (int i = 0; i < 1000; ++i) {
            if (allocations[i].ptr == nullptr) {
                int toAllocate = dist(gen);
                allocations[i] = alloc.allocate(toAllocate);
            } else {
                alloc.deallocate(allocations[i]);
            }
        }
        for (int i = 0; i < 1000; ++i) {
            if (allocations[i].ptr != nullptr) {
                alloc.deallocate(allocations[i]);
            }
        }
    }
}

BENCHMARK(bench_mallocator_alloc)->Range(8, 1024 * 1024);
BENCHMARK(bench_mallocator_dealloc)->Range(8, 1024 * 1024);
BENCHMARK(bench_mallocator_randomValues)->Range(8, 1024 * 1024);
