file(GLOB BENCHMARK_FILES "*.cpp")

add_executable(benchmark_all ${BENCHMARK_FILES})

target_link_libraries(benchmark_all PRIVATE
    stl
    benchmark::benchmark
)