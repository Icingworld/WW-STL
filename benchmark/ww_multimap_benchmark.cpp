#include <benchmark/benchmark.h>
#include <map>
#include <ww_map.h>

// iterator

static void BM_ww_multimap_iterator(benchmark::State & state)
{
    wwstl::multimap<int, int> m = {
        {0, 0}, {1, 1}, {2, 2}
    };
    for (auto _ : state) {
        for (auto it = m.begin(); it != m.end(); ++it) {
            benchmark::DoNotOptimize(it->second);
        }
    }
}
BENCHMARK(BM_ww_multimap_iterator);

static void BM_std_multimap_iterator(benchmark::State & state)
{
    std::multimap<int, int> m = {
        {0, 0}, {1, 1}, {2, 2}
    };
    for (auto _ : state) {
        for (auto it = m.begin(); it != m.end(); ++it) {
            benchmark::DoNotOptimize(it->second);
        }
    }
}
BENCHMARK(BM_std_multimap_iterator);

// clear

static void BM_ww_multimap_clear(benchmark::State & state)
{
    for (auto _ : state) {
        wwstl::multimap<int, int> m = {
            {0, 0}, {1, 1}, {2, 2}
        };
        m.clear();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_multimap_clear);

static void BM_std_multimap_clear(benchmark::State & state)
{
    for (auto _ : state) {
        std::multimap<int, int> m = {
            {0, 0}, {1, 1}, {2, 2}
        };
        m.clear();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_multimap_clear);

// insert

static void BM_ww_multimap_insert(benchmark::State & state)
{
    for (auto _ : state) {
        wwstl::multimap<int, int> m = {
            {0, 0}, {1, 1}, {2, 2}
        };
        m.insert({0, 1});
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_multimap_insert);

static void BM_std_multimap_insert(benchmark::State & state)
{
    for (auto _ : state) {
        std::multimap<int, int> m = {
            {0, 0}, {1, 1}, {2, 2}
        };
        m.insert({0, 1});
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_multimap_insert);

// erase

static void BM_ww_multimap_erase(benchmark::State & state)
{
    for (auto _ : state) {
        wwstl::multimap<int, int> m = {
            {0, 0}, {1, 1}, {2, 2}
        };
        m.erase(0);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_multimap_erase);

static void BM_std_multimap_erase(benchmark::State & state)
{
    for (auto _ : state) {
        std::multimap<int, int> m = {
            {0, 0}, {1, 1}, {2, 2}
        };
        m.erase(0);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_multimap_erase);

// count

static void BM_ww_multimap_count(benchmark::State & state)
{
    wwstl::multimap<int, int> m = {
        {0, 0}, {1, 1}, {2, 2}
    };
    for (auto _ : state) {
        benchmark::DoNotOptimize(m.count(0));
    }
}
BENCHMARK(BM_ww_multimap_count);

static void BM_std_multimap_count(benchmark::State & state)
{
    std::multimap<int, int> m = {
        {0, 0}, {1, 1}, {2, 2}
    };
    for (auto _ : state) {
        benchmark::DoNotOptimize(m.count(0));
    }
}
BENCHMARK(BM_std_multimap_count);

// find

static void BM_ww_multimap_find(benchmark::State & state)
{
    wwstl::multimap<int, int> m = {
        {0, 0}, {1, 1}, {2, 2}
    };
    for (auto _ : state) {
        benchmark::DoNotOptimize(m.find(0));
    }
}
BENCHMARK(BM_ww_multimap_find);

static void BM_std_multimap_find(benchmark::State & state)
{
    std::multimap<int, int> m = {
        {0, 0}, {1, 1}, {2, 2}
    };
    for (auto _ : state) {
        benchmark::DoNotOptimize(m.find(0));
    }
}
BENCHMARK(BM_std_multimap_find);

// equal_range

static void BM_ww_multimap_equal_range(benchmark::State & state)
{
    wwstl::multimap<int, int> m = {
        {0, 0}, {1, 1}, {2, 2}
    };
    for (auto _ : state) {
        benchmark::DoNotOptimize(m.equal_range(0));
    }
}
BENCHMARK(BM_ww_multimap_equal_range);

static void BM_std_multimap_equal_range(benchmark::State & state)
{
    std::multimap<int, int> m = {
        {0, 0}, {1, 1}, {2, 2}
    };
    for (auto _ : state) {
        benchmark::DoNotOptimize(m.equal_range(0));
    }
}
BENCHMARK(BM_std_multimap_equal_range);

// lower_bound

static void BM_ww_multimap_lower_bound(benchmark::State & state)
{
    wwstl::multimap<int, int> m = {
        {0, 0}, {1, 1}, {2, 2}
    };
    for (auto _ : state) {
        benchmark::DoNotOptimize(m.lower_bound(0));
    }
}
BENCHMARK(BM_ww_multimap_lower_bound);

static void BM_std_multimap_lower_bound(benchmark::State & state)
{
    std::multimap<int, int> m = {
        {0, 0}, {1, 1}, {2, 2}
    };
    for (auto _ : state) {
        benchmark::DoNotOptimize(m.lower_bound(0));
    }
}
BENCHMARK(BM_std_multimap_lower_bound);

// upper_bound

static void BM_ww_multimap_upper_bound(benchmark::State & state)
{
    wwstl::multimap<int, int> m = {
        {0, 0}, {1, 1}, {2, 2}
    };
    for (auto _ : state) {
        benchmark::DoNotOptimize(m.upper_bound(0));
    }
}
BENCHMARK(BM_ww_multimap_upper_bound);

static void BM_std_multimap_upper_bound(benchmark::State & state)
{
    std::multimap<int, int> m = {
        {0, 0}, {1, 1}, {2, 2}
    };
    for (auto _ : state) {
        benchmark::DoNotOptimize(m.upper_bound(0));
    }
}
BENCHMARK(BM_std_multimap_upper_bound);
