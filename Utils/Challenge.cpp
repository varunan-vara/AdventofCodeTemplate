#include "AoCUtils/Challenge.h"

#include <cstdlib>
#include <fstream>
#include <iostream>

namespace AoCUtils {
    Challenge getChallenge (int argc, char *argv[], const std::string &rootDir, const std::string &challengesDir, const std::string &inputFile) {
        std::string exePath = argv[0];
        std::string name = exePath.substr(exePath.find_last_of('/') + 1);
        std::string root = rootDir;
        std::string dir = root + "/" + challengesDir + "/" + name;

        std::vector<std::string> args;
        std::vector<std::string> benchArgs;
        bool visualize = false;
        bool bench = false;
        int part = 0;

        for (int i = 1; i < argc; i++) {
            std::string arg = argv[i];
            std::string partValue;

            if (arg == "--visualize" || arg == "-v") {
                visualize = true;
            } else if (arg == "--bench") {
                bench = true;
            } else if (arg.rfind("--benchmark_", 0) == 0) {
                benchArgs.push_back(arg);
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

        std::string fileName = dir + "/" + inputFile;

        if (!std::ifstream(fileName).good()) {
            std::cerr << "\033[31mInput file not found: " << fileName << "\033[0m\n"
                      << "Create a new txt file, and paste input contents there. .gitignored by default" << std::endl;
            std::exit(1);
        }

        return Challenge{name, dir, fileName, root, args, visualize, part, bench, benchArgs, InputFile(fileName)};
    }

    InputFile getFileInput (std::string fileName, const std::string &rootDir, const std::string &challengesDir, const std::string &inputFile) {
        size_t last_slash = fileName.find_last_of('/');
        std::string day_name = fileName.substr(last_slash + 1);
        AoCUtils::InputFile challengeInput = AoCUtils::InputFile(rootDir + "/" + challengesDir + "/" + day_name + "/" + inputFile);
        return challengeInput;
    }
}
