#pragma once

#include "../src/Nonogram.h"
#include <vector>
#include <string>
#include "NonogramPuzzleFactory.h"
class RandomGenerator
{
public:
    static Nonogram randomFromString(int rows, int cols);
    static Nonogram randomFromBool(int rows, int cols);
    static void printRandomString(int rows, int cols);

    static std::string generateRandomStringGrid(int rows, int cols);
    static std::vector<bool> generateRandomBoolGrid(int rows, int cols);

};