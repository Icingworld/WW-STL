#include <benchmark/benchmark.h>
#include <queue>
#include <ww_queue.h>

// front

static void BM_ww_queue_front(benchmark::State& state) {
    wwstl::queue<int> q;
    q.push(1);
    for (auto _ : state) {
        benchmark::DoNotOptimize(q.front());
    }
}
BENCHMARK(BM_ww_queue_front);

static void BM_std_queue_front(benchmark::State& state) {
    std::queue<int> q;
    q.push(1);
    for (auto _ : state) {
        benchmark::DoNotOptimize(q.front());
    }
}
BENCHMARK(BM_std_queue_front);

// back

static void BM_ww_queue_back(benchmark::State& state) {
    wwstl::queue<int> q;
    q.push(1);
    for (auto _ : state) {
        benchmark::DoNotOptimize(q.back());
    }
}
BENCHMARK(BM_ww_queue_back);

static void BM_std_queue_back(benchmark::State& state) {
    std::queue<int> q;
    q.push(1);
    for (auto _ : state) {
        benchmark::DoNotOptimize(q.back());
    }
}
BENCHMARK(BM_std_queue_back);

// push

static void BM_ww_queue_push(benchmark::State& state) {
    wwstl::queue<int> q;
    for (auto _ : state) {
        q.push(1);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_queue_push);

static void BM_std_queue_push(benchmark::State& state) {
    std::queue<int> q;
    for (auto _ : state) {
        q.push(1);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_queue_push);

// pop

static void BM_ww_queue_pop(benchmark::State& state) {
    for (auto _ : state) {
        wwstl::queue<int> q;
        for (int i = 0; i < 1024; i++)
            q.push(1);
        
        for (int i = 0; i < 1024; i++)
            q.pop();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_queue_pop);

static void BM_std_queue_pop(benchmark::State& state) {
    for (auto _ : state) {
        std::queue<int> q;
        for (int i = 0; i < 1024; i++)
            q.push(1);
        
        for (int i = 0; i < 1024; i++)
            q.pop();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_queue_pop);

BENCHMARK_MAIN();
