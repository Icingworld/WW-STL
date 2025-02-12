#include <benchmark/benchmark.h>
#include <unordered_map>
#include <ww_unordered_map.h>

// iterator

static void BM_ww_unordered_multimap_iterator(benchmark::State & state)
{
    wwstl::unordered_multimap<int, int> map = {
        {0, 0}, {1, 1}, {2, 2}
    };
    for (auto _ : state) {
        for (auto it = map.begin(); it != map.end(); ++it) {
            benchmark::DoNotOptimize(it->second);
        }
    }
}
BENCHMARK(BM_ww_unordered_multimap_iterator);

static void BM_std_unordered_multimap_iterator(benchmark::State & state)
{
    std::unordered_multimap<int, int> map = {
        {0, 0}, {1, 1}, {2, 2}
    };
    for (auto _ : state) {
        for (auto it = map.begin(); it != map.end(); ++it) {
            benchmark::DoNotOptimize(it->second);
        }
    }
}
BENCHMARK(BM_std_unordered_multimap_iterator);

// clear

static void BM_ww_unordered_multimap_clear(benchmark::State & state)
{
    for (auto _ : state) {
        wwstl::unordered_multimap<int, int> map = {
            {0, 0}, {1, 1}, {2, 2}
        };
        map.clear();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_unordered_multimap_clear);

static void BM_std_unordered_multimap_clear(benchmark::State & state)
{
    for (auto _ : state) {
        std::unordered_multimap<int, int> map = {
            {0, 0}, {1, 1}, {2, 2}
        };
        map.clear();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_unordered_multimap_clear);

// insert

static void BM_ww_unordered_multimap_insert(benchmark::State & state)
{
    for (auto _ : state) {
        wwstl::unordered_multimap<int, int> map = {
            {0, 0}, {1, 1}, {2, 2}
        };
        map.insert({1, 3});
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_unordered_multimap_insert);

static void BM_std_unordered_multimap_insert(benchmark::State & state)
{
    for (auto _ : state) {
        std::unordered_multimap<int, int> map = {
            {0, 0}, {1, 1}, {2, 2}
        };
        map.insert({1, 3});
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_unordered_multimap_insert);

// erase

static void BM_ww_unordered_multimap_erase(benchmark::State & state)
{
    for (auto _ : state) {
        wwstl::unordered_multimap<int, int> map = {
            {0, 0}, {1, 1}, {1, 2}
        };
        map.erase(1);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_unordered_multimap_erase);

static void BM_std_unordered_multimap_erase(benchmark::State & state)
{
    for (auto _ : state) {
        std::unordered_multimap<int, int> map = {
            {0, 0}, {1, 1}, {1, 2}
        };
        map.erase(1);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_unordered_multimap_erase);

// count

static void BM_ww_unordered_multimap_count(benchmark::State & state)
{
    wwstl::unordered_multimap<int, int> map = {
        {0, 0}, {1, 1}, {1, 2}
    };
    for (auto _ : state) {
        benchmark::DoNotOptimize(map.count(1));
    }
}
BENCHMARK(BM_ww_unordered_multimap_count);

static void BM_std_unordered_multimap_count(benchmark::State & state)
{
    std::unordered_multimap<int, int> map = {
        {0, 0}, {1, 1}, {1, 2}
    };
    for (auto _ : state) {
        benchmark::DoNotOptimize(map.count(1));
    }
}
BENCHMARK(BM_std_unordered_multimap_count);

// equal_range

static void BM_ww_unordered_multimap_equal_range(benchmark::State & state)
{
    wwstl::unordered_multimap<int, int> map = {
        {0, 0}, {1, 1}, {1, 2}
    };
    for (auto _ : state) {
        benchmark::DoNotOptimize(map.equal_range(1));
    }
}
BENCHMARK(BM_ww_unordered_multimap_equal_range);

static void BM_std_unordered_multimap_equal_range(benchmark::State & state)
{
    std::unordered_multimap<int, int> map = {
        {0, 0}, {1, 1}, {1, 2}
    };
    for (auto _ : state) {
        benchmark::DoNotOptimize(map.equal_range(1));
    }
}
BENCHMARK(BM_std_unordered_multimap_equal_range);

// bucket_iterator

static void BM_ww_unordered_multimap_bucket_iterator(benchmark::State & state)
{
    wwstl::unordered_multimap<int, int> map = {
        {0, 0}, {1, 1}, {2, 2}
    };
    for (auto _ : state) {
        for (auto it = map.begin(1); it != map.end(1); ++it) {
            benchmark::DoNotOptimize(it->second);
        }
    }
}
BENCHMARK(BM_ww_unordered_multimap_bucket_iterator);

static void BM_std_unordered_multimap_bucket_iterator(benchmark::State & state)
{
    std::unordered_multimap<int, int> map = {
        {0, 0}, {1, 1}, {2, 2}
    };
    for (auto _ : state) {
        for (auto it = map.begin(1); it != map.end(1); ++it) {
            benchmark::DoNotOptimize(it->second);
        }
    }
}
BENCHMARK(BM_std_unordered_multimap_bucket_iterator);

// bucket_count

static void BM_ww_unordered_multimap_bucket_count(benchmark::State & state)
{
    wwstl::unordered_multimap<int, int> map = {
        {0, 0}, {1, 1}, {2, 2}
    };
    for (auto _ : state) {
        benchmark::DoNotOptimize(map.bucket_count());
    }
}
BENCHMARK(BM_ww_unordered_multimap_bucket_count);

static void BM_std_unordered_multimap_bucket_count(benchmark::State & state)
{
    std::unordered_multimap<int, int> map = {
        {0, 0}, {1, 1}, {2, 2}
    };
    for (auto _ : state) {
        benchmark::DoNotOptimize(map.bucket_count());
    }
}
BENCHMARK(BM_std_unordered_multimap_bucket_count);

// rehash

static void BM_ww_unordered_multimap_rehash(benchmark::State & state)
{
    for (auto _ : state) {
        wwstl::unordered_multimap<int, int> map = {
            {0, 0}, {1, 1}, {2, 2}
        };
        map.rehash(1024);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_unordered_multimap_rehash);

static void BM_std_unordered_multimap_rehash(benchmark::State & state)
{
    for (auto _ : state) {
        std::unordered_multimap<int, int> map = {
            {0, 0}, {1, 1}, {2, 2}
        };
        map.rehash(1024);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_unordered_multimap_rehash);

BENCHMARK_MAIN();
