# Advent of Code Template

My personal template repository that I use to run C++ code for Advent of Code

## Instructions for use (generated using AI):

1. Use this repository as a GitHub template (or clone it and delete the `.git` folder), then create a new git repo for the year.
2. Use `chmod +x aoc` to enable execution on the file 'aoc' in the root of the directory.
3. Requires `cmake` and a C++17 compiler.
4. Create your first challenge with `./aoc -n 1`, paste your puzzle input into `Challenges/Day01/input.txt`, and run it with `./aoc 1`.

## Layout

 - `aoc`: script to create and run challenges. It can be run from any directory.
 - `Challenge_Template/`: files copied into every new challenge. `TEMPLATE_NAME` in a file name is replaced with the challenge name.
 - `Challenges/<name>/`: one folder per challenge, containing `<name>.cpp` and `input.txt`. Any folder with a matching `<name>.cpp` is built automatically by the root `CMakeLists.txt`, no per-challenge CMake file needed.
 - `Utils/`: shared library (`InputFile`, `Challenge`, `Solution`) linked into every challenge.

`input.txt` files in `Challenges/` are git-ignored, since Advent of Code asks that puzzle inputs not be shared.

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

For example, `./aoc 5 --part 2 -v` runs only part 2 of `Day05` with visualization on. An invalid `--part` value (anything other than 0, 1 or 2) stops with an error.
