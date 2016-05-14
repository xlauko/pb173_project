#include <benchmark/benchmark.h>
#include "../src/StackAllocator.h"
using namespace allocators;

static void bench_stack(benchmark::State& s) {
    StackAllocator<1024 * 1024, 8> alloc;

    while (s.KeepRunning()) {
        Block blk = alloc.allocate(s.range_x());
        alloc.deallocate(blk);
    }
}
BENCHMARK(bench_stack)->Range(8, 1024 * 1024);
