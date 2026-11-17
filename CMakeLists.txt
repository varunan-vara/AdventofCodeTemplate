# AoCUtils: library for running Advent of Code challenges. Use it from another project with
# FetchContent_MakeAvailable (or add_subdirectory), then call aoc_add_challenges(<dir>).
# See Example/ for a project doing that
cmake_minimum_required(VERSION 3.19)
project(AoCUtils LANGUAGES CXX)

# Library setup and aoc_add_challenges live in cmake/AoCUtils.cmake
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/AoCUtils.cmake)

# When this is the top-level project (ex. opened in an IDE), also build the sample project
if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    option(AOC_BUILD_EXAMPLE "Build the sample project in Example/" ON)
    if (AOC_BUILD_EXAMPLE)
        add_subdirectory(Example)
    endif()

    # Tests: 'ctest --test-dir build'. Unit tests are quick. The integration test builds
    # throwaway consumer projects (see tests/Integration.cmake), so it is opt-in
    option(AOC_BUILD_TESTS "Build the unit tests" ON)
    option(AOC_INTEGRATION_TESTS "Add the CMake integration test (slower)" OFF)
    option(AOC_MATPLOT_TESTS "Also test Plot.h in the integration test (builds Matplot++)" OFF)
    if (AOC_BUILD_TESTS)
        enable_testing()
        add_executable(UnitTests tests/UnitTests.cpp)
        target_link_libraries(UnitTests PRIVATE AoCUtilsLib)
        add_test(NAME UnitTests COMMAND UnitTests)

        if (AOC_INTEGRATION_TESTS)
            add_test(NAME Integration COMMAND ${CMAKE_COMMAND}
                -DAOC_SRC=${CMAKE_CURRENT_SOURCE_DIR}
                -DWORK=${CMAKE_BINARY_DIR}/integration
                -DBENCHMARK_SRC=${benchmark_SOURCE_DIR}
                -DMATPLOT=${AOC_MATPLOT_TESTS}
                -P ${CMAKE_CURRENT_SOURCE_DIR}/tests/Integration.cmake)
        endif()
    endif()
endif()
