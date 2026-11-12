# Advent of Code Template

My personal template repository that I use to run C++ code for Advent of Code

## Instructions for use (generated using AI):

1. Use this repository as a GitHub template (or clone it and delete the `.git` folder), then create a new git repo for the year.
2. Use `chmod +x aoc` to enable execution on the file 'aoc' in the root of the directory.
3. Requires `cmake` and a C++17 compiler.
4. The repo is being turned into a library (see `LIBRARY_PLAN.md`), and the sample project that uses it is in `Example/`. Create your first challenge with `Example/aoc -n 1`, paste your puzzle input into `Example/Challenges/Day01/input.txt`, and run it with `Example/aoc 1`. The commands below are written as `./aoc`, run from `Example/`.

## Layout

 - `aoc`: script to create and run challenges. It can be run from any directory. It works on the project given by `AOC_PROJECT_DIR` (the folder with `Challenges/` and `build/`), or the folder it is in.
 - `Example/`: sample project that uses the library, for testing it. Has its own `CMakeLists.txt` and an `aoc` stub that runs the library's script on `Example/`. Built too when the root `CMakeLists.txt` is the top-level project (`-DAOC_BUILD_EXAMPLE=OFF` to skip).
 - `Challenge_Template/`: files copied into every new challenge. `TEMPLATE_NAME` in a file name is replaced with the challenge name.
 - `Example/Challenges/<name>/`: one folder per challenge, containing `<name>.cpp` and `input.txt`. Any folder with a matching `<name>.cpp` is built automatically by `aoc_add_challenges`, no per-challenge CMake file needed.
 - `Utils/`: shared library (`InputFile`, `Challenge`, `Solution`) linked into every challenge. Headers are included as `<AoCUtils/AoCUtils.h>`.
 - `cmake/AoCUtils.cmake`: builds the library and defines `aoc_add_challenges(<dir>)`, which the root `CMakeLists.txt` calls. Another project can `include()` it and call the same function.

`input.txt` files in any `Challenges/` folder are git-ignored, since Advent of Code asks that puzzle inputs not be shared.

## Usage (generated using AI)

The usage of this repo is mainly that of the script `aoc`. 

### `./aoc -n` or `./aoc --new-challenge`

**Creates a new challenge with the name as the argument passed into the command.**

Since this script was made for Advent of Code, naming it with the format `DayX`, `DayXX`, etc. would reperesent the code for Day XX.

### `./aoc -h` or `./aoc --help`

**Runs the help screen.**

For a personal project, not necessary, but was to make it full-featured

### `./aoc`

**Run a challenge with the name as the argument passed into the command.**

If you run `./aoc XX`, where XX is a number, it will run the project titled `DayXX`. Otherwise, it will run the project of the same name.

Anything after the challenge name is passed to the challenge itself:

| Argument | Effect |
|---|---|
| `--part <N>` or `--part=<N>` | Run only part `N` (1 or 2). Both parts run by default (`0`). |
| `-v` or `--visualize` | Sets the `visualize` flag for the part being run. With `--part N`, part `N` visualizes. Without `--part`, part 1 runs normally and part 2 visualizes. |
| anything else | Available in order as `challenge.args`. |

| `--bench` | Benchmarks `parse`, `part1` and `part2` (those selected by `--part`) with Google Benchmark, instead of running once. `--benchmark_*` flags are forwarded, ex. `./aoc 5 --bench --benchmark_min_time=0.5s --benchmark_filter=part1`. |

Builds default to `Release`, so run times and benchmarks are optimized. Override with `cmake -B build -DCMAKE_BUILD_TYPE=Debug`.

`parse()` is an optional override on `Solution`: fill members from `challenge.input` there, so the parts only solve. It runs once before the parts, and is benchmarked on its own. Since `--bench` calls it repeatedly, assign members rather than appending to them.

For example, `./aoc 5 --part 2 -v` runs only part 2 of `Day05` with visualization on. An invalid `--part` value (anything other than 0, 1 or 2) stops with an error.
