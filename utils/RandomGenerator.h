#pragma once

#include "../src/Nonogram.h"
#include <vector>
#include <string>

class RandomGenerator
{
public:
    static Nonogram randomFromString(int rows, int cols);
    static Nonogram randomFromBool(int rows, int cols);
    static Nonogram fromString(const std::string &input);
    static Nonogram fromBool(const std::vector<bool> &input, int cols);
    static void printRandomString(int rows, int cols);

    static std::string generateRandomStringGrid(int rows, int cols);
    static std::vector<bool> generateRandomBoolGrid(int rows, int cols);

private:
    static std::vector<std::string> splitLines(const std::string &str);
    static std::vector<std::vector<bool>> splitLines(const std::vector<bool> &input, int row_length);
    static bool isValidLines(const std::vector<std::string> &lines);
    static void parseLine(Line &line, std::string &linestr);
    static void parseLine(Line &line, std::vector<bool> boolLine);
    static void parseLine(Line &line);
};