#pragma once

#include "linesolvers/SequentialLineSolver.h"
#include "../../src/Nonogram.h"
#include <vector>
#include <list>

template <typename LineSolverType>
class BaseSolver
{
public:
    Nonogram *nonogram;
    std::vector<LineSolverType *> rowSolvers;
    std::vector<LineSolverType *> columnSolvers;

    BaseSolver(Nonogram &nonogram_ref);
    
    /// @brief Initializate a solver, generating possibilities across the line solvers.
    virtual void init() = 0;

    /// @brief Solve the puzzle
    /// @return true if solved, false otherwise
    virtual bool solve() = 0;
    bool isSolved();
};

template <typename LineSolverType>
bool BaseSolver<LineSolverType>::isSolved()
{
    for (const auto &solver : rowSolvers)
        if (!solver->isSolved())
            return false;

    return true;
}

template <typename LineSolverType>
BaseSolver<LineSolverType>::BaseSolver(Nonogram &nonogram_ref)
{

    nonogram = &nonogram_ref;
    for (int i = 0; i < nonogram->getHeight(); i++)
        rowSolvers.push_back(new LineSolverType(nonogram->getRow(i)));

    for (int i = 0; i < nonogram->getWidth(); i++)
        columnSolvers.push_back(new LineSolverType(nonogram->getColumn(i)));
}
