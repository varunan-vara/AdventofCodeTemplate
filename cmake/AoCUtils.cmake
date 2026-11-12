# AoCUtils CMake module: builds the shared library and exposes aoc_add_challenges().
# Include it from the CMakeLists.txt of the project holding the challenges:
#
#     include(<path to>/AoCUtils.cmake)
#     aoc_add_challenges(Challenges)

include_guard(GLOBAL)

# Root of the library (this file lives in <root>/cmake)
get_filename_component(AOC_UTILS_ROOT ${CMAKE_CURRENT_LIST_DIR}/.. ABSOLUTE)

include(FetchContent)

# Google Benchmark, used by '<challenge> --bench' (see Solution::bench)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_WERROR OFF CACHE BOOL "" FORCE)
FetchContent_Declare(benchmark
    GIT_REPOSITORY https://github.com/google/benchmark
    GIT_TAG v1.9.1
)
FetchContent_MakeAvailable(benchmark)

# Shared utilities, linked into every challenge. Includes are <AoCUtils/...>
add_library(AoCUtilsLib STATIC
    ${AOC_UTILS_ROOT}/Utils/InputFile.cpp
    ${AOC_UTILS_ROOT}/Utils/Challenge.cpp
    ${AOC_UTILS_ROOT}/Utils/Solution.cpp
)
target_include_directories(AoCUtilsLib PUBLIC ${AOC_UTILS_ROOT}/Utils)
target_compile_features(AoCUtilsLib PUBLIC cxx_std_17)
target_link_libraries(AoCUtilsLib PUBLIC benchmark::benchmark)

# Matplot++ is optional: only fetched/built if a challenge includes <matplot/...>
# Needs gnuplot at runtime (brew install gnuplot)
function(aoc_require_matplot)
    if (TARGET Matplot++::matplot)
        return()
    endif()
    message(STATUS "Matplot++ required, fetching it")
    FetchContent_Declare(matplotplusplus
        GIT_REPOSITORY https://github.com/alandefreitas/matplotplusplus
        GIT_TAG origin/master
    )
    FetchContent_MakeAvailable(matplotplusplus)
endfunction()

# Builds <challenges_dir>/<name>/<name>.cpp into an executable named <name>
# The binary lands in the build directory (./build/<name>), and is given the project root and
# challenges folder, so it finds its input.txt no matter where it is run from
function(aoc_add_challenge name project_root challenges_dir)
    set(source ${project_root}/${challenges_dir}/${name}/${name}.cpp)
    add_executable(${name} ${source})
    set_target_properties(${name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    target_link_libraries(${name} PRIVATE AoCUtilsLib)
    target_compile_definitions(${name} PRIVATE
        AOC_ROOT_DIR="${project_root}"
        AOC_CHALLENGES_DIR="${challenges_dir}"
    )

    file(READ ${source} source_text)
    if (source_text MATCHES "#include[ \t]*[<\"]matplot/")
        aoc_require_matplot()
        target_link_libraries(${name} PRIVATE Matplot++::matplot)
    endif()
    # Re-run CMake when the source changes, so adding/removing the include is picked up
    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${source})
endfunction()

# Every folder in <challenges_dir> containing a <folder>.cpp is a challenge, not just 'DayX' ones
# CONFIGURE_DEPENDS re-globs on every build, so new challenges are picked up automatically
#
# Parameters:
#  - challenges_dir: folder holding one folder per challenge, relative to the calling
#    CMakeLists.txt (or absolute, inside it). That CMakeLists.txt's folder is the project root
function(aoc_add_challenges challenges_dir)
    set(project_root ${CMAKE_CURRENT_SOURCE_DIR})
    get_filename_component(challenges_path ${challenges_dir} ABSOLUTE BASE_DIR ${project_root})
    file(RELATIVE_PATH challenges_rel ${project_root} ${challenges_path})

    file(GLOB challenge_dirs CONFIGURE_DEPENDS LIST_DIRECTORIES true ${challenges_path}/*)

    foreach (dir_path ${challenge_dirs})
        # Loose files (e.g. README.md) are not challenges
        if (NOT IS_DIRECTORY ${dir_path})
            continue()
        endif()
        get_filename_component(dir ${dir_path} NAME)
        if (EXISTS ${dir_path}/${dir}.cpp)
            message(STATUS "Compiling folder: ${dir}")
            aoc_add_challenge(${dir} ${project_root} ${challenges_rel})
        else()
            message(WARNING "Skipping folder: ${dir}")
        endif()
    endforeach()
endfunction()
