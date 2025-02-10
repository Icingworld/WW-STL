#include <benchmark/benchmark.h>
#include <array>
#include <ww_array.h>

// operator[]

static void BM_ww_array_access(benchmark::State & state) {
    wwstl::array<int, 1024> arr;
    for (auto _ : state) {
        benchmark::DoNotOptimize(arr[512] = 1);
    }
}
BENCHMARK(BM_ww_array_access);

static void BM_std_array_access(benchmark::State & state) {
    std::array<int, 1024> arr;
    for (auto _ : state) {
        benchmark::DoNotOptimize(arr[512] = 1);
    }
}
BENCHMARK(BM_std_array_access);

// at

static void BM_ww_array_at(benchmark::State & state) {
    wwstl::array<int, 1024> arr;
    for (auto _ : state) {
        benchmark::DoNotOptimize(arr.at(512) = 1);
    }
}
BENCHMARK(BM_ww_array_at);

static void BM_std_array_at(benchmark::State & state) {
    std::array<int, 1024> arr;
    for (auto _ : state) {
        benchmark::DoNotOptimize(arr.at(512) = 1);
    }
}
BENCHMARK(BM_std_array_at);

// iterator

static void BM_ww_array_iterator(benchmark::State & state) {
    wwstl::array<int, 1024> arr;
    for (auto _ : state) {
        for (auto it = arr.begin(); it != arr.end(); ++it) {
            benchmark::DoNotOptimize(*it = 1);
        }
    }
}
BENCHMARK(BM_ww_array_iterator);

static void BM_std_array_iterator(benchmark::State & state) {
    std::array<int, 1024> arr;
    for (auto _ : state) {
        for (auto it = arr.begin(); it != arr.end(); ++it) {
            benchmark::DoNotOptimize(*it = 1);
        }
    }
}
BENCHMARK(BM_std_array_iterator);

// fill

static void BM_ww_array_fill(benchmark::State & state) {
    wwstl::array<int, 1024> arr;
    for (auto _ : state) {
        arr.fill(1);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_array_fill);

static void BM_std_array_fill(benchmark::State & state) {
    std::array<int, 1024> arr;
    for (auto _ : state) {
        arr.fill(1);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_array_fill);

BENCHMARK_MAIN();
