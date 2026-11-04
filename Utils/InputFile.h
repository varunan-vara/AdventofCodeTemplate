#ifndef AOCUTILS_INPUTFILE
#define AOCUTILS_INPUTFILE

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <stdexcept>

namespace AoCUtils
{
    class InputFile {
        private:
            std::vector<std::string> parsedInput;
            std::vector<std::string> regexConvert(std::string input, std::string regex);
            long long vecSize;
            long long charSize = 0;
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

            // Returns the number of characters in one line of input (last line)
            //
            // Parameters: none
            int numberOfChars();

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
            // Using a specifc character, delimit each value in inputVector
            //
            // Parameters:
            // - inputVector: array of strings to be delimitted
            // - delimitterValue: character to split string by
            static std::vector<std::vector<std::string>> getDelimited2DArray(std::vector<std::string> inputVector, char delimitterValue);
            // Use a regex string to delimit each line
            //
            // Parameters:
            //  - delimitterPattern: regex pattern (every capture group must be numeric, or stoi throws)
            std::vector<std::vector<int>> getDelimited2DIntArray(std::string delimitterPattern);
            // Using a specific character, delimit each line string
            //
            // Parameters:
            // - delimitterValue: character to split the string by
            std::vector<std::vector<int>> getDelimited2DIntArray(char delimitterValue);

            // Returns the value of a char at a point in the string, assuming newlines are different y entries
            //
            // Parameters:
            //  - xIndex: Along a line, from left to right
            //  - yIndex: Along a file, from top to bottom
            char getGridValue(int xIndex, int yIndex);
            // Set a grid value in the inputString (does not change size or anything)
            //
            // Parameters:
            //  - xIndex: Along a line, from left to right
            //  - yIndex: Along a file, from top to bottom
            //  - value: char value to be set
            void setGridValue(int xIndex, int yIndex, char value);
            int getGridValue(int xIndex, int yIndex, int multiplier);
    };
} // namespace AoCUtils


#endif /*AOCUTILS_INPUTFILE*/