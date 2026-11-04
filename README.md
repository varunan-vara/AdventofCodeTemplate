# Advent of Code Template

My personal template repository that I use to run C++ code for Advent of Code

## Instructions for use:

1. Use this repository as a GitHub template (or clone it and delete the `.git` folder), then create a new git repo for the year.
2. Use `chmod +x aoc` to enable execution on the file 'aoc' in the root of the directory.
3. Requires `cmake` and a C++17 compiler.
4. Create your first challenge with `./aoc -n 1`, paste your puzzle input into `Challenges/Day01/input.txt`, and run it with `./aoc 1`.

## Layout

 - `aoc`: script to create and run challenges. It can be run from any directory.
 - `Challenge_Template/`: files copied into every new challenge. `TEMPLATE_NAME` in a file name is replaced with the challenge name.
 - `Challenges/<name>/`: one folder per challenge, containing `<name>.cpp` and `input.txt`. Any folder with a matching `<name>.cpp` is built automatically by the root `CMakeLists.txt`, no per-challenge CMake file needed.
 - `Utils/`: `AoCUtils`, shared helpers (such as `InputFile`) linked into every challenge.

`input.txt` files in `Challenges/` are git-ignored, since Advent of Code asks that puzzle inputs not be shared.

## Usage

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
| `-v` or `--visualize` | Sets `challenge.visualize`, for challenges that can draw or simulate. |
| anything else | Available in order as `challenge.args`. |

For example, `./aoc 5 --part 2 -v` runs only part 2 of `Day05` with visualization on. An invalid `--part` value (anything other than 0, 1 or 2) stops with an error.

## Challenge info in `main`

Each challenge's `main` calls `AoCUtils::getChallenge(argc, argv)`, which returns a `Challenge` with:

 - `name`: the challenge name, such as `Day01`
 - `dir`: the challenge's source folder
 - `fileName`: the full path of the input file being read
 - `rootDir`: the repo root
 - `args`: the extra arguments described above
 - `visualize`: whether `-v` or `--visualize` was passed
 - `part`: `0` for both parts, or `1` or `2`
 - `input`: the parsed `InputFile`

`challenge.runPart(1)` and `challenge.runPart(2)` tell you which parts to run, so you can wrap each part in `if (challenge.runPart(N)) { ... }`. The template already does this.
