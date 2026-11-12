# Sample project using the AoCUtils library, for testing it. A real project would fetch the
# library instead of pointing at the parent folder:
#     FetchContent_Declare(AoCUtils GIT_REPOSITORY <library repo> GIT_TAG <version>)
cmake_minimum_required(VERSION 3.19)
project(AoCExample LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optimized by default: unoptimized builds make run times and benchmarks meaningless
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Already there when this folder is built as part of the library's own CMakeLists.txt
if (NOT TARGET AoCUtilsLib)
    include(FetchContent)
    FetchContent_Declare(AoCUtils SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
    FetchContent_MakeAvailable(AoCUtils)
endif()

aoc_add_challenges(Challenges)
