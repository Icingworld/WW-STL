#include <benchmark/benchmark.h>
#include <unordered_map>
#include <ww_unordered_map.h>

// iterator

static void BM_ww_unordered_map_iterator(benchmark::State & state)
{
    wwstl::unordered_map<int, int> map = {
        {0, 0}, {1, 1}, {2, 2}
    };
    for (auto _ : state) {
        for (auto it = map.begin(); it != map.end(); ++it) {
            benchmark::DoNotOptimize(it->second);
        }
    }
}
BENCHMARK(BM_ww_unordered_map_iterator);

static void BM_std_unordered_map_iterator(benchmark::State & state)
{
    std::unordered_map<int, int> map = {
        {0, 0}, {1, 1}, {2, 2}
    };
    for (auto _ : state) {
        for (auto it = map.begin(); it != map.end(); ++it) {
            benchmark::DoNotOptimize(it->second);
        }
    }
}
BENCHMARK(BM_std_unordered_map_iterator);

// clear

static void BM_ww_unordered_map_clear(benchmark::State & state)
{
    for (auto _ : state) {
        wwstl::unordered_map<int, int> map = {
            {0, 0}, {1, 1}, {2, 2}
        };
        map.clear();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_unordered_map_clear);

static void BM_std_unordered_map_clear(benchmark::State & state)
{
    for (auto _ : state) {
        std::unordered_map<int, int> map = {
            {0, 0}, {1, 1}, {2, 2}
        };
        map.clear();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_unordered_map_clear);

// insert

static void BM_ww_unordered_map_insert(benchmark::State & state)
{
    for (auto _ : state) {
        wwstl::unordered_map<int, int> map = {
            {0, 0}, {1, 1}, {2, 2}
        };
        map.insert({3, 3});
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_unordered_map_insert);

static void BM_std_unordered_map_insert(benchmark::State & state)
{
    for (auto _ : state) {
        std::unordered_map<int, int> map = {
            {0, 0}, {1, 1}, {2, 2}
        };
        map.insert({3, 3});
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_unordered_map_insert);

// erase

static void BM_ww_unordered_map_erase(benchmark::State & state)
{
    for (auto _ : state) {
        wwstl::unordered_map<int, int> map = {
            {0, 0}, {1, 1}, {2, 2}
        };
        map.erase(1);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_unordered_map_erase);

static void BM_std_unordered_map_erase(benchmark::State & state)
{
    for (auto _ : state) {
        std::unordered_map<int, int> map = {
            {0, 0}, {1, 1}, {2, 2}
        };
        map.erase(1);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_unordered_map_erase);

// access

static void BM_ww_unordered_map_access(benchmark::State & state)
{
    std::unordered_map<int, int> map = {
        {0, 0}, {1, 1}, {2, 2}
    };
    for (auto _ : state) {
        benchmark::DoNotOptimize(map[0]);
    }
}
BENCHMARK(BM_ww_unordered_map_access);

static void BM_std_unordered_map_access(benchmark::State & state)
{
    std::unordered_map<int, int> map = {
        {0, 0}, {1, 1}, {2, 2}
    };
    for (auto _ : state) {
        benchmark::DoNotOptimize(map[0]);
    }
}
BENCHMARK(BM_std_unordered_map_access);

// at

static void BM_ww_unordered_map_at(benchmark::State & state)
{
    wwstl::unordered_map<int, int> map = {
        {0, 0}, {1, 1}, {2, 2}
    };
    for (auto _ : state) {
        benchmark::DoNotOptimize(map.at(0));
    }
}
BENCHMARK(BM_ww_unordered_map_at);

static void BM_std_unordered_map_at(benchmark::State & state)
{
    std::unordered_map<int, int> map = {
        {0, 0}, {1, 1}, {2, 2}
    };
    for (auto _ : state) {
        benchmark::DoNotOptimize(map.at(0));
    }
}
BENCHMARK(BM_std_unordered_map_at);

// count

static void BM_ww_unordered_map_count(benchmark::State & state)
{
    wwstl::unordered_map<int, int> map = {
        {0, 0}, {1, 1}, {2, 2}
    };
    for (auto _ : state) {
        benchmark::DoNotOptimize(map.count(0));
    }
}
BENCHMARK(BM_ww_unordered_map_count);

static void BM_std_unordered_map_count(benchmark::State & state)
{
    std::unordered_map<int, int> map = {
        {0, 0}, {1, 1}, {2, 2}
    };
    for (auto _ : state) {
        benchmark::DoNotOptimize(map.count(0));
    }
}
BENCHMARK(BM_std_unordered_map_count);

// equal_range

static void BM_ww_unordered_map_equal_range(benchmark::State & state)
{
    wwstl::unordered_map<int, int> map = {
        {0, 0}, {1, 1}, {2, 2}
    };
    for (auto _ : state) {
        benchmark::DoNotOptimize(map.equal_range(0));
    }
}
BENCHMARK(BM_ww_unordered_map_equal_range);

static void BM_std_unordered_map_equal_range(benchmark::State & state)
{
    std::unordered_map<int, int> map = {
        {0, 0}, {1, 1}, {2, 2}
    };
    for (auto _ : state) {
        benchmark::DoNotOptimize(map.equal_range(0));
    }
}
BENCHMARK(BM_std_unordered_map_equal_range);

// bucket_iterator

static void BM_ww_unordered_map_bucket_iterator(benchmark::State & state)
{
    wwstl::unordered_map<int, int> map = {
        {0, 0}, {1, 1}, {2, 2}
    };
    for (auto _ : state) {
        for (auto it = map.begin(1); it != map.end(1); ++it) {
            benchmark::DoNotOptimize(it->second);
        }
    }
}
BENCHMARK(BM_ww_unordered_map_bucket_iterator);

static void BM_std_unordered_map_bucket_iterator(benchmark::State & state)
{
    std::unordered_map<int, int> map = {
        {0, 0}, {1, 1}, {2, 2}
    };
    for (auto _ : state) {
        for (auto it = map.begin(1); it != map.end(1); ++it) {
            benchmark::DoNotOptimize(it->second);
        }
    }
}
BENCHMARK(BM_std_unordered_map_bucket_iterator);

// bucket_count

static void BM_ww_unordered_map_bucket_count(benchmark::State & state)
{
    wwstl::unordered_map<int, int> map = {
        {0, 0}, {1, 1}, {2, 2}
    };
    for (auto _ : state) {
        benchmark::DoNotOptimize(map.bucket_count());
    }
}
BENCHMARK(BM_ww_unordered_map_bucket_count);

static void BM_std_unordered_map_bucket_count(benchmark::State & state)
{
    std::unordered_map<int, int> map = {
        {0, 0}, {1, 1}, {2, 2}
    };
    for (auto _ : state) {
        benchmark::DoNotOptimize(map.bucket_count());
    }
}
BENCHMARK(BM_std_unordered_map_bucket_count);

// rehash

static void BM_ww_unordered_map_rehash(benchmark::State & state)
{
    for (auto _ : state) {
        wwstl::unordered_map<int, int> map = {
            {0, 0}, {1, 1}, {2, 2}
        };
        map.rehash(1024);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_unordered_map_rehash);

static void BM_std_unordered_map_rehash(benchmark::State & state)
{
    for (auto _ : state) {
        std::unordered_map<int, int> map = {
            {0, 0}, {1, 1}, {2, 2}
        };
        map.rehash(1024);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_unordered_map_rehash);
