# Note: The CMake build process is for test and benchmark only.
# For the purpose of using WW-STL, all your need is to include header files in your code.

cmake_minimum_required(VERSION 3.13)
project(ww-stl VERSION 1.0.0)

# set the C++ standard to C++14, as is required by googletest
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(stl)
add_subdirectory(third-party)

option(ENABLE_TEST "Enable Test" OFF)
option(ENABLE_BENCHMARK "Enable Benchmark" OFF)

if(ENABLE_TEST)
    add_subdirectory(test)
endif()

if(ENABLE_BENCHMARK)
    add_subdirectory(benchmark)
endif()
