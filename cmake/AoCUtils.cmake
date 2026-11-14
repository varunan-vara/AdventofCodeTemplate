# AoCUtils CMake module: builds the shared library and exposes aoc_add_challenges().
# Include it from the CMakeLists.txt of the project holding the challenges:
#
#     include(<path to>/AoCUtils.cmake)
#     aoc_add_challenges(Challenges)

include_guard(GLOBAL)

# Root of the library (this file lives in <root>/cmake)
# (cached, so the functions below still see it when called from another directory scope)
get_filename_component(root ${CMAKE_CURRENT_LIST_DIR}/.. ABSOLUTE)
set(AOC_UTILS_ROOT ${root} CACHE INTERNAL "Root of the AoCUtils library")

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

# Fills <challenge_dir> with the files of <template_dir>, for a challenge called <name>
# 'TEMPLATE_NAME' is replaced by <name> in file names and contents. Files already there are kept.
# A template file called input.txt is created as <input_file> instead
function(aoc_scaffold_challenge name challenge_dir template_dir input_file)
    file(GLOB template_files ${template_dir}/*)
    foreach (template_file ${template_files})
        get_filename_component(file_name ${template_file} NAME)
        if (file_name STREQUAL "input.txt")
            set(file_name ${input_file})
        endif()
        string(REPLACE "TEMPLATE_NAME" ${name} file_name ${file_name})
        if (EXISTS ${challenge_dir}/${file_name})
            continue()
        endif()
        file(READ ${template_file} text)
        string(REPLACE "TEMPLATE_NAME" ${name} text "${text}")
        file(WRITE ${challenge_dir}/${file_name} "${text}")
    endforeach()
endfunction()

# Builds <challenges_dir>/<name>/<name>.cpp into an executable named <name>
# The binary lands in the build directory (./build/<name>), and is given the project root and
# challenges folder and input file name, so it finds its input no matter where it is run from
function(aoc_add_challenge name project_root challenges_dir input_file)
    set(source ${project_root}/${challenges_dir}/${name}/${name}.cpp)
    add_executable(${name} ${source})
    set_target_properties(${name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    target_link_libraries(${name} PRIVATE AoCUtilsLib)
    target_compile_definitions(${name} PRIVATE
        AOC_ROOT_DIR="${project_root}"
        AOC_CHALLENGES_DIR="${challenges_dir}"
        AOC_INPUT_FILE="${input_file}"
    )

    # 'cmake --build build --target run_<name>' builds and runs it, without arguments
    add_custom_target(run_${name} COMMAND ${name} USES_TERMINAL)

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
#  - INPUT_FILE <name> (default input.txt): name of each challenge's input file
#  - TEMPLATE <dir> (default: the library's Challenge_Template): folder of files copied into new
#    challenges, relative to the calling CMakeLists.txt (or absolute)
#  - AUTO_SCAFFOLD ON|OFF (default ON): a folder with no <folder>.cpp gets one, and the rest of
#    the template, generated from it. So 'mkdir Challenges/Day05' is enough to create a
#    challenge. OFF only warns about the folder and skips it
function(aoc_add_challenges challenges_dir)
    cmake_parse_arguments(ARG "" "INPUT_FILE;TEMPLATE;AUTO_SCAFFOLD" "" ${ARGN})
    if (ARG_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "aoc_add_challenges: unknown arguments: ${ARG_UNPARSED_ARGUMENTS}")
    endif()
    if (NOT DEFINED ARG_INPUT_FILE)
        set(ARG_INPUT_FILE input.txt)
    endif()
    if (NOT DEFINED ARG_AUTO_SCAFFOLD)
        set(ARG_AUTO_SCAFFOLD ON)
    endif()
    set(project_root ${CMAKE_CURRENT_SOURCE_DIR})
    get_filename_component(challenges_path ${challenges_dir} ABSOLUTE BASE_DIR ${project_root})
    file(RELATIVE_PATH challenges_rel ${project_root} ${challenges_path})

    if (DEFINED ARG_TEMPLATE)
        get_filename_component(template_dir ${ARG_TEMPLATE} ABSOLUTE BASE_DIR ${project_root})
        if (NOT IS_DIRECTORY ${template_dir})
            message(FATAL_ERROR "aoc_add_challenges: TEMPLATE is not a folder: ${template_dir}")
        endif()
    else()
        set(template_dir ${AOC_UTILS_ROOT}/Challenge_Template)
    endif()

    file(GLOB challenge_dirs CONFIGURE_DEPENDS LIST_DIRECTORIES true ${challenges_path}/*)

    foreach (dir_path ${challenge_dirs})
        # Loose files (e.g. README.md) are not challenges
        if (NOT IS_DIRECTORY ${dir_path})
            continue()
        endif()
        get_filename_component(dir ${dir_path} NAME)
        if (ARG_AUTO_SCAFFOLD AND NOT EXISTS ${dir_path}/${dir}.cpp)
            message(STATUS "Creating challenge from template: ${dir}")
            aoc_scaffold_challenge(${dir} ${dir_path} ${template_dir} ${ARG_INPUT_FILE})
        endif()
        if (EXISTS ${dir_path}/${dir}.cpp)
            message(STATUS "Compiling folder: ${dir}")
            aoc_add_challenge(${dir} ${project_root} ${challenges_rel} ${ARG_INPUT_FILE})
        else()
            message(WARNING "Skipping folder: ${dir}")
        endif()
    endforeach()
endfunction()
