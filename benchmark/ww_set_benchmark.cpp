#include <benchmark/benchmark.h>
#include <set>
#include <ww_set.h>

// iterator

static void BM_ww_set_iterator(benchmark::State & state)
{
    wwstl::set<int> set = {1, 2, 3, 4, 5, 6, 7};
    for (auto _ : state) {
        for (auto it = set.begin(); it != set.end(); ++it) {
            int i = *it;
            benchmark::DoNotOptimize(i);
        }
    }
}
BENCHMARK(BM_ww_set_iterator);

static void BM_std_set_iterator(benchmark::State & state)
{
    std::set<int> set = {1, 2, 3, 4, 5, 6, 7};
    for (auto _ : state) {
        for (auto it = set.begin(); it != set.end(); ++it) {
            int i = *it;
            benchmark::DoNotOptimize(i);
        }
    }
}
BENCHMARK(BM_std_set_iterator);

// clear

static void BM_ww_set_clear(benchmark::State & state)
{
    for (auto _ : state) {
        wwstl::set<int> set = {1, 2, 3, 4, 5, 6, 7};
        set.clear();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_set_clear);

static void BM_std_set_clear(benchmark::State & state)
{
    for (auto _ : state) {
        std::set<int> set = {1, 2, 3, 4, 5, 6, 7};
        set.clear();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_set_clear);

// insert

static void BM_ww_set_insert(benchmark::State & state)
{
    for (auto _ : state) {
        wwstl::set<int> set;
        set.insert({1, 2, 3, 4, 5, 6, 7});
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_set_insert);

static void BM_std_set_insert(benchmark::State & state)
{
    for (auto _ : state) {
        std::set<int> set;
        set.insert({1, 2, 3, 4, 5, 6, 7});
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_set_insert);

// erase

static void BM_ww_set_erase(benchmark::State & state)
{
    wwstl::set<int> set = {1, 2, 3, 4, 5, 6, 7};
    for (auto _ : state) {
        set.erase(4);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_set_erase);

static void BM_std_set_erase(benchmark::State & state)
{
    std::set<int> set = {1, 2, 3, 4, 5, 6, 7};
    for (auto _ : state) {
        set.erase(4);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_set_erase);

// count

static void BM_ww_set_count(benchmark::State & state)
{
    wwstl::set<int> set = {1, 2, 3, 4, 5, 6, 7};
    for (auto _ : state) {
        benchmark::DoNotOptimize(set.count(4));
    }
}
BENCHMARK(BM_ww_set_count);

static void BM_std_set_count(benchmark::State & state)
{
    std::set<int> set = {1, 2, 3, 4, 5, 6, 7};
    for (auto _ : state) {
        benchmark::DoNotOptimize(set.count(4));
    }
}
BENCHMARK(BM_std_set_count);

// find

static void BM_ww_set_find(benchmark::State & state)
{
    wwstl::set<int> set = {1, 2, 3, 4, 5, 6, 7};
    for (auto _ : state) {
        benchmark::DoNotOptimize(set.find(4));
    }
}
BENCHMARK(BM_ww_set_find);

static void BM_std_set_find(benchmark::State & state)
{
    std::set<int> set = {1, 2, 3, 4, 5, 6, 7};
    for (auto _ : state) {
        benchmark::DoNotOptimize(set.find(4));
    }
}
BENCHMARK(BM_std_set_find);

// equal_range

static void BM_ww_set_equal_range(benchmark::State & state)
{
    wwstl::set<int> set = {1, 2, 3, 4, 5, 6, 7};
    for (auto _ : state) {
        benchmark::DoNotOptimize(set.equal_range(4));
    }
}
BENCHMARK(BM_ww_set_equal_range);

static void BM_std_set_equal_range(benchmark::State & state)
{
    std::set<int> set = {1, 2, 3, 4, 5, 6, 7};
    for (auto _ : state) {
        benchmark::DoNotOptimize(set.equal_range(4));
    }
}
BENCHMARK(BM_std_set_equal_range);

// lower_bound

static void BM_ww_set_lower_bound(benchmark::State & state)
{
    wwstl::set<int> set = {1, 2, 3, 4, 5, 6, 7};
    for (auto _ : state) {
        benchmark::DoNotOptimize(set.lower_bound(4));
    }
}
BENCHMARK(BM_ww_set_lower_bound);

static void BM_std_set_lower_bound(benchmark::State & state)
{
    std::set<int> set = {1, 2, 3, 4, 5, 6, 7};
    for (auto _ : state) {
        benchmark::DoNotOptimize(set.lower_bound(4));
    }
}
BENCHMARK(BM_std_set_lower_bound);

// upper_bound

static void BM_ww_set_upper_bound(benchmark::State & state)
{
    wwstl::set<int> set = {1, 2, 3, 4, 5, 6, 7};
    for (auto _ : state) {
        benchmark::DoNotOptimize(set.upper_bound(4));
    }
}
BENCHMARK(BM_ww_set_upper_bound);

static void BM_std_set_upper_bound(benchmark::State & state)
{
    std::set<int> set = {1, 2, 3, 4, 5, 6, 7};
    for (auto _ : state) {
        benchmark::DoNotOptimize(set.upper_bound(4));
    }
}
BENCHMARK(BM_std_set_upper_bound);

BENCHMARK_MAIN();
