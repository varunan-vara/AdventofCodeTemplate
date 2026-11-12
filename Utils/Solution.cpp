#include "AoCUtils/Solution.h"

#include <benchmark/benchmark.h>
#include <chrono>
#include <iostream>
#include <vector>

namespace AoCUtils {
    int Solution::run() {
        if (challenge.bench) return bench();

        auto parseStart = std::chrono::steady_clock::now();
        parse();
        auto parseElapsed = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - parseStart);
        std::cout << challenge.name << " parse: (" << parseElapsed.count() << " ms)" << std::endl;

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

    int Solution::bench() {
        // Visualizing would draw on every iteration
        visualize = false;

        benchmark::RegisterBenchmark(challenge.name + "/parse", [this](benchmark::State &state) {
            for (auto _ : state) {
                parse();
            }
        });

        for (int part = 1; part <= 2; part++) {
            if (!challenge.runPart(part)) continue;

            benchmark::RegisterBenchmark(challenge.name + "/part" + std::to_string(part),
                [this, part](benchmark::State &state) {
                    for (auto _ : state) {
                        std::string answer = (part == 1) ? part1() : part2();
                        benchmark::DoNotOptimize(answer);
                    }
                });
        }

        // parse is run once up front, so the parts start with their parsed data
        parse();

        // Google Benchmark rejects flags it does not know, so only pass it its own
        std::vector<std::string> flags = {challenge.name};
        flags.insert(flags.end(), challenge.benchArgs.begin(), challenge.benchArgs.end());
        std::vector<char *> argv;
        for (std::string &flag : flags) argv.push_back(flag.data());
        int argc = static_cast<int>(argv.size());

        benchmark::Initialize(&argc, argv.data());
        if (benchmark::ReportUnrecognizedArguments(argc, argv.data())) return 1;
        benchmark::RunSpecifiedBenchmarks();
        benchmark::Shutdown();

        return 0;
    }
}
