#include <benchmark/benchmark.h>
#include <list>
#include <ww_list.h>

// front

static void BM_ww_list_front(benchmark::State& state) {
    wwstl::list<int> l(1024);
    for (auto _ : state) {
        benchmark::DoNotOptimize(l.front());
    }
}
BENCHMARK(BM_ww_list_front);

static void BM_std_list_front(benchmark::State& state) {
    std::list<int> l(1024);
    for (auto _ : state) {
        benchmark::DoNotOptimize(l.front());
    }
}
BENCHMARK(BM_std_list_front);

// back

static void BM_ww_list_back(benchmark::State& state) {
    wwstl::list<int> l(1024);
    for (auto _ : state) {
        benchmark::DoNotOptimize(l.back());
    }
}
BENCHMARK(BM_ww_list_back);

static void BM_std_list_back(benchmark::State& state) {
    std::list<int> l(1024);
    for (auto _ : state) {
        benchmark::DoNotOptimize(l.back());
    }
}
BENCHMARK(BM_std_list_back);

// iterator

static void BM_ww_list_iterator(benchmark::State& state) {
    wwstl::list<int> l(1024);
    for (auto _ : state) {
        for (auto it = l.begin(); it != l.end(); ++it) {
            benchmark::DoNotOptimize(*it = 1);
        }
    }
}
BENCHMARK(BM_ww_list_iterator);

static void BM_std_list_iterator(benchmark::State& state) {
    std::list<int> l(1024);
    for (auto _ : state) {
        for (auto it = l.begin(); it != l.end(); ++it) {
            benchmark::DoNotOptimize(*it = 1);
        }
    }
}
BENCHMARK(BM_std_list_iterator);

// clear

