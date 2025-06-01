#pragma once
#include "solvers/NonogramSolver.h"
#include "NonogramPuzzleFactory.h"
#include <chrono>
#include <string>

class NonogramRun
{
    const RawPuzzleData gameData;
    std::chrono::duration<double> init_time;
    std::chrono::duration<double> solve_time;
    bool is_correct = false;

public:
    NonogramRun(const RawPuzzleData gameData);
    Nonogram puzzle;

    void run();
    void printStats() const;
    static bool verifyCorrectness(const RawPuzzleData gameData, const Nonogram &puzzle);

private:
    static std::vector<bool> puzzleToVector(const Nonogram &puzzle);

    bool verifyCorrectness();
};