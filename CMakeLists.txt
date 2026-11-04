cmake_minimum_required(VERSION 3.10)
project(AdventOfCode LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Binaries land directly in the build directory (./build/<name>), which is where
# the 'aoc' script looks for them
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Shared utilities, linked into every challenge
add_library(AoCUtilsLib STATIC ${CMAKE_CURRENT_SOURCE_DIR}/Utils/AoCUtils.cpp)
target_include_directories(AoCUtilsLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Utils)

# Builds Challenges/<name>/<name>.cpp into an executable named <name>
function(add_challenge name)
    add_executable(${name} ${CMAKE_CURRENT_SOURCE_DIR}/Challenges/${name}/${name}.cpp)
    target_link_libraries(${name} PRIVATE AoCUtilsLib)
endfunction()

# Every folder in Challenges/ containing a <folder>.cpp is a challenge, not just 'DayX' ones
# CONFIGURE_DEPENDS re-globs on every build, so new challenges are picked up automatically
file(GLOB CHALLENGE_DIRS CONFIGURE_DEPENDS LIST_DIRECTORIES true
    ${CMAKE_CURRENT_SOURCE_DIR}/Challenges/*
)

foreach (DIR_PATH ${CHALLENGE_DIRS})
    get_filename_component(DIR ${DIR_PATH} NAME)
    if (IS_DIRECTORY ${DIR_PATH} AND EXISTS ${DIR_PATH}/${DIR}.cpp)
        message (STATUS "Compiling folder: ${DIR}")
        add_challenge(${DIR})
    else()
        message (WARNING "Skipping folder: ${DIR}")
    endif()
endforeach()
