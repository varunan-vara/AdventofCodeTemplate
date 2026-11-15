# Integration test for cmake/AoCUtils.cmake: builds throwaway consumer projects that use the
# library through FetchContent, the way a year's repo would. Run by CTest as
#     cmake -DAOC_SRC=<library> -DWORK=<scratch dir> [-DBENCHMARK_SRC=<benchmark checkout>] -P Integration.cmake

cmake_minimum_required(VERSION 3.19)

file(REMOVE_RECURSE ${WORK})
file(MAKE_DIRECTORY ${WORK})

# Reusing the main build's Benchmark checkout keeps this offline and faster
set(extra_args "")
if (BENCHMARK_SRC)
    list(APPEND extra_args -DFETCHCONTENT_SOURCE_DIR_BENCHMARK=${BENCHMARK_SRC})
endif()

function(fail message)
    message(FATAL_ERROR "Integration test failed: ${message}")
endfunction()

function(run_checked description)
    execute_process(COMMAND ${ARGN} RESULT_VARIABLE result OUTPUT_VARIABLE out ERROR_VARIABLE err)
    if (NOT result EQUAL 0)
        fail("${description} (exit ${result})\n${out}\n${err}")
    endif()
    set(last_output "${out}${err}" PARENT_SCOPE)
endfunction()

# Makes a consumer project in ${WORK}/<name> whose aoc_add_challenges call has the given options
function(make_project name options)
    set(dir ${WORK}/${name})
    file(MAKE_DIRECTORY ${dir}/Challenges)
    file(WRITE ${dir}/CMakeLists.txt "
cmake_minimum_required(VERSION 3.19)
project(Consumer LANGUAGES CXX)
include(FetchContent)
FetchContent_Declare(AoCUtils SOURCE_DIR ${AOC_SRC})
FetchContent_MakeAvailable(AoCUtils)
aoc_add_challenges(Challenges ${options})
")
endfunction()

function(configure name)
    run_checked("configure ${name}" ${CMAKE_COMMAND} -S ${WORK}/${name} -B ${WORK}/${name}/build
        -DCMAKE_BUILD_TYPE=Release ${extra_args})
endfunction()

# Case 1: defaults. An empty folder becomes a runnable challenge; the binary finds its input from anywhere
make_project(defaults "")
file(MAKE_DIRECTORY ${WORK}/defaults/Challenges/Day05)
configure(defaults)
foreach (file Day05.cpp input.txt)
    if (NOT EXISTS ${WORK}/defaults/Challenges/Day05/${file})
        fail("defaults: Day05/${file} was not scaffolded")
    endif()
endforeach()
file(READ ${WORK}/defaults/Challenges/Day05/Day05.cpp scaffolded)
if (scaffolded MATCHES "TEMPLATE_NAME")
    fail("defaults: TEMPLATE_NAME left in the scaffolded file")
endif()
run_checked("build run_Day05" ${CMAKE_COMMAND} --build ${WORK}/defaults/build --target run_Day05)
if (NOT last_output MATCHES "Day05 part 1")
    fail("defaults: run_Day05 did not run the challenge:\n${last_output}")
endif()
run_checked("run binary from another directory" ${CMAKE_COMMAND} -E chdir / ${WORK}/defaults/build/Day05 --part 2)
if (NOT last_output MATCHES "Day05 part 2" OR last_output MATCHES "Day05 part 1")
    fail("defaults: --part 2 output wrong:\n${last_output}")
endif()
# Scaffolding never overwrites existing files: with the .cpp gone it is regenerated, input.txt kept
file(WRITE ${WORK}/defaults/Challenges/Day05/input.txt "mine\n")
file(REMOVE ${WORK}/defaults/Challenges/Day05/Day05.cpp)
configure(defaults)
file(READ ${WORK}/defaults/Challenges/Day05/input.txt kept)
if (NOT kept STREQUAL "mine\n")
    fail("defaults: reconfigure overwrote input.txt")
endif()
if (NOT EXISTS ${WORK}/defaults/Challenges/Day05/Day05.cpp)
    fail("defaults: Day05.cpp was not regenerated")
endif()

# Case 2: INPUT_FILE and TEMPLATE
make_project(custom "INPUT_FILE puzzle.txt TEMPLATE tpl")
file(MAKE_DIRECTORY ${WORK}/custom/tpl ${WORK}/custom/Challenges/Day03)
file(READ ${AOC_SRC}/Challenge_Template/TEMPLATE_NAME.cpp template_source)
file(WRITE ${WORK}/custom/tpl/TEMPLATE_NAME.cpp "// custom template for TEMPLATE_NAME\n${template_source}")
file(WRITE ${WORK}/custom/tpl/input.txt "template input\n")
configure(custom)
if (NOT EXISTS ${WORK}/custom/Challenges/Day03/puzzle.txt OR EXISTS ${WORK}/custom/Challenges/Day03/input.txt)
    fail("custom: input file should be puzzle.txt, not input.txt")
endif()
file(READ ${WORK}/custom/Challenges/Day03/Day03.cpp scaffolded)
if (NOT scaffolded MATCHES "custom template for Day03")
    fail("custom: did not use the custom template")
endif()
run_checked("build custom" ${CMAKE_COMMAND} --build ${WORK}/custom/build --target run_Day03)
if (NOT last_output MATCHES "Day03 part 1")
    fail("custom: run_Day03 did not run the challenge:\n${last_output}")
endif()

# Case 3: AUTO_SCAFFOLD OFF leaves empty folders alone, but still builds finished ones
make_project(noscaffold "AUTO_SCAFFOLD OFF")
file(MAKE_DIRECTORY ${WORK}/noscaffold/Challenges/Empty ${WORK}/noscaffold/Challenges/Day01)
file(WRITE ${WORK}/noscaffold/Challenges/Day01/Day01.cpp "int main() { return 0; }\n")
configure(noscaffold)
file(GLOB created ${WORK}/noscaffold/Challenges/Empty/*)
if (created)
    fail("noscaffold: Empty was scaffolded: ${created}")
endif()
run_checked("build noscaffold" ${CMAKE_COMMAND} --build ${WORK}/noscaffold/build --target Day01)

# Case 4: unknown options are an error
make_project(badoption "NOT_AN_OPTION x")
execute_process(COMMAND ${CMAKE_COMMAND} -S ${WORK}/badoption -B ${WORK}/badoption/build ${extra_args}
    RESULT_VARIABLE result OUTPUT_QUIET ERROR_QUIET)
if (result EQUAL 0)
    fail("badoption: configure should have failed")
endif()

message(STATUS "Integration tests passed")
