# Advent of Code Template

My personal template repository that I use to run C++ code for Advent of Code

## Instructions for use:

1. Clone repository (or download zip), and remove current git repo from the directory.
2. Use `chmod` to enable execution on the file 'aoc' in the root of the directory.
3. Add the code to a new github repo for a given challenge.

## Usage

The usage of this repo is mainly that of the script `aoc`. 

### `./aoc -n` or `./aoc --new-challenge`

**Creates a new challenge with the name as the argument passed into the command.**

Since this script was made for Advent of Code, naming it with the format `DayX`, `DayXX`, etc. would reperesent the code for Day XX.

### `.aoc -h` or `./aoc --help`

**Runs the help screen.**

For a personal project, not necessary, but was to make it full-featured

### `./aoc`

**Run a challenge with the name as the argument passed into the command.**

If you run `./aoc XX`, where XX is a number, it will run the project titled `DayXX`. Otherwise, it will run the project of the same name.