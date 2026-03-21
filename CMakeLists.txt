cmake_minimum_required(VERSION 3.31)
project(dagpp)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(DAGPP_INCLUDES
        include/digraph.h
        include/dot.h
        include/topo_sort.h
        include/dagpp.h
)

option(DAGPP_ENABLE_LTO "Enable LTO" ON)
option(DAGPP_BUILD_NATIVE "Enable native build" ON)
option(DAGPP_ENABLE_AVX10 "Enable AVX10.1 instructions (if not building native)" ON)
option(DAGPP_ENABLE_AVX2 "Enable AVX2 instructions (if not building native)" ON)
option(DAGPP_STRIP_DEAD "Enable dead code stripping" ON)
option(DAGPP_HIDDEN_VISIBILITY "Hide symbols" ON)
option(DAGPP_ENABLE_NO_EXCEPT "Enable -fno-exception flag." ON)
option(DAGPP_ENABLE_NO_RTTI "Enable -fno-rtti flag." ON)
option(DAGPP_ENABLE_FAST_MATH "Enable -ffast-math optimizations. Not recommended" OFF)


find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    message(STATUS "Using CCache: ${CCACHE_PROGRAM}")
    set(CMAKE_C_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
else ()
    message(STATUS "No ccache found on the machine.")
endif()

add_library(dagpp INTERFACE
        include/dijkstra.h)
target_sources(dagpp INTERFACE
        FILE_SET HEADERS
        BASE_DIRS include
        FILES
        include/digraph.h
        include/dot.h
        include/topo_sort.h
        include/dagpp.h
        include/helpers.h
        include/usings.h
        include/csr.h
)
target_include_directories(dagpp INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

if(UNIX AND NOT APPLE)
    if(CMAKE_BUILD_TYPE MATCHES Debug)
        add_compile_options("-gsplit-dwarf")
    endif()
    find_program(MOLD_LINKER mold)
    if(MOLD_LINKER)
        message(STATUS "Using Mold Linker")
        add_link_options("-fuse-ld=mold")
    else()
        find_program(LLD_LINKER lld)
        if(LLD_LINKER)
            message(STATUS "Using LLD Linker")
            add_link_options("-fuse-ld=lld")
        endif()
    endif()
endif()

include(CheckCXXCompilerFlag)
include(CheckIPOSupported)

if(CMAKE_BUILD_TYPE MATCHES Release)
    if(MSVC)
        add_compile_options("/O2")
    else()
        add_compile_options("-O3" "-fno-math-errno" "-fomit-frame-pointer")
    endif()
    if(DAGPP_ENABLE_NO_EXCEPT)
        check_cxx_compiler_flag("-fno-exceptions" COMPILER_HAS_NO_EXCEPTIONS)
        if(COMPILER_HAS_NO_EXCEPTIONS)
            add_compile_options("-fno-exceptions")
        endif ()
    endif()
    if(DAGPP_ENABLE_NO_RTTI)
        check_cxx_compiler_flag("-fno-rtti" COMPILER_HAS_NO_RTTI)
        if(COMPILER_HAS_NO_RTTI)
            add_compile_options("-fno-rtti")
        endif ()
    endif ()
    if(DAGPP_HIDDEN_VISIBILITY)
        set(CMAKE_CXX_VISIBILITY_PRESET hidden)
        set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
        message(STATUS "Hidden symbol visibility enabled.")
    endif()
    if(DAGPP_STRIP_DEAD AND UNIX AND NOT APPLE)
        check_cxx_compiler_flag("-ffunction-sections" COMPILER_HAS_FUNCTION_SECTIONS)
        if(COMPILER_HAS_FUNCTION_SECTIONS)
            add_compile_options("-ffunction-sections" "-fdata-sections")
            add_link_options("-Wl,--gc-sections")
            message(STATUS "Dead code stripping enabled.")
        endif()
    endif()
    if(DAGPP_ENABLE_LTO)
        check_ipo_supported(RESULT lto_supported OUTPUT lto_error)
        if(lto_supported)
            set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
        else()
            message(WARNING "LTO is requested but not supported")
        endif()
    endif()

    if(DAGPP_BUILD_NATIVE)
        if(NOT MSVC)
            check_cxx_compiler_flag("-march=native" COMPILER_HAS_MARCH_NATIVE)
            if(COMPILER_HAS_MARCH_NATIVE)
                add_compile_options("-march=native")
            endif()
        endif()
    else()
        if(DAGPP_ENABLE_AVX10)
            if(MSVC)
                set(FLAG_AVX10 "/arch:AVX10.1")
            else()
                set(FLAG_AVX10 "-mavx10.1")
            endif()
            check_cxx_compiler_flag("${FLAG_AVX10}" COMPILER_HAS_AVX10)
            if(COMPILER_HAS_AVX10)
                add_compile_options("${FLAG_AVX10}")
            endif()
        elseif(DAGPP_ENABLE_AVX2)
            if(MSVC)
                set(FLAG_AVX2 "/arch:AVX2")
            else()
                set(FLAG_AVX2 "-mavx2")
            endif()
            check_cxx_compiler_flag("${FLAG_AVX2}" COMPILER_HAS_AVX2)
            if(COMPILER_HAS_AVX2)
                add_compile_options("${FLAG_AVX2}")
            endif()
        endif()
    endif()
    if(DAGPP_ENABLE_FAST_MATH)
        if(MSVC)
            set(FLAG_FAST_MATH "/fp:fast")
        else()
            set(FLAG_FAST_MATH "-ffast-math")
        endif()
        check_cxx_compiler_flag("${FLAG_FAST_MATH}" COMPILER_HAS_FAST_MATH)
        if(COMPILER_HAS_FAST_MATH)
            message(WARNING "${FLAG_FAST_MATH} flag is set")
            add_compile_options("${FLAG_FAST_MATH}")
        endif()
    endif()
endif()

enable_testing()
add_subdirectory(tests)
add_subdirectory(bench)
