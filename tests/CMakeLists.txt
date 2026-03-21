include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.17.0
)
FetchContent_MakeAvailable(googletest)


add_executable(dagpp_tests
        digraph_test.cpp
        mut_json_ext_test.cpp
        mut_basic_usage.cpp
        csr_digraph_test.cpp
        csr_wdigraph_test.cpp
        ext_dot_exporter_test.cpp
        dijkstra_test.cpp
        topo_sort_test.cpp
        global.h
        in_edges_acyclic_test.cpp
        json_ext_test.cpp
        basic_usage.cpp
)

target_link_libraries(dagpp_tests
        PRIVATE dagpp GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(dagpp_tests)

target_compile_options(dagpp_tests PRIVATE "-Wall" "-Wextra" "-Wpedantic")
