#ifndef AOCUTILS_CHALLENGE
#define AOCUTILS_CHALLENGE

#include <string>
#include <vector>
#include "InputFile.h"

namespace AoCUtils
{
    // Challenge class is built by running 'getChallenge' - contains relevant details required for
    // running and visualizing a challenge
    struct Challenge {
        // Challenge name, ex. "Day01"
        std::string name;
        // Challenge source folder
        std::string dir;
        // Full path of the input file
        std::string fileName;
        // Repo root
        std::string rootDir;
        // Arguments that are not recognized flags, in order
        std::vector<std::string> args;
        // True if --visualize or -v was passed
        bool visualize;
        // Part to run (--part): 1 or 2, or 0 for both
        int part;
        // True if --bench was passed
        bool bench;
        // --benchmark_* flags, forwarded to Google Benchmark
        std::vector<std::string> benchArgs;
        // Parsed contents of fileName
        InputFile input;

        // True if the given part should run
        //
        // Parameters:
        //  - p: part number, 1 or 2
        bool runPart(int p) const { return part == 0 || part == p; }
    };

    // Builds a Challenge from main's arguments. Flags: --visualize/-v, --part <n>, --bench
    // (--benchmark_* flags go to benchArgs); everything else goes in args. Throws std::runtime_error on a bad --part value.
    // Prints an error and exits if the challenge's input.txt does not exist
    //
    // Parameters:
    //  - argc: argument count from main
    //  - argv: arguments from main (argv[0] gives the challenge name)
    //  - rootDir: project root, which contains challengesDir
    //  - challengesDir: folder (relative to rootDir) holding one folder per challenge
    Challenge getChallenge (int argc, char *argv[], const std::string &rootDir, const std::string &challengesDir);

    // Returns the InputFile of a challenge, found as <rootDir>/<challengesDir>/<name>/input.txt
    // (name is the last component of fileName)
    //
    // Parameters:
    //  - fileName: path or name of the challenge, ex. argv[0]
    //  - rootDir: project root, which contains challengesDir
    //  - challengesDir: folder (relative to rootDir) holding one folder per challenge
    InputFile getFileInput (std::string fileName, const std::string &rootDir, const std::string &challengesDir);

    // AOC_ROOT_DIR and AOC_CHALLENGES_DIR are compile definitions that aoc_add_challenges sets on
    // each challenge, so these resolve to the project using the library, not the library itself
#if defined(AOC_ROOT_DIR) && defined(AOC_CHALLENGES_DIR)
    inline Challenge getChallenge (int argc, char *argv[]) {
        return getChallenge(argc, argv, AOC_ROOT_DIR, AOC_CHALLENGES_DIR);
    }

    inline InputFile getFileInput (std::string fileName) {
        return getFileInput(fileName, AOC_ROOT_DIR, AOC_CHALLENGES_DIR);
    }
#endif
} // namespace AoCUtils

#endif /*AOCUTILS_CHALLENGE*/
