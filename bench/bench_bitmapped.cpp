#include "../src/BitmappedBlock.h"
#include "../src/Mallocator.h"
#include "bench_common.h"
#include <array>
#include <benchmark/benchmark.h>

using namespace allocators;

template <size_t size> void bench_bitmapped_alloc(benchmark::State& s) {
    BitmappedBlock<Mallocator, size> alloc(batch_size);
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

template <size_t size> void bench_bitmapped_dealloc(benchmark::State& s) {
    BitmappedBlock<Mallocator, size> alloc(batch_size);
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

BENCHMARK_TEMPLATE(bench_bitmapped_alloc, 8);
BENCHMARK_TEMPLATE(bench_bitmapped_alloc, 64);
BENCHMARK_TEMPLATE(bench_bitmapped_alloc, 512);
BENCHMARK_TEMPLATE(bench_bitmapped_alloc, 4*1024);
BENCHMARK_TEMPLATE(bench_bitmapped_alloc, 32*1024);
BENCHMARK_TEMPLATE(bench_bitmapped_alloc, 256*1024);
BENCHMARK_TEMPLATE(bench_bitmapped_alloc, 1024*1024);

BENCHMARK_TEMPLATE(bench_bitmapped_dealloc, 8);
BENCHMARK_TEMPLATE(bench_bitmapped_dealloc, 64);
BENCHMARK_TEMPLATE(bench_bitmapped_dealloc, 512);
BENCHMARK_TEMPLATE(bench_bitmapped_dealloc, 4*1024);
BENCHMARK_TEMPLATE(bench_bitmapped_dealloc, 32*1024);
BENCHMARK_TEMPLATE(bench_bitmapped_dealloc, 256*1024);
BENCHMARK_TEMPLATE(bench_bitmapped_dealloc, 1024*1024);
