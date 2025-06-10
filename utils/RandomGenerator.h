#pragma once
#include "../src/Nonogram.h"
#include <vector>
#include <string>
#include "NonogramPuzzleFactory.h"

class RandomGenerator
{
public:
    /// @brief Create random game represented by sequential strings of 1s and 0s
    /// @param rows number of rows of the game
    /// @param cols number of collumns of the game
    /// @return Nonogram object
    static Nonogram randomFromString(int rows, int cols);

    /// @brief Create random game represented by sequential booleans
    /// @param rows number of rows of the game
    /// @param cols number of collumns of the game
    /// @return Nonogram object
    static Nonogram randomFromBool(int rows, int cols);

    /* debug only */
    static void printRandomString(int rows, int cols);
    
    /// @brief Generate a random string representing a game, with 60% of cells painted, on average.
    /// @param rows number of rows of the game
    /// @param cols number of collumns of the game
    /// @return string representing the game
    static std::string generateRandomStringGrid(int rows, int cols);

     /// @brief Generate a random string representing a game, with 60% of cells painted, on average.
    /// @param rows number of rows of the game
    /// @param cols number of collumns of the game
    /// @return boolean vector representing the game
    static std::vector<bool> generateRandomBoolGrid(int rows, int cols);
};