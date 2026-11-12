#ifndef AOCUTILS_SOLUTION
#define AOCUTILS_SOLUTION

#include <iostream>
#include <string>
#include "Challenge.h"

namespace AoCUtils
{
    // Base class for a day's solution. Override part1 and part2, then call run.
    // Initialize by passing a Challenge object
    class Solution {
        public:
            // Optional: parse challenge.input into members, so part1 and part2 only solve.
            // Called once before the parts run (its time is printed), and benchmarked on its own
            // with --bench. It is called repeatedly when benchmarking, so it must reset any
            // members it fills (assign, don't append), and parts should not modify what it built
            //
            // Parameters: none
            virtual void parse() {}

            // Answer for part 1, as a string (use std::to_string for numbers)
            //
            // Parameters: none
            virtual std::string part1() { return "Part 1 not implemented"; }

            // Answer for part 2, as a string (use std::to_string for numbers)
            //
            // Parameters: none
            virtual std::string part2() { return "Part 2 not implemented"; }

            virtual ~Solution() = default;

            // Runs the parts selected by --part, printing each answer and its run time.
            // Sets visualize before each part
            //
            // Parameters: none
            int run();

            // Benchmarks parse, part1 and part2 (those selected by --part) with Google Benchmark.
            // Called by run when --bench is passed; --benchmark_* flags are forwarded
            // (ex. --benchmark_min_time=0.5s, --benchmark_filter=part1)
            //
            // Parameters: none
            int bench();
        protected:
            // The challenge being solved (input, args, name, etc.)
            Challenge &challenge;
            // True when the running part should visualize (push frames, update graphics)
            bool visualize = false;

        public:
            // Parameters:
            //  - c: challenge object from getChallenge
            Solution(Challenge &c) : challenge(c) {}
    };
} // namespace AoCUtils

#endif /*AOCUTILS_SOLUTION*/
