#include <iostream>
#include <string>
#include "RandomGenerator.h"

// #define DEBUG 1

#ifdef DEBUG
#define DBG(x) std::cout << x << std::endl
#else
#define DBG(x)
#endif


Nonogram RandomGenerator::randomFromString(int rows, int cols)
{
    std::string grid = generateRandomStringGrid(rows, cols);
    Nonogram nonogram = NonogramPuzzleFactory::fromString(grid);
    return nonogram;
}

Nonogram RandomGenerator::randomFromBool(int rows, int cols)
{
    std::vector<bool> grid = generateRandomBoolGrid(rows, cols);
    Nonogram nonogram = NonogramPuzzleFactory::fromBool(grid, cols);
    return nonogram;
}

void RandomGenerator::printRandomString(int rows, int cols)
{
    std::string grid = generateRandomStringGrid(rows, cols);
    std::cout << grid;
}

// Função auxiliar para gerar uma string de grid aleatória com 0s e 1s
std::string RandomGenerator::generateRandomStringGrid(int rows, int cols)
{
    std::string grid;

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            grid += (rand() % 2) ? '1' : '0';
        }
        grid += '\n';
    }
    return grid;
}

std::vector<bool> RandomGenerator::generateRandomBoolGrid(int rows, int cols)
{
    std::vector<bool> grid;

    grid.reserve(rows * cols);
    for (int i = 0; i < rows * cols; ++i)
    {
        grid.push_back((rand() % 10 < 6) ? true : false);
    }
    return grid;
}
