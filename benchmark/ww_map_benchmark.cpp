#include <benchmark/benchmark.h>
#include <map>
#include <ww_map.h>

// access

static void BM_ww_map_access(benchmark::State & state)
{
    std::map<int, int> m = {
        {0, 0}, {1, 1}, {2, 2}
    };
    for (auto _ : state) {
        benchmark::DoNotOptimize(m[0]);
    }
}
BENCHMARK(BM_ww_map_access);

static void BM_std_map_access(benchmark::State & state)
{
    std::map<int, int> m = {
        {0, 0}, {1, 1}, {2, 2}
    };
    for (auto _ : state) {
        benchmark::DoNotOptimize(m.at(0));
    }
}
BENCHMARK(BM_std_map_access);

// at

static void BM_ww_map_at(benchmark::State & state)
{
    wwstl::map<int, int> m = {
        {0, 0}, {1, 1}, {2, 2}
    };
    for (auto _ : state) {
        benchmark::DoNotOptimize(m.at(0));
    }
}
BENCHMARK(BM_ww_map_at);

static void BM_std_map_at(benchmark::State & state)
{
    std::map<int, int> m = {
        {0, 0}, {1, 1}, {2, 2}
    };
    for (auto _ : state) {
        benchmark::DoNotOptimize(m.at(0));
    }
}
BENCHMARK(BM_std_map_at);

// iterator

static void BM_ww_map_iterator(benchmark::State & state)
{
    wwstl::map<int, int> m = {
        {0, 0}, {1, 1}, {2, 2}
    };
    for (auto _ : state) {
        for (auto it = m.begin(); it != m.end(); ++it) {
            benchmark::DoNotOptimize(it->second);
        }
    }
}
BENCHMARK(BM_ww_map_iterator);

static void BM_std_map_iterator(benchmark::State & state)
{
    std::map<int, int> m = {
        {0, 0}, {1, 1}, {2, 2}
    };
    for (auto _ : state) {
        for (auto it = m.begin(); it != m.end(); ++it) {
            benchmark::DoNotOptimize(it->second);
        }
    }
}
BENCHMARK(BM_std_map_iterator);

// clear

static void BM_ww_map_clear(benchmark::State & state)
{
    for (auto _ : state) {
        wwstl::map<int, int> m = {
            {0, 0}, {1, 1}, {2, 2}
        };
        m.clear();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_map_clear);

static void BM_std_map_clear(benchmark::State & state)
{
    for (auto _ : state) {
        std::map<int, int> m = {
            {0, 0}, {1, 1}, {2, 2}
        };
        m.clear();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_map_clear);

// insert

static void BM_ww_map_insert(benchmark::State & state)
{
    for (auto _ : state) {
        wwstl::map<int, int> m = {
            {0, 0}, {1, 1}, {2, 2}
        };
        m.insert({3, 3});
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_map_insert);

static void BM_std_map_insert(benchmark::State & state)
{
    for (auto _ : state) {
        std::map<int, int> m = {
            {0, 0}, {1, 1}, {2, 2}
        };
        m.insert({3, 3});
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_map_insert);

// erase

static void BM_ww_map_erase(benchmark::State & state)
{
    for (auto _ : state) {
        wwstl::map<int, int> m = {
            {0, 0}, {1, 1}, {2, 2}
        };
        m.erase(0);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_map_erase);

static void BM_std_map_erase(benchmark::State & state)
{
    for (auto _ : state) {
        std::map<int, int> m = {
            {0, 0}, {1, 1}, {2, 2}
        };
        m.erase(0);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_map_erase);

// count

static void BM_ww_map_count(benchmark::State & state)
{
    wwstl::map<int, int> m = {
        {0, 0}, {1, 1}, {2, 2}
    };
    for (auto _ : state) {
        benchmark::DoNotOptimize(m.count(0));
    }
}
BENCHMARK(BM_ww_map_count);

static void BM_std_map_count(benchmark::State & state)
{
    std::map<int, int> m = {
        {0, 0}, {1, 1}, {2, 2}
    };
    for (auto _ : state) {
        benchmark::DoNotOptimize(m.count(0));
    }
}
BENCHMARK(BM_std_map_count);

// find

static void BM_ww_map_find(benchmark::State & state)
{
    wwstl::map<int, int> m = {
        {0, 0}, {1, 1}, {2, 2}
    };
    for (auto _ : state) {
        benchmark::DoNotOptimize(m.find(0));
    }
}
BENCHMARK(BM_ww_map_find);

static void BM_std_map_find(benchmark::State & state)
{
    std::map<int, int> m = {
        {0, 0}, {1, 1}, {2, 2}
    };
    for (auto _ : state) {
        benchmark::DoNotOptimize(m.find(0));
    }
}
BENCHMARK(BM_std_map_find);

// equal_range

static void BM_ww_map_equal_range(benchmark::State & state)
{
    wwstl::map<int, int> m = {
        {0, 0}, {1, 1}, {2, 2}
    };
    for (auto _ : state) {
        benchmark::DoNotOptimize(m.equal_range(0));
    }
}
BENCHMARK(BM_ww_map_equal_range);

static void BM_std_map_equal_range(benchmark::State & state)
{
    std::map<int, int> m = {
        {0, 0}, {1, 1}, {2, 2}
    };
    for (auto _ : state) {
        benchmark::DoNotOptimize(m.equal_range(0));
    }
}
BENCHMARK(BM_std_map_equal_range);

// lower_bound

static void BM_ww_map_lower_bound(benchmark::State & state)
{
    wwstl::map<int, int> m = {
        {0, 0}, {1, 1}, {2, 2}
    };
    for (auto _ : state) {
        benchmark::DoNotOptimize(m.lower_bound(0));
    }
}
BENCHMARK(BM_ww_map_lower_bound);

static void BM_std_map_lower_bound(benchmark::State & state)
{
    std::map<int, int> m = {
        {0, 0}, {1, 1}, {2, 2}
    };
    for (auto _ : state) {
        benchmark::DoNotOptimize(m.lower_bound(0));
    }
}
BENCHMARK(BM_std_map_lower_bound);

// upper_bound

static void BM_ww_map_upper_bound(benchmark::State & state)
{
    wwstl::map<int, int> m = {
        {0, 0}, {1, 1}, {2, 2}
    };
    for (auto _ : state) {
        benchmark::DoNotOptimize(m.upper_bound(0));
    }
}
BENCHMARK(BM_ww_map_upper_bound);

static void BM_std_map_upper_bound(benchmark::State & state)
{
    std::map<int, int> m = {
        {0, 0}, {1, 1}, {2, 2}
    };
    for (auto _ : state) {
        benchmark::DoNotOptimize(m.upper_bound(0));
    }
}
BENCHMARK(BM_std_map_upper_bound);

BENCHMARK_MAIN();
