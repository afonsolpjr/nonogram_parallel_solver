#pragma once

#include "linesolvers/SequentialLineSolver.h"
#include "../src/Nonogram.h"
#include <vector>
#include <list>

class NonogramSolver
{
public:
    Nonogram *nonogram;
    std::vector<SequentialLineSolver *> rowSolvers;
    std::vector<SequentialLineSolver *> columnSolvers;

    NonogramSolver(Nonogram &nonogram_ref);

    void main();
    bool solve();
    bool isSolved();
};