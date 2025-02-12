set(BENCHMARK_FILES
    benchmark_main.cpp
    
    ww_array_benchmark.cpp
    ww_vector_benchmark.cpp
    ww_deque_benchmark.cpp
    ww_forward_list_benchmark.cpp
    ww_list_benchmark.cpp

    ww_set_benchmark.cpp
    ww_multiset_benchmark.cpp
    ww_map_benchmark.cpp
    ww_multimap_benchmark.cpp

    ww_unordered_set_benchmark.cpp
    ww_unordered_multiset_benchmark.cpp
    ww_unordered_map_benchmark.cpp
    ww_unordered_multimap_benchmark.cpp

    ww_stack_benchmark.cpp
    ww_queue_benchmark.cpp
    ww_priority_queue_benchmark.cpp
)

add_executable(benchmark_all ${BENCHMARK_FILES})

target_link_libraries(benchmark_all PRIVATE
    stl
    benchmark::benchmark
)