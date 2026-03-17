cmake_minimum_required(VERSION 3.31)
project(dagpp)

set(CMAKE_CXX_STANDARD 23)

set(DAGPP_INCLUDES
        include/digraph.h
        include/ext.h
)
set(DAGPP_SRC
        src/digraph.cpp
        src/ext.cpp
)

add_library(dagpp STATIC
        ${DAGPP_INCLUDES}
        ${DAGPP_SRC}
)

enable_testing()
add_subdirectory(tests)
