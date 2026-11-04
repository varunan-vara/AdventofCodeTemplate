#ifndef AOCUTILS_CHALLENGE
#define AOCUTILS_CHALLENGE

#include <string>
#include <vector>
#include "InputFile.h"

namespace AoCUtils
{
    // Everything a challenge's main() needs, built by getChallenge from argc/argv
    struct Challenge {
        std::string name;                // Challenge name, ex. "Day01" (not "./build/Day01")
        std::string dir;                 // Challenge source folder, ex. "<root>/Challenges/Day01"
        std::string fileName;            // Full path of the input file being read
        std::string rootDir;             // Repo root
        std::vector<std::string> args;   // Arguments that are not recognized flags, in order
        bool visualize;                  // --visualize or -v was passed
        int part;                        // --part <n>: 1 or 2 to run one part, 0 (default) for both
        InputFile input;                 // Parsed contents of fileName

        // True if the given part (1 or 2) should run
        bool runPart(int p) const { return part == 0 || part == p; }
    };

    // Builds a Challenge from main's arguments. Recognized flags:
    //  --visualize, -v:   sets visualize
    //  --part <n>, --part=<n>: sets part (0, 1 or 2)
    // Everything else ends up in args. Throws std::runtime_error on a bad --part value
    Challenge getChallenge (int argc, char *argv[]);

    // Function that returns a basic InputFile object, based on the ./aoc cli tool's format
    InputFile getFileInput (std::string fileName);
} // namespace AoCUtils

#endif /*AOCUTILS_CHALLENGE*/
