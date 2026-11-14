# AoCUtils

A C++17 library for running Advent of Code challenges. A year's repo holds only its solutions
and pulls this library in as a dependency, so there is nothing to clone or copy.
`Example/` is a sample project using it: copy it to start a year.

Requires `cmake` (3.19+) and a C++17 compiler. The first configure needs network access, since
the library and Google Benchmark are fetched with `FetchContent`.

## Using it

A year's repo looks like this:

```
MyYear/
  CMakeLists.txt
  Challenges/
    Day01/Day01.cpp, input.txt
    ...
```

```cmake
cmake_minimum_required(VERSION 3.19)
project(AoC2026 LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(AoCUtils GIT_REPOSITORY <this repo> GIT_TAG <version>)
FetchContent_MakeAvailable(AoCUtils)

# Optimized by default: unoptimized builds make run times and benchmarks meaningless
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

aoc_add_challenges(Challenges)
```

Pin `GIT_TAG` to keep a year building against the version it was written for.

### Creating a challenge

Make a folder in `Challenges/` and reconfigure (or just build):

```
mkdir Challenges/Day05
cmake -B build
```

`Day05.cpp` and `input.txt` are generated from `Challenge_Template/`, where `TEMPLATE_NAME` in a
file name or its contents becomes the folder name. Paste your puzzle input into `input.txt`.
Inputs are git-ignored by the sample project, since Advent of Code asks that they not be shared.

Any folder containing a matching `<name>.cpp` is built automatically, no per-challenge CMake file
needed, and the name doesn't have to be `DayXX`.

### Building and running

```
cmake --build build --target Day05       # build
./build/Day05                            # run, from anywhere
cmake --build build --target run_Day05   # build and run, without arguments
```

Every challenge is also a target in an IDE such as CLion.

### Arguments

Arguments go to the binary, ex. `./build/Day05 --part 2 -v`:

| Argument | Effect |
|---|---|
| `--part <N>` or `--part=<N>` | Run only part `N` (1 or 2). Both parts run by default (`0`). Anything else is an error. |
| `-v` or `--visualize` | Sets the `visualize` flag for the part being run. With `--part N`, part `N` visualizes. Without `--part`, part 1 runs normally and part 2 visualizes. |
| `--bench` | Benchmarks `parse`, `part1` and `part2` (those selected by `--part`) with Google Benchmark, instead of running once. `--benchmark_*` flags are forwarded, ex. `--bench --benchmark_min_time=0.5s --benchmark_filter=part1`. |
| anything else | Available in order as `challenge.args`. |

`parse()` is an optional override on `Solution`: fill members from `challenge.input` there, so
the parts only solve. It runs once before the parts, and is benchmarked on its own. Since
`--bench` calls it repeatedly, assign members rather than appending to them.

### Options

`aoc_add_challenges(<dir> ...)` takes:

| Option | Default | Effect |
|---|---|---|
| `INPUT_FILE <name>` | `input.txt` | Name of each challenge's input file. |
| `TEMPLATE <dir>` | the library's `Challenge_Template/` | Folder copied into new challenges, relative to your `CMakeLists.txt`. |
| `AUTO_SCAFFOLD ON\|OFF` | `ON` | Generate the files of any folder in `<dir>` that has no `<name>.cpp`. When `OFF`, such folders are skipped with a warning. |

### Matplot++

Optional. If a challenge includes `<matplot/...>`, Matplot++ is fetched and linked for it only.
It needs `gnuplot` at runtime (`brew install gnuplot`).

## Layout

 - `Utils/`: the library (`InputFile`, `Challenge`, `Solution`), linked into every challenge. Headers are included as `<AoCUtils/AoCUtils.h>`.
 - `cmake/AoCUtils.cmake`: builds the library and defines `aoc_add_challenges`.
 - `Challenge_Template/`: files copied into every new challenge.
 - `Example/`: sample project using the library. Built too when this repo is the top-level project, ex. opened in an IDE (`-DAOC_BUILD_EXAMPLE=OFF` to skip).
 - `LIBRARY_PLAN.md`: notes from turning the repo into a library.
