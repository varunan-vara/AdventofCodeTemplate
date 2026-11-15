// Unit tests for AoCUtilsLib. No framework: CHECK records a failure, main exits non-zero if any
#include <AoCUtils/AoCUtils.h>

#include <filesystem>
#include <iostream>
#include <random>
#include <sstream>

namespace fs = std::filesystem;

static int failures = 0;

#define CHECK(cond) \
    do { \
        if (!(cond)) { \
            std::cerr << __FILE__ << ":" << __LINE__ << ": CHECK failed: " #cond << std::endl; \
            failures++; \
        } \
    } while (0)

#define CHECK_THROWS(expr) \
    do { \
        bool threw = false; \
        try { expr; } catch (const std::exception &) { threw = true; } \
        CHECK(threw); \
    } while (0)

// Temp project root holding <challengesDir>/<name>/<inputFile>, removed when destroyed
struct Fixture {
    fs::path root;

    Fixture() {
        root = fs::temp_directory_path() / ("aoc_unit_test_" + std::to_string(std::random_device{}()));
        fs::create_directories(root);
    }
    ~Fixture() { fs::remove_all(root); }

    fs::path write(const std::string &relPath, const std::string &contents) {
        fs::path path = root / relPath;
        fs::create_directories(path.parent_path());
        std::ofstream(path) << contents;
        return path;
    }
};

static void testInputFile() {
    Fixture fx;
    std::string path = fx.write("in.txt", "abc\ndef\nghi\n").string();
    AoCUtils::InputFile input(path);

    CHECK(input.numberOfLines() == 3);
    CHECK(input.numberOfChars() == 3);
    CHECK(input.getLine(1) == "def");
    CHECK(input.getFullString() == "abcdefghi");
    CHECK(input.getParsedInput().size() == 3);

    auto grid = input.get2DArray();
    CHECK(grid.size() == 3 && grid[2][0] == 'g');

    // x is the column, y the row
    CHECK(input.getGridValue(2, 0) == 'c');
    input.setGridValue(2, 0, 'X');
    CHECK(input.getGridValue(2, 0) == 'X');
    CHECK(input.getLine(0) == "abX");
    CHECK_THROWS(input.setGridValue(0, 0, '\0'));

    CHECK_THROWS(AoCUtils::InputFile(fx.root.string() + "/missing.txt"));
}

static void testDelimited() {
    Fixture fx;
    std::string path = fx.write("in.txt", "1,2,3\n4,,5\n").string();
    AoCUtils::InputFile input(path);

    auto strings = input.getDelimited2DArray(',');
    CHECK(strings.size() == 2);
    CHECK((strings[0] == std::vector<std::string>{"1", "2", "3"}));
    CHECK((strings[1] == std::vector<std::string>{"4", "", "5"}));

    auto ints = input.getDelimited2DIntArray(',');
    CHECK((ints[0] == std::vector<int>{1, 2, 3}));
    // Empty entries are skipped
    CHECK((ints[1] == std::vector<int>{4, 5}));

    CHECK((AoCUtils::InputFile::getDelimited2DArray("a b c", ' ') == std::vector<std::string>{"a", "b", "c"}));
    auto fromVector = AoCUtils::InputFile::getDelimited2DArray(std::vector<std::string>{"a-b", "c-d"}, '-');
    CHECK(fromVector.size() == 2 && fromVector[1][0] == "c");

    // Regex form: the capture groups are the values
    std::string patternPath = fx.write("pattern.txt", "move 12 from 3\nmove 4 from 56\n").string();
    AoCUtils::InputFile patterned(patternPath);
    auto captured = patterned.getDelimited2DIntArray("move (\\d+) from (\\d+)");
    CHECK((captured[0] == std::vector<int>{12, 3}));
    CHECK((captured[1] == std::vector<int>{4, 56}));
}

// Builds a Challenge the way a challenge's main would, from a list of arguments
static AoCUtils::Challenge makeChallenge(const Fixture &fx, std::vector<std::string> args,
                                         const std::string &inputFile = "input.txt") {
    // argv[0] gives the challenge name, as the binary built in the build directory would have
    args.insert(args.begin(), (fx.root / "build" / "Day01").string());
    std::vector<char *> argv;
    for (std::string &arg : args) argv.push_back(arg.data());
    return AoCUtils::getChallenge(static_cast<int>(argv.size()), argv.data(), fx.root.string(), "Challenges", inputFile);
}

