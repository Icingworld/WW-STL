#include <benchmark/benchmark.h>
#include <set>
#include <ww_set.h>

// iterator

static void BM_ww_multiset_iterator(benchmark::State & state)
{
    wwstl::multiset<int> set = {1, 2, 3, 4, 5, 6, 7};
    for (auto _ : state) {
        for (auto it = set.begin(); it != set.end(); ++it) {
            int i = *it;
            benchmark::DoNotOptimize(i);
        }
    }
}
BENCHMARK(BM_ww_multiset_iterator);

static void BM_std_multiset_iterator(benchmark::State & state)
{
    std::multiset<int> set = {1, 2, 3, 4, 5, 6, 7};
    for (auto _ : state) {
        for (auto it = set.begin(); it != set.end(); ++it) {
            int i = *it;
            benchmark::DoNotOptimize(i);
        }
    }
}
BENCHMARK(BM_std_multiset_iterator);

// clear

static void BM_ww_multiset_clear(benchmark::State & state)
{
    for (auto _ : state) {
        wwstl::multiset<int> set = {1, 2, 3, 4, 5, 6, 7};
        set.clear();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_multiset_clear);

static void BM_std_multiset_clear(benchmark::State & state)
{
    for (auto _ : state) {
        std::multiset<int> set = {1, 2, 3, 4, 5, 6, 7};
        set.clear();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_multiset_clear);

// insert

static void BM_ww_multiset_insert(benchmark::State & state)
{
    for (auto _ : state) {
        wwstl::multiset<int> set;
        set.insert({1, 2, 3, 4, 5, 6, 7});
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_multiset_insert);

static void BM_std_multiset_insert(benchmark::State & state)
{
    for (auto _ : state) {
        std::multiset<int> set;
        set.insert({1, 2, 3, 4, 5, 6, 7});
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_multiset_insert);

// erase

static void BM_ww_multiset_erase(benchmark::State & state)
{
    wwstl::multiset<int> set = {1, 2, 3, 4, 5, 6, 7};
    for (auto _ : state) {
        set.erase(4);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_multiset_erase);

static void BM_std_multiset_erase(benchmark::State & state)
{
    std::multiset<int> set = {1, 2, 3, 4, 5, 6, 7};
    for (auto _ : state) {
        set.erase(4);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_multiset_erase);

// count

static void BM_ww_multiset_count(benchmark::State & state)
{
    wwstl::multiset<int> set = {1, 2, 3, 4, 5, 6, 7};
    for (auto _ : state) {
        benchmark::DoNotOptimize(set.count(4));
    }
}
BENCHMARK(BM_ww_multiset_count);

static void BM_std_multiset_count(benchmark::State & state)
{
    std::multiset<int> set = {1, 2, 3, 4, 5, 6, 7};
    for (auto _ : state) {
        benchmark::DoNotOptimize(set.count(4));
    }
}
BENCHMARK(BM_std_multiset_count);

// find

static void BM_ww_multiset_find(benchmark::State & state)
{
    wwstl::multiset<int> set = {1, 2, 3, 4, 5, 6, 7};
    for (auto _ : state) {
        benchmark::DoNotOptimize(set.find(4));
    }
}
BENCHMARK(BM_ww_multiset_find);

static void BM_std_multiset_find(benchmark::State & state)
{
    std::multiset<int> set = {1, 2, 3, 4, 5, 6, 7};
    for (auto _ : state) {
        benchmark::DoNotOptimize(set.find(4));
    }
}
BENCHMARK(BM_std_multiset_find);

// equal_range

static void BM_ww_multiset_equal_range(benchmark::State & state)
{
    wwstl::multiset<int> set = {1, 2, 3, 4, 5, 6, 7};
    for (auto _ : state) {
        benchmark::DoNotOptimize(set.equal_range(4));
    }
}
BENCHMARK(BM_ww_multiset_equal_range);

static void BM_std_multiset_equal_range(benchmark::State & state)
{
    std::multiset<int> set = {1, 2, 3, 4, 5, 6, 7};
    for (auto _ : state) {
        benchmark::DoNotOptimize(set.equal_range(4));
    }
}
BENCHMARK(BM_std_multiset_equal_range);

// lower_bound

static void BM_ww_multiset_lower_bound(benchmark::State & state)
{
    wwstl::multiset<int> set = {1, 2, 3, 4, 5, 6, 7};
    for (auto _ : state) {
        benchmark::DoNotOptimize(set.lower_bound(4));
    }
}
BENCHMARK(BM_ww_multiset_lower_bound);

static void BM_std_multiset_lower_bound(benchmark::State & state)
{
    std::multiset<int> set = {1, 2, 3, 4, 5, 6, 7};
    for (auto _ : state) {
        benchmark::DoNotOptimize(set.lower_bound(4));
    }
}
BENCHMARK(BM_std_multiset_lower_bound);

// upper_bound

static void BM_ww_multiset_upper_bound(benchmark::State & state)
{
    wwstl::multiset<int> set = {1, 2, 3, 4, 5, 6, 7};
    for (auto _ : state) {
        benchmark::DoNotOptimize(set.upper_bound(4));
    }
}
BENCHMARK(BM_ww_multiset_upper_bound);

static void BM_std_multiset_upper_bound(benchmark::State & state)
{
    std::multiset<int> set = {1, 2, 3, 4, 5, 6, 7};
    for (auto _ : state) {
        benchmark::DoNotOptimize(set.upper_bound(4));
    }
}
BENCHMARK(BM_std_multiset_upper_bound);

BENCHMARK_MAIN();
