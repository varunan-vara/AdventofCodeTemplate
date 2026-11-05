#include "../Utils/AoCUtils.h"
#include <iostream>

class Solution : public AoCUtils::Solution {
    public:
        // Do not change the following constructor
        Solution(AoCUtils::Challenge &c) : AoCUtils::Solution(c) {}

        std::string part1() override {
            AoCUtils::InputFile &challengeInput = challenge.input;

            return "Part 1 not implemented";
        }

        std::string part2() override {
            AoCUtils::InputFile &challengeInput = challenge.input;

            return "Part 2 not implemented";
        }
};

int main (int argc, char *argv[]) {
    // Lines below setup the solution and run - modify functions above for part 1 and 2 code
    AoCUtils::Challenge challenge = AoCUtils::getChallenge(argc, argv);
    Solution solution(challenge);
    return solution.run();
}
