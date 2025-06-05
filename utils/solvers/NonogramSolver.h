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

    bool solve();
    void init();
};