#pragma once
#include "RandomGenerator.h"
#include <vector>

struct RawPuzzleData
{
    int dimension;
    int gameId;
    std::vector<bool> grid;
};

class NonogramPuzzleFactory
{
public:

    static Nonogram fromString(const std::string &input);

    static Nonogram fromBool(const std::vector<bool> &input, int cols);
    
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

    /**
     * @brief Split a continuous string (representing a game) into lines.
     * @param str The string representing the puzzle.
     * @return Vector containing all lines, as a string.
     */
    static std::vector<std::string> splitLines(const std::string &str);

    /**
     * @brief Split a continuous bool vector, representing a puzzle, into lines.
     * @param input The bool vector representing a puzzle.
     * @param row_length Length of the rows.
     * @return Vector containing all lines, as a bool vector.
     */
    static std::vector<std::vector<bool>> splitLines(const std::vector<bool> &input, int row_length);

    // funcao para verificar se as linhas da string sao validas (contem apenas 0 e 1s e linhas do mesmo tamanho), recebe vetor de linhas
    static bool isValidLines(const std::vector<std::string> &lines);

    // Criadores de linhas para uma linha em string, bool, ou a partir do estado do jogo (usado para construir as colunas)
    static void parseLine(Line &line, std::string &linestr);
    static void parseLine(Line &line, std::vector<bool> boolLine);
    static void parseLine(Line &line);
};