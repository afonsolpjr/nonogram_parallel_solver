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
    bool is_parallel = false;
    int nThreads = 0;

public:
    Nonogram puzzle;

    NonogramRun(const RawPuzzleData gameData, bool parallel = false, int nThreads = 0);

    void run();
    void printStats() const;
    static bool verifyCorrectness(const RawPuzzleData gameData, const Nonogram &puzzle);

private:
    static std::vector<bool> puzzleToVector(const Nonogram &puzzle);

    bool verifyCorrectness();
};