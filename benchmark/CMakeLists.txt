set(BENCHMARK_FILES
    # ww_array_benchmark.cpp
    ww_vector_benchmark.cpp
)

add_executable(benchmark_all ${BENCHMARK_FILES})

target_link_libraries(benchmark_all PRIVATE
    stl
    benchmark::benchmark
)