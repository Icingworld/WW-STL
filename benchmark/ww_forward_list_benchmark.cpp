#include <benchmark/benchmark.h>
#include <forward_list>
#include <ww_forward_list.h>

// front

static void BM_ww_forward_list_front(benchmark::State & state)
{
    wwstl::forward_list<int> l(1024);
    for (auto _ : state) {
        benchmark::DoNotOptimize(l.front());
    }
}
BENCHMARK(BM_ww_forward_list_front);

static void BM_std_forward_list_front(benchmark::State & state)
{
    std::forward_list<int> l(1024);
    for (auto _ : state) {
        benchmark::DoNotOptimize(l.front());
    }
}
BENCHMARK(BM_std_forward_list_front);

// iterator

static void BM_ww_forward_list_iterator(benchmark::State & state)
{
    wwstl::forward_list<int> l(1024);
    for (auto _ : state) {
        for (auto it = l.begin(); it != l.end(); ++it) {
            benchmark::DoNotOptimize(*it = 1);
        }
    }
}
BENCHMARK(BM_ww_forward_list_iterator);

static void BM_std_forward_list_iterator(benchmark::State & state)
{
    std::forward_list<int> l(1024);
    for (auto _ : state) {
        for (auto it = l.begin(); it != l.end(); ++it) {
            benchmark::DoNotOptimize(*it = 1);
        }
    }
}
BENCHMARK(BM_std_forward_list_iterator);

// clear

