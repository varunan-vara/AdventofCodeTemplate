#include <AoCUtils/AoCUtils.h>
#include <iostream>

class Solution : public AoCUtils::Solution {
    public:
        // Do not change the following constructor
        Solution(AoCUtils::Challenge &c) : AoCUtils::Solution(c) {}

        // Optional: parse the input into members here, so part1 and part2 only solve (and
        // --bench can time parsing separately). Assign members rather than appending to them
        // void parse() override {
        //     AoCUtils::InputFile &challengeInput = challenge.input;
        // }

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
