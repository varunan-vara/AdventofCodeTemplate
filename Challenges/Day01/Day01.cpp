#include "../Utils/AoCUtils.h"
#include <iostream>

int main (int argc, char *argv[]) {
    // Do not change the following lines - this gets the FileInput type
    std::string fileName = argv[0];
    AoCUtils::InputFile challengeInput = AoCUtils::getFileInput(fileName);

    std::cout << "Challenge code for " << argv[0] << std::endl;
}