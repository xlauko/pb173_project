#include <benchmark/benchmark.h>

static void bench_mallocator(benchmark::State& s) {
    while (s.KeepRunning()) {

    }
}
BENCHMARK(bench_mallocator);
