#include "../Utils/AoCUtils.h"
#include <iostream>

int main (int argc, char *argv[]) {
    // Do not change the following lines - this gets the challenge info and input
    AoCUtils::Challenge challenge = AoCUtils::getChallenge(argc, argv);
    AoCUtils::InputFile &challengeInput = challenge.input;

    std::cout << "Challenge code for " << challenge.name << std::endl;

    if (challenge.runPart(1)) {
        // Part 1
    }
    if (challenge.runPart(2)) {
        // Part 2
    }
}