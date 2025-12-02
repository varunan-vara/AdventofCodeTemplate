#include "AoCUtils.h"

namespace AoCUtils {
    InputFile::InputFile(std::string fileName) {
        std::ios::sync_with_stdio(false);
        std::cin.tie(NULL);

        // Use fstream for reading lines
        std::ifstream file_stream(fileName);

        if (!file_stream.is_open()) {
            std::cerr << "Error: Could not open file at path: " << fileName << std::endl;
        }

        std::string line;

        while (std::getline(file_stream, line)) {
            parsedInput.push_back(std::move(line));
        }

        file_stream.close();
    }

    InputFile::~InputFile() {

    }

    std::string InputFile::getLine(int index) {
        return parsedInput[index];
    }

    int InputFile::numberOfLines() {
        return parsedInput.size();
    }

    std::string InputFile::getFullString() {
        std::string returnstring = "";
        
        for (const auto& line : parsedInput) {
            returnstring = returnstring + line;
        }

        return returnstring;
    }

    std::vector<std::string> InputFile::getParsedInput() {
        return parsedInput;
    }

    std::vector<std::vector<char>> InputFile::get2DArray() {
        std::vector<std::vector<char>> tpi;
        std::vector<char> temp;

        for (const auto& line : parsedInput) {
            temp.clear();
            for (const auto& character : line) {
                temp.push_back(character);
            }
            tpi.push_back(temp);
        }

        return tpi;
    }

    std::vector<std::string> InputFile::regexConvert(std::string input, std::string regex) {
        std::vector<std::string> captured_data;
    
        try {
            std::regex pattern_regex(regex);
            std::smatch match_results;
            
            if (std::regex_match(input, match_results, pattern_regex)) {
                for (size_t i = 1; i < match_results.size(); ++i) {
                    captured_data.push_back(match_results[i].str());
                }
            } else {
                std::cerr << "Warning: Line did not match pattern: " << input << std::endl;
            }
        } catch (const std::regex_error& e) {
            std::cerr << "Regex Error: " << e.what() << " Code: " << e.code() << std::endl;
            throw; 
        }

        return captured_data;
    }

    std::vector<std::vector<std::string>> InputFile::getDelimited2DArray(std::string delimitterPattern) {
        std::vector<std::vector<std::string>> return_vector;

        for (const std::string& line : parsedInput) {
            std::vector<std::string> row = regexConvert(line, delimitterPattern);
            return_vector.push_back(std::move(row));
        }

        return return_vector;
    }

    std::vector<std::vector<std::string>> InputFile::getDelimited2DArray(char delimitterValue) {
        std::vector<std::vector<std::string>> return_vector;
        std::vector<std::string> row;

        for (const std::string& line : parsedInput) {
            row.clear();
            std::stringstream ss(line);
            std::string temp;

            while (std::getline(ss, temp, delimitterValue)) {
                row.push_back(temp);
            }
            return_vector.push_back(row);
        }

        return return_vector;
    }

    std::vector<std::string> InputFile::getDelimited2DArray(std::string inputString, char delimitterValue) {
        std::vector<std::string> output;
        std::stringstream ss(inputString);
        std::string temp;

        while (std::getline(ss, temp, delimitterValue)) {
            output.push_back(temp);
        }

        return output;
    }

    std::vector<std::vector<std::string>> InputFile::getDelimited2DArray(std::vector<std::string> inputVector, char delimitterValue) {
        std::vector<std::vector<std::string>> return_vector;
        std::vector<std::string> row;

        for (const std::string& line : inputVector) {
            row.clear();
            std::stringstream ss(line);
            std::string temp;

            while (std::getline(ss, temp, delimitterValue)) {
                row.push_back(temp);
            }
            return_vector.push_back(row);
        }

        return return_vector;
    }
}