#include <benchmark/benchmark.h>
#include <unordered_set>
#include <ww_unordered_set.h>

// iterator

static void BM_ww_unordered_multiset_iterator(benchmark::State & state)
{
    wwstl::unordered_multiset<int> s = {1, 2, 3, 3, 3, 4, 5, 6, 7};
    for (auto _ : state) {
        for (auto it = s.begin(); it != s.end(); ++it) {
            int i = *it;
            benchmark::DoNotOptimize(i);
        }
    }
}
BENCHMARK(BM_ww_unordered_multiset_iterator);

static void BM_std_unordered_multiset_iterator(benchmark::State & state)
{
    std::unordered_multiset<int> s = {1, 2, 3, 3, 3, 4, 5, 6, 7};
    for (auto _ : state) {
        for (auto it = s.begin(); it != s.end(); ++it) {
            int i = *it;
            benchmark::DoNotOptimize(i);
        }
    }
}
BENCHMARK(BM_std_unordered_multiset_iterator);

// clear

static void BM_ww_unordered_multiset_clear(benchmark::State & state)
{
    for (auto _ : state) {
        wwstl::unordered_multiset<int> s = {1, 2, 3, 3, 3, 4, 5, 6, 7};
        s.clear();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_unordered_multiset_clear);

static void BM_std_unordered_multiset_clear(benchmark::State & state)
{
    for (auto _ : state) {
        std::unordered_multiset<int> s = {1, 2, 3, 3, 3, 4, 5, 6, 7};
        s.clear();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_unordered_multiset_clear);

// insert

static void BM_ww_unordered_multiset_insert(benchmark::State & state)
{
    for (auto _ : state) {
        wwstl::unordered_multiset<int> s = {1, 2, 3, 3, 3, 4, 5, 6, 7};
        s.insert(8);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_unordered_multiset_insert);

static void BM_std_unordered_multiset_insert(benchmark::State & state)
{
    for (auto _ : state) {
        std::unordered_multiset<int> s = {1, 2, 3, 3, 3, 4, 5, 6, 7};
        s.insert(8);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_unordered_multiset_insert);

// erase

static void BM_ww_unordered_multiset_erase(benchmark::State & state)
{
    for (auto _ : state) {
        wwstl::unordered_multiset<int> s = {1, 2, 3, 3, 3, 4, 5, 6, 7};
        s.erase(4);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_unordered_multiset_erase);

// count

static void BM_ww_unordered_multiset_count(benchmark::State & state)
{
    wwstl::unordered_multiset<int> s = {1, 2, 3, 3, 3, 4, 5, 6, 7};
    for (auto _ : state) {
        benchmark::DoNotOptimize(s.count(4));
    }
}
BENCHMARK(BM_ww_unordered_multiset_count);

static void BM_std_unordered_multiset_count(benchmark::State & state)
{
    std::unordered_multiset<int> s = {1, 2, 3, 3, 3, 4, 5, 6, 7};
    for (auto _ : state) {
        benchmark::DoNotOptimize(s.count(3));
    }
}
BENCHMARK(BM_std_unordered_multiset_count);

// find

static void BM_ww_unordered_multiset_find(benchmark::State & state)
{
    wwstl::unordered_multiset<int> s = {1, 2, 3, 3, 3, 4, 5, 6, 7};
    for (auto _ : state) {
        benchmark::DoNotOptimize(s.find(3));
    }
}
BENCHMARK(BM_ww_unordered_multiset_find);

static void BM_std_unordered_multiset_find(benchmark::State & state)
{
    std::unordered_multiset<int> s = {1, 2, 3, 3, 3, 4, 5, 6, 7};
    for (auto _ : state) {
        benchmark::DoNotOptimize(s.find(4));
    }
}
BENCHMARK(BM_std_unordered_multiset_find);

// equal_range

static void BM_ww_unordered_multiset_equal_range(benchmark::State & state)
{
    wwstl::unordered_multiset<int> s = {1, 2, 3, 3, 3, 4, 5, 6, 7};
    for (auto _ : state) {
        benchmark::DoNotOptimize(s.equal_range(3));
    }
}
BENCHMARK(BM_ww_unordered_multiset_equal_range);

static void BM_std_unordered_multiset_equal_range(benchmark::State & state)
{
    std::unordered_multiset<int> s = {1, 2, 3, 3, 3, 4, 5, 6, 7};
    for (auto _ : state) {
        benchmark::DoNotOptimize(s.equal_range(3));
    }
}
BENCHMARK(BM_std_unordered_multiset_equal_range);

// bucket iterator

static void BM_ww_unordered_multiset_bucket_iterator(benchmark::State & state)
{
    wwstl::unordered_multiset<int> s = {1, 2, 3, 3, 3, 4, 5, 6, 7};
    for (auto _ : state) {
        for (auto it = s.begin(3); it != s.end(3); ++it) {
            int i = *it;
            benchmark::DoNotOptimize(i);
        }
    }
}
BENCHMARK(BM_ww_unordered_multiset_bucket_iterator);

static void BM_std_unordered_multiset_bucket_iterator(benchmark::State & state)
{
    std::unordered_multiset<int> s = {1, 2, 3, 3, 3, 4, 5, 6, 7};
    for (auto _ : state) {
        for (auto it = s.begin(3); it != s.end(3); ++it) {
            int i = *it;
            benchmark::DoNotOptimize(i);
        }
    }
}
BENCHMARK(BM_std_unordered_multiset_bucket_iterator);

// bucket_count

static void BM_ww_unordered_multiset_bucket_count(benchmark::State & state)
{
    wwstl::unordered_multiset<int> s = {1, 2, 3, 3, 3, 4, 5, 6, 7};
    for (auto _ : state) {
        benchmark::DoNotOptimize(s.bucket_count());
    }
}
BENCHMARK(BM_ww_unordered_multiset_bucket_count);

static void BM_std_unordered_multiset_bucket_count(benchmark::State & state)
{
    std::unordered_multiset<int> s = {1, 2, 3, 3, 3, 4, 5, 6, 7};
    for (auto _ : state) {
        benchmark::DoNotOptimize(s.bucket_count());
    }
}
BENCHMARK(BM_std_unordered_multiset_bucket_count);

// rehash

static void BM_ww_unordered_multiset_rehash(benchmark::State & state)
{
    for (auto _ : state) {
        wwstl::unordered_multiset<int> s = {1, 2, 3, 3, 3, 4, 5, 6, 7};
        s.rehash(1024);
    }
}
BENCHMARK(BM_ww_unordered_multiset_rehash);

static void BM_std_unordered_multiset_rehash(benchmark::State & state)
{
    for (auto _ : state) {
        std::unordered_multiset<int> s = {1, 2, 3, 3, 3, 4, 5, 6, 7};
        s.rehash(1024);
    }
}
BENCHMARK(BM_std_unordered_multiset_rehash);
