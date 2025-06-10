#include "../src/Nonogram.h"
#include "NonogramSolver.h"
#include <vector>

bool NonogramSolver::solve()
{
    while (!isSolved())
    {
        int changesMade = 0;
        std::stack<Update> rowUpdates;
        std::stack<Update> columnUpdates;

        for (int i = 0; i < rowSolvers.size(); i++)
        {
            if (rowSolvers[i]->isSolved())
                continue;

            rowSolvers[i]->updatePossibilities();
            columnUpdates = rowSolvers[i]->resolveCommonPatterns();
            changesMade += columnUpdates.size();
            // At first, the index value on updates indicate the column.
            //  But when registered on column solver, it must indicate the index on the column.
            while (!columnUpdates.empty())
            {
                auto update = columnUpdates.top();
                columnSolvers[update.index]->insertUpdate({i, update.status});
                columnUpdates.pop();
            }
        }

        for (int i = 0; i < columnSolvers.size(); i++)
        {
            if (columnSolvers[i]->isSolved())
                continue;

            columnSolvers[i]->updatePossibilities();
            rowUpdates = columnSolvers[i]->resolveCommonPatterns();
            changesMade += rowUpdates.size();
            // At first, the index value on updates indicate the row.
            //  But when registered on row solver, it must indicate the index on the row.
            while (!rowUpdates.empty())
            {
                auto update = rowUpdates.top();
                rowSolvers[update.index]->insertUpdate({i, update.status});
                rowUpdates.pop();
            }
        }

        if (!changesMade)
            return false;
    }
    return true;
}

void NonogramSolver::init()
{
    for( auto& solver : rowSolvers )
        solver->generatePossibilities();
    
    for( auto& solver : columnSolvers )
        solver->generatePossibilities();
}