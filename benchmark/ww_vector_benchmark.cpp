#include <benchmark/benchmark.h>
#include <vector>
#include <ww_vector.h>

// operator[]

static void BM_ww_vector(benchmark::State & state)
{
    wwstl::vector<int> vec(1024);
    for (auto _ : state) {
        benchmark::DoNotOptimize(vec[512] = 1);
    }
}
BENCHMARK(BM_ww_vector);

static void BM_std_vector(benchmark::State & state)
{
    std::vector<int> vec(1024);
    for (auto _ : state) {
        benchmark::DoNotOptimize(vec[512] = 1);
    }
}
BENCHMARK(BM_std_vector);

// at

static void BM_ww_vector_at(benchmark::State & state)
{
    wwstl::vector<int> vec(1024);
    for (auto _ : state) {
        benchmark::DoNotOptimize(vec.at(512) = 1);
    }
}
BENCHMARK(BM_ww_vector_at);

static void BM_std_vector_at(benchmark::State & state)
{
    std::vector<int> vec(1024);
    for (auto _ : state) {
        benchmark::DoNotOptimize(vec.at(512) = 1);
    }
}
BENCHMARK(BM_std_vector_at);

// iterator

static void BM_ww_vector_iterator(benchmark::State & state)
{
    wwstl::vector<int> vec(1024);
    for (auto _ : state) {
        for (auto it = vec.begin(); it != vec.end(); ++it) {
            benchmark::DoNotOptimize(*it = 1);
        }
    }
}
BENCHMARK(BM_ww_vector_iterator);

static void BM_std_vector_iterator(benchmark::State & state)
{
    std::vector<int> vec(1024);
    for (auto _ : state) {
        for (auto it = vec.begin(); it != vec.end(); ++it) {
            benchmark::DoNotOptimize(*it = 1);
        }
    }
}
BENCHMARK(BM_std_vector_iterator);

// clear

static void BM_ww_vector_clear(benchmark::State & state)
{
    for (auto _ : state) {
        wwstl::vector<int> vec(1024);
        vec.clear();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_vector_clear);

static void BM_std_vector_clear(benchmark::State & state)
{
    for (auto _ : state) {
        std::vector<int> vec(1024);
        vec.clear();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_vector_clear);

// insert

static void BM_ww_vector_insert(benchmark::State & state)
{
    wwstl::vector<int> vec;
    for (auto _ : state) {
        vec.insert(vec.begin(), 10, 1);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_vector_insert);

static void BM_std_vector_insert(benchmark::State & state)
{
    std::vector<int> vec;
    for (auto _ : state) {
        vec.insert(vec.begin(), 10, 1);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_vector_insert);

// erase

static void BM_ww_vector_erase(benchmark::State & state)
{
    wwstl::vector<int> vec(1024);
    for (auto _ : state) {
        vec.erase(vec.begin(), vec.end());
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_vector_erase);

static void BM_std_vector_erase(benchmark::State & state)
{
    std::vector<int> vec(1024);
    for (auto _ : state) {
        vec.erase(vec.begin(), vec.end());
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_vector_erase);

// push_back

static void BM_ww_vector_push_back(benchmark::State & state)
{
    wwstl::vector<int> vec;
    for (auto _ : state) {
        vec.push_back(1);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_vector_push_back);

static void BM_std_vector_push_back(benchmark::State & state)
{
    std::vector<int> vec;
    for (auto _ : state) {
        vec.push_back(1);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_vector_push_back);

// pop_back

static void BM_ww_vector_pop_back(benchmark::State & state)
{
    for (auto _ : state) {
        wwstl::vector<int> vec(1024);
        for (int i = 0; i < 1024; i++)
            vec.pop_back();
        benchmark::DoNotOptimize(vec);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_vector_pop_back);

static void BM_std_vector_pop_back(benchmark::State & state)
{
    for (auto _ : state) {
        std::vector<int> vec(1024);
        for (int i = 0; i < 1024; i++)
            vec.pop_back();
        benchmark::DoNotOptimize(vec);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_vector_pop_back);

// resize

static void BM_ww_vector_resize(benchmark::State & state)
{
    wwstl::vector<int> vec(1024);
    for (auto _ : state) {
        vec.resize(512);
        vec.resize(1024, 2);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_vector_resize);

static void BM_std_vector_resize(benchmark::State & state)
{
    std::vector<int> vec(1024);
    for (auto _ : state) {
        vec.resize(512);
        vec.resize(1024, 2);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_vector_resize);