static void testGetChallenge() {
    Fixture fx;
    fx.write("Challenges/Day01/input.txt", "line\n");

    AoCUtils::Challenge defaults = makeChallenge(fx, {});
    CHECK(defaults.name == "Day01");
    CHECK(defaults.dir == fx.root.string() + "/Challenges/Day01");
    CHECK(defaults.fileName == defaults.dir + "/input.txt");
    CHECK(defaults.rootDir == fx.root.string());
    CHECK(defaults.part == 0 && defaults.runPart(1) && defaults.runPart(2));
    CHECK(!defaults.visualize && !defaults.bench);
    CHECK(defaults.args.empty() && defaults.benchArgs.empty());
    CHECK(defaults.input.getLine(0) == "line");

    AoCUtils::Challenge part2 = makeChallenge(fx, {"--part", "2", "-v"});
    CHECK(part2.part == 2 && !part2.runPart(1) && part2.runPart(2));
    CHECK(part2.visualize);

    AoCUtils::Challenge part1 = makeChallenge(fx, {"--part=1", "--visualize"});
    CHECK(part1.part == 1 && part1.visualize);

    AoCUtils::Challenge other = makeChallenge(fx, {"extra", "--bench", "--benchmark_filter=part1", "more"});
    CHECK(other.bench);
    CHECK((other.args == std::vector<std::string>{"extra", "more"}));
    CHECK((other.benchArgs == std::vector<std::string>{"--benchmark_filter=part1"}));

    CHECK_THROWS(makeChallenge(fx, {"--part", "3"}));
    CHECK_THROWS(makeChallenge(fx, {"--part=x"}));
    CHECK_THROWS(makeChallenge(fx, {"--part"}));

    // Custom input file name
    fx.write("Challenges/Day01/puzzle.txt", "custom\n");
    AoCUtils::Challenge custom = makeChallenge(fx, {}, "puzzle.txt");
    CHECK(custom.fileName == custom.dir + "/puzzle.txt");
    CHECK(custom.input.getLine(0) == "custom");

    AoCUtils::InputFile viaFileInput = AoCUtils::getFileInput("/anywhere/Day01", fx.root.string(), "Challenges", "puzzle.txt");
    CHECK(viaFileInput.getLine(0) == "custom");
}

// Counts how the library calls it, and answers from the input
class CountingSolution : public AoCUtils::Solution {
    public:
        int parses = 0, part1Calls = 0, part2Calls = 0;
        std::vector<bool> visualizeSeen;

        CountingSolution(AoCUtils::Challenge &c) : AoCUtils::Solution(c) {}

        void parse() override { parses++; }
        std::string part1() override { part1Calls++; visualizeSeen.push_back(visualize); return "answer-" + challenge.input.getLine(0); }
        std::string part2() override { part2Calls++; visualizeSeen.push_back(visualize); return "second"; }
};

// Runs the solution with stdout captured
static std::string runCaptured(CountingSolution &solution, int *result = nullptr) {
    std::ostringstream captured;
    std::streambuf *original = std::cout.rdbuf(captured.rdbuf());
    int code = solution.run();
    std::cout.rdbuf(original);
    if (result) *result = code;
    return captured.str();
}

static void testSolutionRun() {
    Fixture fx;
    fx.write("Challenges/Day01/input.txt", "seven\n");

    {
        AoCUtils::Challenge challenge = makeChallenge(fx, {});
        CountingSolution solution(challenge);
        int code = -1;
        std::string out = runCaptured(solution, &code);
        CHECK(code == 0);
        CHECK(solution.parses == 1 && solution.part1Calls == 1 && solution.part2Calls == 1);
        CHECK(out.find("Day01 part 1: answer-seven") != std::string::npos);
        CHECK(out.find("Day01 part 2: second") != std::string::npos);
    }
    {
        AoCUtils::Challenge challenge = makeChallenge(fx, {"--part", "2"});
        CountingSolution solution(challenge);
        std::string out = runCaptured(solution);
        CHECK(solution.part1Calls == 0 && solution.part2Calls == 1);
        CHECK(out.find("part 1") == std::string::npos);
    }
    {
        // --visualize alone: part 1 runs normally, part 2 visualizes
        AoCUtils::Challenge challenge = makeChallenge(fx, {"-v"});
        CountingSolution solution(challenge);
        runCaptured(solution);
        CHECK((solution.visualizeSeen == std::vector<bool>{false, true}));
    }
    {
        AoCUtils::Challenge challenge = makeChallenge(fx, {"--part=1", "-v"});
        CountingSolution solution(challenge);
        runCaptured(solution);
        CHECK((solution.visualizeSeen == std::vector<bool>{true}));
    }
    {
        // --bench: runs the benchmarks instead of once, forwarding --benchmark_* flags
        AoCUtils::Challenge challenge = makeChallenge(fx, {"--bench", "--benchmark_min_time=0.01s"});
        CountingSolution solution(challenge);
        int code = -1;
        std::string out = runCaptured(solution, &code);
        CHECK(code == 0);
        CHECK(solution.parses > 1 && solution.part1Calls > 1 && solution.part2Calls > 1);
        CHECK(out.find("Day01/part1") != std::string::npos);
    }
}

int main() {
    testInputFile();
    testDelimited();
    testGetChallenge();
    testSolutionRun();

    if (failures) {
        std::cerr << failures << " check(s) failed" << std::endl;
        return 1;
    }
    std::cout << "All unit tests passed" << std::endl;
    return 0;
}
