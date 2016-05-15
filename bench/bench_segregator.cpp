#include <benchmark/benchmark.h>
#include "bench_common.h"
#include "../src/Segregator.h"
#include "../src/Freelist.h"
#include "../src/Mallocator.h"
#include <random>

using namespace allocators;

static void bench_segregator_alloc(benchmark::State& s) {
    Segregator<128, Mallocator, Freelist<Mallocator, 128, 256, 1024>> alloc;
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

static void bench_segregator_dealloc(benchmark::State& s) {
    Segregator<128, Mallocator, Freelist<Mallocator, 128, 256, 1024>> alloc;
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

static void bench_segregator_randomValues(benchmark::State& s) {
    Segregator<128, Mallocator, Freelist<Mallocator, 128, 256, 1024>> alloc;
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

BENCHMARK(bench_segregator_alloc)->Range(8, 1024 * 1024);
BENCHMARK(bench_segregator_dealloc)->Range(8, 1024 * 1024);
BENCHMARK(bench_segregator_randomValues)->Range(8, 1024 * 1024);
