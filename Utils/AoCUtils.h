#ifndef AOCUTILS
#define AOCUTILS

#include <vector>
#include <string>
#include <variant>

namespace AoCUtils
{
    class InputFile {
        private:
            std::vector<std::string> parsedInput;
        public:
            // Constructor Function for InputFile, a class that parses the project input from a txt file
            //
            // Paramaters: 
            //  - fileName: relative directory of input txt file
            InputFile(std::string fileName);
            ~InputFile();
            std::string getLine(int index);
            std::string getFullString();
            std::vector<std::string> getParsedInput();
            std::vector<std::vector<int>> get2DArray();
            std::vector<std::vector<std::string>> getDelimited2DArray();
            std::string getGridValue(int xIndex, int yIndex);
            int getGridValue(int xIndex, int yIndex, int multiplier);
    };
} // namespace AoCUtils


#endif /*AOCUTILS*/