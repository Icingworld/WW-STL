#include <benchmark/benchmark.h>
#include <deque>
#include <ww_deque.h>

// access

static void BM_ww_deque_access(benchmark::State & state)
{
    wwstl::deque<int> l(1024);
    for (auto _ : state) {
        benchmark::DoNotOptimize(l[512]);
    }
}
BENCHMARK(BM_ww_deque_access);

static void BM_std_deque_access(benchmark::State & state)
{
    std::deque<int> l(1024);
    for (auto _ : state) {
        benchmark::DoNotOptimize(l[512]);
    }
}
BENCHMARK(BM_std_deque_access);

// at

static void BM_ww_deque_at(benchmark::State & state)
{
    wwstl::deque<int> l(1024);
    for (auto _ : state) {
        benchmark::DoNotOptimize(l.at(512));
    }
}
BENCHMARK(BM_ww_deque_at);

static void BM_std_deque_at(benchmark::State & state)
{
    std::deque<int> l(1024);
    for (auto _ : state) {
        benchmark::DoNotOptimize(l.at(512));
    }
}
BENCHMARK(BM_std_deque_at);

// front

static void BM_ww_deque_front(benchmark::State & state)
{
    wwstl::deque<int> l(1024);
    for (auto _ : state) {
        benchmark::DoNotOptimize(l.front());
    }
}
BENCHMARK(BM_ww_deque_front);

static void BM_std_deque_front(benchmark::State & state)
{
    std::deque<int> l(1024);
    for (auto _ : state) {
        benchmark::DoNotOptimize(l.front());
    }
}
BENCHMARK(BM_std_deque_front);

// back

static void BM_ww_deque_back(benchmark::State & state)
{
    wwstl::deque<int> l(1024);
    for (auto _ : state) {
        benchmark::DoNotOptimize(l.back());
    }
}
BENCHMARK(BM_ww_deque_back);

static void BM_std_deque_back(benchmark::State & state)
{
    std::deque<int> l(1024);
    for (auto _ : state) {
        benchmark::DoNotOptimize(l.back());
    }
}
BENCHMARK(BM_std_deque_back);

// iterator

static void BM_ww_deque_iterator(benchmark::State & state)
{
    wwstl::deque<int> l(1024);
    for (auto _ : state) {
        for (auto it = l.begin(); it != l.end(); ++it) {
            benchmark::DoNotOptimize(*it);
        }
    }
}
BENCHMARK(BM_ww_deque_iterator);

static void BM_std_deque_iterator(benchmark::State & state)
{
    std::deque<int> l(1024);
    for (auto _ : state) {
        for (auto it = l.begin(); it != l.end(); ++it) {
            benchmark::DoNotOptimize(*it);
        }
    }
}
BENCHMARK(BM_std_deque_iterator);

// clear

static void BM_ww_deque_clear(benchmark::State & state)
{
    for (auto _ : state) {
        wwstl::deque<int> l(1024);
        l.clear();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_deque_clear);

static void BM_std_deque_clear(benchmark::State & state)
{
    for (auto _ : state) {
        std::deque<int> l(1024);
        l.clear();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_deque_clear);

// insert

static void BM_ww_deque_insert(benchmark::State & state)
{
    wwstl::deque<int> l;
    for (auto _ : state) {
        l.insert(l.begin(), 10, 1);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_deque_insert);

static void BM_std_deque_insert(benchmark::State & state)
{
    std::deque<int> l;
    for (auto _ : state) {
        l.insert(l.begin(), 10, 1);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_deque_insert);

// erase

static void BM_ww_deque_erase(benchmark::State & state)
{
    for (auto _ : state) {
        wwstl::deque<int> l(1024);
        l.erase(l.begin(), l.end());
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_deque_erase);

static void BM_std_deque_erase(benchmark::State & state)
{
    for (auto _ : state) {
        std::deque<int> l(1024);
        l.erase(l.begin(), l.end());
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_deque_erase);

// push_back

static void BM_ww_deque_push_back(benchmark::State & state)
{
    wwstl::deque<int> l;
    for (auto _ : state) {
        l.push_back(1);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_deque_push_back);

static void BM_std_deque_push_back(benchmark::State & state)
{
    std::deque<int> l;
    for (auto _ : state) {
        l.push_back(1);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_deque_push_back);

// pop_back

static void BM_ww_deque_pop_back(benchmark::State & state)
{
    for (auto _ : state) {
        wwstl::deque<int> l(1024);
        for (int i = 0; i < 1024; i++)
            l.pop_back();
        
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_deque_pop_back);

static void BM_std_deque_pop_back(benchmark::State & state)
{
    for (auto _ : state) {
        std::deque<int> l(1024);
        for (int i = 0; i < 1024; i++)
            l.pop_back();
        
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_deque_pop_back);

// push_front

static void BM_ww_deque_push_front(benchmark::State & state)
{
    wwstl::deque<int> l;
    for (auto _ : state) {
        l.push_front(1);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_deque_push_front);

static void BM_std_deque_push_front(benchmark::State & state)
{
    std::deque<int> l;
    for (auto _ : state) {
        l.push_front(1);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_deque_push_front);

// pop_front

static void BM_ww_deque_pop_front(benchmark::State & state)
{
    for (auto _ : state) {
        wwstl::deque<int> l(1024);
        for (int i = 0; i < 1024; i++)
            l.pop_front();
        
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_deque_pop_front);

static void BM_std_deque_pop_front(benchmark::State & state)
{
    for (auto _ : state) {
        std::deque<int> l(1024);
        for (int i = 0; i < 1024; i++)
            l.pop_front();
        
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_deque_pop_front);

// resize

static void BM_ww_deque_resize(benchmark::State & state)
{
    wwstl::deque<int> l(1024);
    for (auto _ : state) {
        l.resize(512);
        l.resize(1024, 2);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_deque_resize);

static void BM_std_deque_resize(benchmark::State & state)
{
    std::deque<int> l(1024);
    for (auto _ : state) {
        l.resize(512);
        l.resize(1024, 2);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_deque_resize);

BENCHMARK_MAIN();
