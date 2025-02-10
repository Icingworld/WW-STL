#include <benchmark/benchmark.h>
#include <stack>
#include <ww_stack.h>

// top

static void BM_ww_stack_top(benchmark::State& state) {
    wwstl::stack<int> s;
    s.push(1);
    for (auto _ : state) {
        benchmark::DoNotOptimize(s.top());
    }
}
BENCHMARK(BM_ww_stack_top);

static void BM_std_stack_top(benchmark::State& state) {
    std::stack<int> s;
    s.push(1);
    for (auto _ : state) {
        benchmark::DoNotOptimize(s.top());
    }
}
BENCHMARK(BM_std_stack_top);

// push

static void BM_ww_stack_push(benchmark::State& state) {
    wwstl::stack<int> s;
    for (auto _ : state) {
        s.push(1);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_stack_push);

static void BM_std_stack_push(benchmark::State& state) {
    std::stack<int> s;
    for (auto _ : state) {
        s.push(1);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_stack_push);

// pop

static void BM_ww_stack_pop(benchmark::State& state) {
    for (auto _ : state) {
        wwstl::stack<int> s;
        for (int i = 0; i < 1024; i++)
            s.push(1);
        
        for (int i = 0; i < 1024; i++)
            s.pop();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_stack_pop);

static void BM_std_stack_pop(benchmark::State& state) {
    for (auto _ : state) {
        std::stack<int> s;
        for (int i = 0; i < 1024; i++)
            s.push(1);
        
        for (int i = 0; i < 1024; i++)
            s.pop();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_stack_pop);

BENCHMARK_MAIN();
