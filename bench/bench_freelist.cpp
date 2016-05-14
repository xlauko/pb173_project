#include <benchmark/benchmark.h>
#include "../src/Freelist.h"
#include "../src/Mallocator.h"
#include "../src/Block.h"

using namespace allocators;

static void bench_freelist(benchmark::State& s) {
    Freelist<Mallocator, 8, 4 * 1024> alloc;

    while (s.KeepRunning()) {
        Block blk = alloc.allocate(s.range_x());
        alloc.deallocate(blk);
    }
}
BENCHMARK(bench_freelist)->Range(8, 1024 * 1024);
