#include <benchmark/benchmark.h>
#include <unordered_set>
#include <ww_unordered_set.h>

// iterator

static void BM_ww_unordered_set_iterator(benchmark::State & state)
{
    wwstl::unordered_set<int> s = {1, 2, 3, 4, 5, 6, 7};
    for (auto _ : state) {
        for (auto it = s.begin(); it != s.end(); ++it) {
            int i = *it;
            benchmark::DoNotOptimize(i);
        }
    }
}
BENCHMARK(BM_ww_unordered_set_iterator);

static void BM_std_unordered_set_iterator(benchmark::State & state)
{
    std::unordered_set<int> s = {1, 2, 3, 4, 5, 6, 7};
    for (auto _ : state) {
        for (auto it = s.begin(); it != s.end(); ++it) {
            int i = *it;
            benchmark::DoNotOptimize(i);
        }
    }
}
BENCHMARK(BM_std_unordered_set_iterator);

// clear

static void BM_ww_unordered_set_clear(benchmark::State & state)
{
    for (auto _ : state) {
        wwstl::unordered_set<int> s = {1, 2, 3, 4, 5, 6, 7};
        s.clear();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_unordered_set_clear);

static void BM_std_unordered_set_clear(benchmark::State & state)
{
    for (auto _ : state) {
        std::unordered_set<int> s = {1, 2, 3, 4, 5, 6, 7};
        s.clear();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_unordered_set_clear);

// insert

static void BM_ww_unordered_set_insert(benchmark::State & state)
{
    for (auto _ : state) {
        wwstl::unordered_set<int> s = {1, 2, 3, 4, 5, 6, 7};
        s.insert(8);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_unordered_set_insert);

static void BM_std_unordered_set_insert(benchmark::State & state)
{
    for (auto _ : state) {
        std::unordered_set<int> s = {1, 2, 3, 4, 5, 6, 7};
        s.insert(8);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_unordered_set_insert);

// erase

static void BM_ww_unordered_set_erase(benchmark::State & state)
{
    for (auto _ : state) {
        wwstl::unordered_set<int> s = {1, 2, 3, 4, 5, 6, 7};
        s.erase(4);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_unordered_set_erase);

// count

static void BM_ww_unordered_set_count(benchmark::State & state)
{
    wwstl::unordered_set<int> s = {1, 2, 3, 4, 5, 6, 7};
    for (auto _ : state) {
        benchmark::DoNotOptimize(s.count(4));
    }
}
BENCHMARK(BM_ww_unordered_set_count);

static void BM_std_unordered_set_count(benchmark::State & state)
{
    std::unordered_set<int> s = {1, 2, 3, 4, 5, 6, 7};
    for (auto _ : state) {
        benchmark::DoNotOptimize(s.count(4));
    }
}
BENCHMARK(BM_std_unordered_set_count);

// find

static void BM_ww_unordered_set_find(benchmark::State & state)
{
    wwstl::unordered_set<int> s = {1, 2, 3, 4, 5, 6, 7};
    for (auto _ : state) {
        benchmark::DoNotOptimize(s.find(4));
    }
}
BENCHMARK(BM_ww_unordered_set_find);

static void BM_std_unordered_set_find(benchmark::State & state)
{
    std::unordered_set<int> s = {1, 2, 3, 4, 5, 6, 7};
    for (auto _ : state) {
        benchmark::DoNotOptimize(s.find(4));
    }
}
BENCHMARK(BM_std_unordered_set_find);

// equal_range

static void BM_ww_unordered_set_equal_range(benchmark::State & state)
{
    wwstl::unordered_set<int> s = {1, 2, 3, 4, 5, 6, 7};
    for (auto _ : state) {
        benchmark::DoNotOptimize(s.equal_range(4));
    }
}
BENCHMARK(BM_ww_unordered_set_equal_range);

static void BM_std_unordered_set_equal_range(benchmark::State & state)
{
    std::unordered_set<int> s = {1, 2, 3, 4, 5, 6, 7};
    for (auto _ : state) {
        benchmark::DoNotOptimize(s.equal_range(4));
    }
}
BENCHMARK(BM_std_unordered_set_equal_range);

// bucket iterator

static void BM_ww_unordered_set_bucket_iterator(benchmark::State & state)
{
    wwstl::unordered_set<int> s = {1, 2, 3, 4, 5, 6, 7};
    for (auto _ : state) {
        for (auto it = s.begin(4); it != s.end(4); ++it) {
            int i = *it;
            benchmark::DoNotOptimize(i);
        }
    }
}
BENCHMARK(BM_ww_unordered_set_bucket_iterator);

static void BM_std_unordered_set_bucket_iterator(benchmark::State & state)
{
    std::unordered_set<int> s = {1, 2, 3, 4, 5, 6, 7};
    for (auto _ : state) {
        for (auto it = s.begin(4); it != s.end(4); ++it) {
            int i = *it;
            benchmark::DoNotOptimize(i);
        }
    }
}
BENCHMARK(BM_std_unordered_set_bucket_iterator);

// bucket_count

static void BM_ww_unordered_set_bucket_count(benchmark::State & state)
{
    wwstl::unordered_set<int> s = {1, 2, 3, 4, 5, 6, 7};
    for (auto _ : state) {
        benchmark::DoNotOptimize(s.bucket_count());
    }
}
BENCHMARK(BM_ww_unordered_set_bucket_count);

static void BM_std_unordered_set_bucket_count(benchmark::State & state)
{
    std::unordered_set<int> s = {1, 2, 3, 4, 5, 6, 7};
    for (auto _ : state) {
        benchmark::DoNotOptimize(s.bucket_count());
    }
}
BENCHMARK(BM_std_unordered_set_bucket_count);

// rehash

static void BM_ww_unordered_set_rehash(benchmark::State & state)
{
    for (auto _ : state) {
        wwstl::unordered_set<int> s = {1, 2, 3, 4, 5, 6, 7};
        s.rehash(1024);
    }
}
BENCHMARK(BM_ww_unordered_set_rehash);

static void BM_std_unordered_set_rehash(benchmark::State & state)
{
    for (auto _ : state) {
        std::unordered_set<int> s = {1, 2, 3, 4, 5, 6, 7};
        s.rehash(1024);
    }
}
BENCHMARK(BM_std_unordered_set_rehash);

BENCHMARK_MAIN();
