#ifndef AOCUTILS
#define AOCUTILS

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>

namespace AoCUtils
{
    class InputFile {
        private:
            std::vector<std::string> parsedInput;
            std::vector<std::string> regexConvert(std::string input, std::string regex);
        public:
            // Constructor Function for InputFile, a class that parses the project input from a txt file
            //
            // Paramaters: 
            //  - fileName: relative directory of input txt file
            InputFile(std::string fileName);
            ~InputFile();

            // Returns a specific line from the file
            //
            // Parameters:
            //  - index: index...
            std::string getLine(int index);

            // Returns the number of lines in the file
            //
            // Parameters: none
            int numberOfLines();

            // Get the file as one large string
            //
            // Parameters: none
            std::string getFullString();

            // Just returns the vector that is stored within the class
            //
            // Parameters: none
            std::vector<std::string> getParsedInput();

            // Returns a 2D array of the items from the parsed input
            //
            // Parameters: none
            std::vector<std::vector<char>> get2DArray();

            // Use a regex string to delimit each line
            //
            // Parameters:
            //  - delimitterPattern: regex pattern
            std::vector<std::vector<std::string>> getDelimited2DArray(std::string delimitterPattern);
            // Using a specific character, delimit each line string
            //
            // Parameters:
            // - delimitterValue: character to split the string by
            std::vector<std::vector<std::string>> getDelimited2DArray(char delimitterValue);
            // Using a speicifc character, delimit the inputString
            //
            // Parameters:
            // - inputString: string to be split
            // - delimitterValue: character to split string by
            static std::vector<std::string> getDelimited2DArray(std::string inputString, char delimitterValue);
            // Using a speicifc character, delimit each value in inputVector
            //
            // Parameters:
            // - inputVector: array of strings to be delimitted
            // - delimitterValue: character to split string by
            static std::vector<std::vector<std::string>> getDelimited2DArray(std::vector<std::string> inputVector, char delimitterValue);
            std::string getGridValue(int xIndex, int yIndex);
            int getGridValue(int xIndex, int yIndex, int multiplier);
    };
} // namespace AoCUtils


#endif /*AOCUTILS*/