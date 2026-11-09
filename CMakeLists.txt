cmake_minimum_required(VERSION 3.10)
project(AdventOfCode LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Binaries land directly in the build directory (./build/<name>), which is where
# the 'aoc' script looks for them
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Shared utilities, linked into every challenge
add_library(AoCUtilsLib STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Utils/InputFile.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Utils/Challenge.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Utils/Solution.cpp
)
target_include_directories(AoCUtilsLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Utils)
# Lets getFileInput find Challenges/<name>/input.txt no matter where the binary is run from
target_compile_definitions(AoCUtilsLib PRIVATE AOC_ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

# Matplot++ is optional: only fetched/built if a challenge includes <matplot/...>
# Needs gnuplot at runtime (brew install gnuplot)
function(require_matplot)
    if (TARGET Matplot++::matplot)
        return()
    endif()
    message(STATUS "Matplot++ required, fetching it")
    include(FetchContent)
    FetchContent_Declare(matplotplusplus
        GIT_REPOSITORY https://github.com/alandefreitas/matplotplusplus
        GIT_TAG origin/master
    )
    FetchContent_MakeAvailable(matplotplusplus)
endfunction()

# Builds Challenges/<name>/<name>.cpp into an executable named <name>
function(add_challenge name)
    set(source ${CMAKE_CURRENT_SOURCE_DIR}/Challenges/${name}/${name}.cpp)
    add_executable(${name} ${source})
    target_link_libraries(${name} PRIVATE AoCUtilsLib)

    file(READ ${source} source_text)
    if (source_text MATCHES "#include[ \t]*[<\"]matplot/")
        require_matplot()
        target_link_libraries(${name} PRIVATE Matplot++::matplot)
    endif()
    # Re-run CMake when the source changes, so adding/removing the include is picked up
    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${source})
endfunction()

# Every folder in Challenges/ containing a <folder>.cpp is a challenge, not just 'DayX' ones
# CONFIGURE_DEPENDS re-globs on every build, so new challenges are picked up automatically
file(GLOB CHALLENGE_DIRS CONFIGURE_DEPENDS LIST_DIRECTORIES true
    ${CMAKE_CURRENT_SOURCE_DIR}/Challenges/*
)

foreach (DIR_PATH ${CHALLENGE_DIRS})
    # Loose files (e.g. README.md) are not challenges
    if (NOT IS_DIRECTORY ${DIR_PATH})
        continue()
    endif()
    get_filename_component(DIR ${DIR_PATH} NAME)
    if (EXISTS ${DIR_PATH}/${DIR}.cpp)
        message (STATUS "Compiling folder: ${DIR}")
        add_challenge(${DIR})
    else()
        message (WARNING "Skipping folder: ${DIR}")
    endif()
endforeach()
