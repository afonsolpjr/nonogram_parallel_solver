#include "NonogramRun.h"
#include <fstream>
#include <sstream>

void NonogramPuzzleFactory::create_games(const std::vector<int> dimensions, int num_games, const std::string filename)
{
    std::ofstream out(filename, std::ios::trunc);
    out.close();
    int unsolvables = 0;
    for (const auto dimension : dimensions)
    {
        std::vector<std::vector<bool>> grids;
        grids.reserve(num_games);

        std::srand(static_cast<unsigned int>(time(0)));

        while (grids.size() != num_games)
        {
            printf("\r %ld\\%d jogos de tamanho %d gerados", grids.size(), num_games, dimension);
            fflush(stdout);

            std::vector<bool> grid = getDifferentGrid(grids, dimension);
            Nonogram puzzle = RandomGenerator::fromBool(grid, dimension);
            NonogramSolver solver(puzzle);
            if (solver.solve() && NonogramRun::verifyCorrectness({dimension,grid},puzzle))
            {
                grids.push_back(grid);
                printf("\r %ld\\%d jogos de tamanho %d gerados", grids.size(), num_games, dimension);
                fflush(stdout);
                writeGridToFile(grid, dimension, filename);
                grid.clear();
            }
            else
                printf(" %d sem solução..", ++unsolvables);
        }
    }
}

std::vector<bool> NonogramPuzzleFactory::getDifferentGrid(const std::vector<std::vector<bool>> &grid, int dimension)
{
    int static c = 0;
    while (true)
    {

        bool unique = true;

        std::vector<bool> newGrid = RandomGenerator::generateRandomBoolGrid(dimension, dimension);
        for (const auto &oldGrid : grid)
        {
            if (equalGrids(oldGrid, newGrid))
            {
                unique = false;
                break;
            }
        }
        if (unique)
            return newGrid;
        else
        {
            printf("tentativa de numero %d \n", c++);
            fflush(stdout);
        }
    }
}

bool NonogramPuzzleFactory::equalGrids(const std::vector<bool> &oldGrid, const std::vector<bool> &newGrid)
{
    if (oldGrid.size() != newGrid.size())
        return false;

    for (int i = 0; i < oldGrid.size(); i++)
    {
        if (oldGrid[i] != newGrid[i])
            return false;
    }
    return true;
}

void NonogramPuzzleFactory::writeGridToFile(const std::vector<bool> &grid, int dimension, const std::string &filename)
{
    FILE *out = fopen(filename.c_str(), "a");
    if (!out)
        return;
    fprintf(out, "%d ", dimension);
    for (bool b : grid)
        fputc(b ? '1' : '0', out);
    fputc('\n', out);
    fclose(out);
}

std::vector<RawPuzzleData> NonogramPuzzleFactory::loadGamesFromFile(const std::string &filename)
{
    std::vector<RawPuzzleData> games;
    std::ifstream in(filename);
    std::string line;

    while (std::getline(in, line))
    {
        std::istringstream iss(line);
        int dimension;
        std::string gridstr;

        if (!(iss >> dimension >> gridstr))
            continue;

        std::vector<bool> grid;
        for (char c : gridstr)
            grid.push_back(c == '1');
        games.push_back({dimension, grid});
    }
    return games;
}

void NonogramPuzzleFactory::loadGames(std::string filename)
{
    std::vector<RawPuzzleData> gamesToInit;
    gamesToInit = loadGamesFromFile(filename);

    for (auto it = gamesToInit.begin(); it != gamesToInit.end();)
    {
        Nonogram puzzle = RandomGenerator::fromBool(it->grid, it->dimension);
        NonogramSolver solver(puzzle);
        solver.solve();
        puzzle.print();
        it = gamesToInit.erase(it);
    }
}