#include <benchmark/benchmark.h>
#include <queue>
#include <ww_priority_queue.h>

// top

static void BM_ww_priority_queue_top(benchmark::State & state)
{
    wwstl::priority_queue<int> pq;
    pq.push(1);
    for (auto _ : state) {
        // 由于top()返回const引用，无法使用已经弃用的DoNotOptimize(const T&)版本，这里改为传值
        int value = pq.top();
        benchmark::DoNotOptimize(value);
    }
}
BENCHMARK(BM_ww_priority_queue_top);

static void BM_std_priority_queue_top(benchmark::State & state)
{
    std::priority_queue<int> pq;
    pq.push(1);
    for (auto _ : state) {
        int value = pq.top();
        benchmark::DoNotOptimize(value);
    }
}
BENCHMARK(BM_std_priority_queue_top);

// push

static void BM_ww_priority_queue_push(benchmark::State & state)
{
    wwstl::priority_queue<int> pq;
    for (auto _ : state) {
        pq.push(1);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_priority_queue_push);

static void BM_std_priority_queue_push(benchmark::State & state)
{
    std::priority_queue<int> pq;
    for (auto _ : state) {
        pq.push(1);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_priority_queue_push);

// pop

static void BM_ww_priority_queue_pop(benchmark::State & state)
{
    for (auto _ : state) {
        wwstl::priority_queue<int> pq;
        for (int i = 0; i < 1024; i++)
            pq.push(i);

        for (int i = 0; i < 1024; i++)
            pq.pop();

        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_priority_queue_pop);

static void BM_std_priority_queue_pop(benchmark::State & state)
{
    for (auto _ : state) {
        std::priority_queue<int> pq;
        for (int i = 0; i < 1024; i++)
            pq.push(i);

        for (int i = 0; i < 1024; i++)
            pq.pop();

        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_priority_queue_pop);