static void BM_ww_forward_list_clear(benchmark::State & state)
{
    for (auto _ : state) {
        wwstl::forward_list<int> l(1024);
        l.clear();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_forward_list_clear);

static void BM_std_forward_list_clear(benchmark::State & state)
{
    for (auto _ : state) {
        std::forward_list<int> l(1024);
        l.clear();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_forward_list_clear);

// insert_after

static void BM_ww_forward_list_insert_after(benchmark::State & state)
{
    wwstl::forward_list<int> l;
    for (auto _ : state) {
        l.insert_after(l.before_begin(), 1);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_forward_list_insert_after);

static void BM_std_forward_list_insert_after(benchmark::State & state)
{
    std::forward_list<int> l;
    for (auto _ : state) {
        l.insert_after(l.before_begin(), 1);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_forward_list_insert_after);

// erase_after

static void BM_ww_forward_list_erase_after(benchmark::State & state)
{
    for (auto _ : state) {
        wwstl::forward_list<int> l(1024);
        for (int i = 0; i < 1024; i++)
            l.erase_after(l.before_begin());
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_forward_list_erase_after);

static void BM_std_forward_list_erase_after(benchmark::State & state)
{
    for (auto _ : state) {
        std::forward_list<int> l(1024);
        for (int i = 0; i < 1024; i++)
            l.erase_after(l.before_begin());
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_forward_list_erase_after);

// push_front

static void BM_ww_forward_list_push_front(benchmark::State & state)
{
    wwstl::forward_list<int> l;
    for (auto _ : state) {
        l.push_front(1);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_forward_list_push_front);

static void BM_std_forward_list_push_front(benchmark::State & state)
{
    std::forward_list<int> l;
    for (auto _ : state) {
        l.push_front(1);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_forward_list_push_front);

// pop_front

static void BM_ww_forward_list_pop_front(benchmark::State & state)
{
    for (auto _ : state) {
        wwstl::forward_list<int> l(1024);
        for (int i = 0; i < 1024; i++)
            l.pop_front();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_forward_list_pop_front);

static void BM_std_forward_list_pop_front(benchmark::State & state)
{
    for (auto _ : state) {
        std::forward_list<int> l(1024);
        for (int i = 0; i < 1024; i++)
            l.pop_front();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_forward_list_pop_front);

// resize

static void BM_ww_forward_list_resize(benchmark::State & state)
{
    wwstl::forward_list<int> l(1024);
    for (auto _ : state) {
        l.resize(512);
        l.resize(1024, 2);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_forward_list_resize);

static void BM_std_forward_list_resize(benchmark::State & state)
{
    std::forward_list<int> l(1024);
    for (auto _ : state) {
        l.resize(512);
        l.resize(1024, 2);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_forward_list_resize);

// merge

static void BM_ww_forward_list_merge(benchmark::State & state)
{
    for (auto _ : state) {
        wwstl::forward_list<int> l1 = {1, 3, 5, 7, 9, 11, 13, 15};
        wwstl::forward_list<int> l2 = {2, 4, 6, 8, 10, 12, 14, 16};
        l1.merge(l2);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_forward_list_merge);

static void BM_std_forward_list_merge(benchmark::State & state)
{
    for (auto _ : state) {
        std::forward_list<int> l1 = {1, 3, 5, 7, 9, 11, 13, 15};
        std::forward_list<int> l2 = {2, 4, 6, 8, 10, 12, 14, 16};
        l1.merge(l2);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_forward_list_merge);

// splice

static void BM_ww_forward_list_splice(benchmark::State & state)
{
    for (auto _ : state) {
        wwstl::forward_list<int> l1 = {1, 3, 5, 7, 9, 11, 13, 15};
        wwstl::forward_list<int> l2 = {2, 4, 6, 8, 10, 12, 14, 16};
        l1.splice_after(l1.before_begin(), l2);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_forward_list_splice);

static void BM_std_forward_list_splice(benchmark::State & state)
{
    for (auto _ : state) {
        std::forward_list<int> l1 = {1, 3, 5, 7, 9, 11, 13, 15};
        std::forward_list<int> l2 = {2, 4, 6, 8, 10, 12, 14, 16};
        l1.splice_after(l1.before_begin(), l2);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_forward_list_splice);

// remove

static void BM_ww_forward_list_remove(benchmark::State & state)
{
    for (auto _ : state) {
        wwstl::forward_list<int> l = {1, 2, 3, 3, 3, 2, 2, 3, 3, 3, 4, 5, 5, 4, 2, 1};
        l.remove(3);
        l.remove_if([](int x) {
            return x % 2 == 0;
        });
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_forward_list_remove);

static void BM_std_forward_list_remove(benchmark::State & state)
{
    for (auto _ : state) {
        std::forward_list<int> l = {1, 2, 3, 3, 3, 2, 2, 3, 3, 3, 4, 5, 5, 4, 2, 1};
        l.remove(3);
        l.remove_if([](int x) {
            return x % 2 == 0;
        });
        benchmark::ClobberMemory();
    }
}

// reverse

static void BM_ww_forward_list_reverse(benchmark::State & state)
{
    for (auto _ : state) {
        wwstl::forward_list<int> l = {1, 2, 3, 3, 3, 2, 2, 3, 3, 3, 4, 5, 5, 4, 2, 1};
        l.reverse();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_forward_list_reverse);

static void BM_std_forward_list_reverse(benchmark::State & state)
{
    for (auto _ : state) {
        std::forward_list<int> l = {1, 2, 3, 3, 3, 2, 2, 3, 3, 3, 4, 5, 5, 4, 2, 1};
        l.reverse();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_forward_list_reverse);

// unique

static void BM_ww_forward_list_unique(benchmark::State & state)
{
    for (auto _ : state) {
        wwstl::forward_list<int> l = {1, 2, 3, 3, 3, 2, 2, 3, 3, 3, 4, 5, 5, 4, 2, 1};
        l.unique();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_forward_list_unique);

static void BM_std_forward_list_unique(benchmark::State & state)
{
    for (auto _ : state) {
        std::forward_list<int> l = {1, 2, 3, 3, 3, 2, 2, 3, 3, 3, 4, 5, 5, 4, 2, 1};
        l.unique();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_forward_list_unique);

// sort

static void BM_ww_forward_list_sort(benchmark::State & state)
{
    for (auto _ : state) {
        wwstl::forward_list<int> l = {1, 2, 3, 3, 3, 2, 2, 3, 3, 3, 4, 5, 5, 4, 2, 1};
        l.sort();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_forward_list_sort);

static void BM_std_forward_list_sort(benchmark::State & state)
{
    for (auto _ : state) {
        std::forward_list<int> l = {1, 2, 3, 3, 3, 2, 2, 3, 3, 3, 4, 5, 5, 4, 2, 1};
        l.sort();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_forward_list_sort);
