#include <benchmark/benchmark.h>
#include "bench_common.h"
#include "../src/Freelist.h"
#include "../src/Mallocator.h"
#include <random>
#include <array>

using namespace allocators;

template < size_t size >
void bench_freelist_alloc(benchmark::State& s) {
    Freelist<Mallocator, size, size> alloc;
    std::array<Block, batch_size> blocks;

    while (s.KeepRunning()) {
        for (auto& blk : blocks) {
            blk = alloc.allocate(size);
        }

        s.PauseTiming();
        for (auto& blk : blocks) {
            alloc.deallocate(blk);
        }
        s.ResumeTiming();
    }
}

template < size_t size >
void bench_freelist_dealloc(benchmark::State& s) {
    Freelist<Mallocator, size, size > alloc;
    std::array<Block, batch_size> blocks;

    while (s.KeepRunning()) {
        s.PauseTiming();
        for (auto& blk : blocks) {
            blk = alloc.allocate(size);
        }
        s.ResumeTiming();

        for (auto& blk : blocks) {
            alloc.deallocate(blk);
        }
    }
}

BENCHMARK_TEMPLATE(bench_freelist_alloc, 8);
BENCHMARK_TEMPLATE(bench_freelist_alloc, 64);
BENCHMARK_TEMPLATE(bench_freelist_alloc, 512);
BENCHMARK_TEMPLATE(bench_freelist_alloc, 4*1024);
BENCHMARK_TEMPLATE(bench_freelist_alloc, 32*1024);
BENCHMARK_TEMPLATE(bench_freelist_alloc, 256*1024);
BENCHMARK_TEMPLATE(bench_freelist_alloc, 1024*1024);

BENCHMARK_TEMPLATE(bench_freelist_dealloc, 8);
BENCHMARK_TEMPLATE(bench_freelist_dealloc, 64);
BENCHMARK_TEMPLATE(bench_freelist_dealloc, 512);
BENCHMARK_TEMPLATE(bench_freelist_dealloc, 4*1024);
BENCHMARK_TEMPLATE(bench_freelist_dealloc, 32*1024);
BENCHMARK_TEMPLATE(bench_freelist_dealloc, 256*1024);
BENCHMARK_TEMPLATE(bench_freelist_dealloc, 1024*1024);
