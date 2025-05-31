#pragma once

#include "LineSolver.h"
#include "../src/Nonogram.h"
#include <vector>
#include <list>

class NonogramSolver
{
public:
    Nonogram *nonogram;
    std::vector<LineSolver *> rowSolvers;
    std::vector<LineSolver *> columnSolvers;

    NonogramSolver(Nonogram &nonogram_ref);

    void main();
    bool solve();
    bool isSolved();
};