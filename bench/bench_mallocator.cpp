#include <benchmark/benchmark.h>
#include "../src/Mallocator.h"
#include "../src/Block.h"

using namespace allocators;

static void bench_mallocator(benchmark::State& s) {
    Mallocator alloc;

    while (s.KeepRunning()) {
        Block blk = alloc.allocate(s.range_x());
        alloc.deallocate(blk);
    }
}
BENCHMARK(bench_mallocator)->Range(8, 1024 * 1024);