static void BM_ww_list_clear(benchmark::State& state) {
    for (auto _ : state) {
        wwstl::list<int> l(1024);
        l.clear();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_list_clear);

static void BM_std_list_clear(benchmark::State& state) {
    for (auto _ : state) {
        std::list<int> l(1024);
        l.clear();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_list_clear);

// insert

static void BM_ww_list_insert(benchmark::State& state) {
    wwstl::list<int> l;
    for (auto _ : state) {
        l.insert(l.begin(), 10 ,1);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_list_insert);

static void BM_std_list_insert(benchmark::State& state) {
    std::list<int> l;
    for (auto _ : state) {
        l.insert(l.begin(), 10 ,1);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_list_insert);

// erase

static void BM_ww_list_erase(benchmark::State& state) {
    for (auto _ : state) {
        wwstl::list<int> l(1024);
        l.erase(l.begin(), l.end());
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_list_erase);

static void BM_std_list_erase(benchmark::State& state) {
    for (auto _ : state) {
        std::list<int> l(1024);
        l.erase(l.begin(), l.end());
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_list_erase);

// push_back

static void BM_ww_list_push_back(benchmark::State& state) {
    wwstl::list<int> l;
    for (auto _ : state) {
        l.push_back(1);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_list_push_back);

static void BM_std_list_push_back(benchmark::State& state) {
    std::list<int> l;
    for (auto _ : state) {
        l.push_back(1);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_list_push_back);

// pop_back

static void BM_ww_list_pop_back(benchmark::State& state) {
    for (auto _ : state) {
        wwstl::list<int> l(1024);
        for (int i = 0; i < 1024; i++)
            l.pop_back();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_list_pop_back);

static void BM_std_list_pop_back(benchmark::State& state) {
    for (auto _ : state) {
        std::list<int> l(1024);
        for (int i = 0; i < 1024; i++)
            l.pop_back();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_list_pop_back);

// push_front

static void BM_ww_list_push_front(benchmark::State& state) {
    wwstl::list<int> l;
    for (auto _ : state) {
        l.push_front(1);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_list_push_front);

static void BM_std_list_push_front(benchmark::State& state) {
    std::list<int> l;
    for (auto _ : state) {
        l.push_front(1);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_list_push_front);

// pop_front

static void BM_ww_list_pop_front(benchmark::State& state) {
    for (auto _ : state) {
        wwstl::list<int> l(1024);
        for (int i = 0; i < 1024; i++)
            l.pop_front();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_list_pop_front);

static void BM_std_list_pop_front(benchmark::State& state) {
    for (auto _ : state) {
        std::list<int> l(1024);
        for (int i = 0; i < 1024; i++)
            l.pop_front();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_list_pop_front);

// resize

static void BM_ww_list_resize(benchmark::State& state) {
    for (auto _ : state) {
        wwstl::list<int> l(1024);
        l.resize(512);
        l.resize(1024, 2);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_list_resize);

static void BM_std_list_resize(benchmark::State& state) {
    for (auto _ : state) {
        std::list<int> l(1024);
        l.resize(512);
        l.resize(1024, 2);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_list_resize);

// merge

static void BM_ww_list_merge(benchmark::State & state) {
    for (auto _ : state) {
        wwstl::list<int> l1 = {1, 3, 5, 7, 9, 11, 13, 15};
        wwstl::list<int> l2 = {2, 4, 6, 8, 10, 12, 14, 16};
        l1.merge(l2);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_list_merge);

static void BM_std_list_merge(benchmark::State & state) {
    for (auto _ : state) {
        std::list<int> l1 = {1, 3, 5, 7, 9, 11, 13, 15};
        std::list<int> l2 = {2, 4, 6, 8, 10, 12, 14, 16};
        l1.merge(l2);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_list_merge);

// splice

static void BM_ww_list_splice(benchmark::State & state) {
    for (auto _ : state) {
        wwstl::list<int> l1 = {1, 3, 5, 7, 9, 11, 13, 15};
        wwstl::list<int> l2 = {2, 4, 6, 8, 10, 12, 14, 16};
        l1.splice(l1.begin(), l2);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_list_splice);

static void BM_std_list_splice(benchmark::State & state) {
    for (auto _ : state) {
        std::list<int> l1 = {1, 3, 5, 7, 9, 11, 13, 15};
        std::list<int> l2 = {2, 4, 6, 8, 10, 12, 14, 16};
        l1.splice(l1.begin(), l2);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_list_splice);

// remove

static void BM_ww_list_remove(benchmark::State & state) {
    for (auto _ : state) {
        wwstl::list<int> l = {1, 2, 3, 3, 3, 2, 2, 3, 3, 3, 4, 5, 5, 4, 2, 1};
        l.remove(3);
        l.remove_if([](int x) {
            return x % 2 == 0;
        });
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_list_remove);

static void BM_std_list_remove(benchmark::State & state) {
    for (auto _ : state) {
        std::list<int> l = {1, 2, 3, 3, 3, 2, 2, 3, 3, 3, 4, 5, 5, 4, 2, 1};
        l.remove(3);
        l.remove_if([](int x) {
            return x % 2 == 0;
        });
        benchmark::ClobberMemory();
    }
}

// reverse

static void BM_ww_list_reverse(benchmark::State & state) {
    for (auto _ : state) {
        wwstl::list<int> l = {1, 2, 3, 3, 3, 2, 2, 3, 3, 3, 4, 5, 5, 4, 2, 1};
        l.reverse();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_list_reverse);

static void BM_std_list_reverse(benchmark::State & state) {
    for (auto _ : state) {
        std::list<int> l = {1, 2, 3, 3, 3, 2, 2, 3, 3, 3, 4, 5, 5, 4, 2, 1};
        l.reverse();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_list_reverse);

// unique

static void BM_ww_list_unique(benchmark::State & state) {
    for (auto _ : state) {
        wwstl::list<int> l = {1, 2, 3, 3, 3, 2, 2, 3, 3, 3, 4, 5, 5, 4, 2, 1};
        l.unique();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_list_unique);

static void BM_std_list_unique(benchmark::State & state) {
    for (auto _ : state) {
        std::list<int> l = {1, 2, 3, 3, 3, 2, 2, 3, 3, 3, 4, 5, 5, 4, 2, 1};
        l.unique();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_list_unique);

// sort

static void BM_ww_list_sort(benchmark::State & state) {
    for (auto _ : state) {
        wwstl::list<int> l = {1, 2, 3, 3, 3, 2, 2, 3, 3, 3, 4, 5, 5, 4, 2, 1};
        l.sort();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ww_list_sort);

static void BM_std_list_sort(benchmark::State & state) {
    for (auto _ : state) {
        std::list<int> l = {1, 2, 3, 3, 3, 2, 2, 3, 3, 3, 4, 5, 5, 4, 2, 1};
        l.sort();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_list_sort);

BENCHMARK_MAIN();
