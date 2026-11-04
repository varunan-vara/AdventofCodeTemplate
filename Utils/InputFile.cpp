#include "InputFile.h"

namespace AoCUtils {
    InputFile::InputFile(std::string fileName) {
        // Use fstream for reading lines
        std::ifstream file_stream(fileName);

        if (!file_stream.is_open()) {
            throw std::runtime_error("Could not open file at path: " + fileName);
        }

        std::string line;

        while (std::getline(file_stream, line)) {
            // Cache this
            charSize = line.size();
            parsedInput.push_back(std::move(line));
        }

        file_stream.close();

        // Cache this
        vecSize = parsedInput.size();
    }

    InputFile::~InputFile() {

    }

    std::string InputFile::getLine(int index) {
        return parsedInput[index];
    }

    int InputFile::numberOfLines() {
        return vecSize;
    }

    int InputFile::numberOfChars() {
        return charSize;
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

    std::vector<std::vector<int>> InputFile::getDelimited2DIntArray(std::string delimitterPattern) {
        std::vector<std::vector<int>> return_vector;

        for (const auto& row : getDelimited2DArray(delimitterPattern)) {
            std::vector<int> intRow;
            for (const std::string& value : row) {
                intRow.push_back(std::stoi(value));
            }
            return_vector.push_back(std::move(intRow));
        }

        return return_vector;
    }

    std::vector<std::vector<int>> InputFile::getDelimited2DIntArray(char delimitterValue) {
        std::vector<std::vector<int>> return_vector;

        for (const auto& row : getDelimited2DArray(delimitterValue)) {
            std::vector<int> intRow;
            for (const std::string& value : row) {
                // Skip empty entries (e.g. "1,,2") rather than letting stoi throw
                if (value.empty()) continue;
                intRow.push_back(std::stoi(value));
            }
            return_vector.push_back(std::move(intRow));
        }

        return return_vector;
    }

    char InputFile::getGridValue(int xIndex, int yIndex) {
        return parsedInput[yIndex][xIndex];
    }

    void InputFile::setGridValue(int xIndex, int yIndex, char value) {
        if (value == 0) {
            throw std::runtime_error("Cannot write null value to string");
        }

        parsedInput[yIndex][xIndex] = value;
    }
}
