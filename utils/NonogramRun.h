#pragma once
#include "solvers/NonogramSolver.h"
#include "solvers/ParallelNonogramSolver.h"
#include "NonogramPuzzleFactory.h"
#include <chrono>
#include <string>

class NonogramRun
{
    const RawPuzzleData gameData;
    std::chrono::duration<double> init_time;
    std::chrono::duration<double> solve_time;
    bool is_correct = false;
    int nThreads = 0;

public:
    Nonogram puzzle;

    /// @brief Initialize a execution stance based on a raw puzzle data.
    /// @param gameData raw game data to instance the puzzle
    /// @param nThreads number of threads to use on the execution of the solver
    NonogramRun(const RawPuzzleData gameData, int nThreads);

    /// @brief Initialize and solve the puzzle.
    /// @return true if game is correct (same as raw data), false otherwise.
    bool run();

    /// @brief Exhibit execution status (debug only)
    void printStats();

    /// @brief Compares the state of a game with the raw data that generated it.
    /// @param gameData raw game  data
    /// @param puzzle game being played
    /// @return true if the puzzle represent the same raw data that generated it, false otherwise.
    static bool verifyCorrectness(const RawPuzzleData gameData, const Nonogram &puzzle);

    /// @brief writes execution data formatted in comma separated values
    /// @param filePtr file descriptor of the data file
    void appendResult(FILE *filePtr);

    /// @brief Formats execution data to a string with csv values
    /// @return execution data as a string, in csv
    const std::string toCSV();

private:
    /// @brief Transforms the game, in the state it is, in a contiguous bool vector. true values represents painted cells,
    /// while false values represents empty cells.
    /// @param puzzle game to transform into vector
    /// @return the bool vector
    static std::vector<bool> puzzleToVector(const Nonogram &puzzle);

    /// @brief Compares the state of the game within the object with the raw data that generated it.
    /// @return true if the puzzle represent the same raw data that generated it, false otherwise.
    bool verifyCorrectness();
};