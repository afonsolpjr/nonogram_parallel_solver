#pragma once
#include "linesolvers/SequentialLineSolver.h"
#include "BaseSolver.h"
#include <vector>
#include <list>

class NonogramSolver : public BaseSolver<SequentialLineSolver>
{
public:
    NonogramSolver(Nonogram &nonogramref)
        : BaseSolver<SequentialLineSolver>(nonogramref) {}
    /// @brief Solve a puzzle
    /// @return true if solved. false if it can't terminate the puzzle.
    bool solve();

    /// @brief Initializate a solver, generating possibilities across the line solvers.
    void init();
};