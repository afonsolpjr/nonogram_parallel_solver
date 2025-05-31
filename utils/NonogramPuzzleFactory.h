#include "RandomGenerator.h"
#include "NonogramSolver.h"
#include <vector>

struct RawPuzzleData
{
    int dimension;
    std::vector<bool> grid;
};

class NonogramPuzzleFactory
{
public:
    static void main(std::string task);
    static void create_games(const std::vector<int> dimensions, int num_games, const std::string filename);
    static void loadGames(std::string filename);

    /// @brief Compare if two bool vectors are equal.
    /// @param oldGrid
    /// @param newGrid
    /// @return true if equal false otherwise
    static bool equalGrids(const std::vector<bool> &oldGrid, const std::vector<bool> &newGrid);
    static std::vector<RawPuzzleData> loadGamesFromFile(const std::string &filename);

private:
    /// @brief Generate a bool grid differente from any grid in the vector parameter
    /// @param grid
    /// @param dimension size of the grid
    /// @return
    std::vector<bool> static getDifferentGrid(const std::vector<std::vector<bool>> &grid, int dimension);

    static void writeGridToFile(const std::vector<bool> &grid, int dimension, const std::string &filename);
};