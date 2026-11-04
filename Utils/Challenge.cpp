#include "Challenge.h"

namespace AoCUtils {
    Challenge getChallenge (int argc, char *argv[]) {
        std::string exePath = argv[0];
        std::string name = exePath.substr(exePath.find_last_of('/') + 1);
        std::string root = AOC_ROOT_DIR;
        std::string dir = root + "/Challenges/" + name;

        std::vector<std::string> args;
        bool visualize = false;
        int part = 0;

        for (int i = 1; i < argc; i++) {
            std::string arg = argv[i];
            std::string partValue;

            if (arg == "--visualize" || arg == "-v") {
                visualize = true;
            } else if (arg == "--part") {
                if (i + 1 >= argc) {
                    throw std::runtime_error("--part requires a value (1 or 2)");
                }
                partValue = argv[++i];
            } else if (arg.rfind("--part=", 0) == 0) {
                partValue = arg.substr(7);
            } else {
                args.push_back(arg);
                continue;
            }

            if (!partValue.empty() || arg.rfind("--part", 0) == 0) {
                if (partValue != "0" && partValue != "1" && partValue != "2") {
                    throw std::runtime_error("--part must be 0, 1 or 2, got: " + partValue);
                }
                part = std::stoi(partValue);
            }
        }

        std::string fileName = dir + "/input.txt";
        return Challenge{name, dir, fileName, root, args, visualize, part, InputFile(fileName)};
    }

    InputFile getFileInput (std::string fileName) {
        size_t last_slash = fileName.find_last_of('/');
        std::string day_name = fileName.substr(last_slash + 1);
        AoCUtils::InputFile challengeInput = AoCUtils::InputFile(std::string(AOC_ROOT_DIR) + "/Challenges/" + day_name + "/input.txt");
        return challengeInput;
    }
}
