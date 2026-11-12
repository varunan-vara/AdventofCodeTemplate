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
endif()
