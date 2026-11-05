#include "Solution.h"

#include <chrono>
#include <iostream>

namespace AoCUtils {
    int Solution::run() {
        for (int part = 1; part <= 2; part++) {
            if (!challenge.runPart(part)) continue;

            // --visualize with no --part: part 1 runs normally, part 2 visualizes
            visualize = challenge.visualize && (challenge.part != 0 || part == 2);
            auto start = std::chrono::steady_clock::now();
            std::string answer = (part == 1) ? part1() : part2();
            auto elapsed = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - start);

            std::cout << challenge.name << " part " << part << ": " << answer
                      << " (" << elapsed.count() << " ms)" << std::endl;
        }

        return 0;
    }
}
