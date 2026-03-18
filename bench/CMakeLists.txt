include(FetchContent)
FetchContent_Declare(
        googlebenchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG v1.9.2
)

set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googlebenchmark)

add_executable(dagpp_bench bench.cpp)

target_link_libraries(dagpp_bench
        PRIVATE dagpp benchmark::benchmark
)
