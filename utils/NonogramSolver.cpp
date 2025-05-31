#include "../src/Nonogram.h"
#include "NonogramSolver.h"
#include <vector>

NonogramSolver::NonogramSolver(Nonogram &nonogram_ref) : nonogram(&nonogram_ref)
{
    for (int i = 0; i < nonogram->getHeight(); i++)
        rowSolvers.push_back(new SequentialLineSolver(nonogram->getRow(i)));

    for (int i = 0; i < nonogram->getWidth(); i++)
        columnSolvers.push_back(new SequentialLineSolver(nonogram->getColumn(i)));
}

void NonogramSolver::main()
{
    for (const auto &solver : rowSolvers)
        solver->print_possibilities();
}

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
            //  But it must indicate the index on the column.
            while (!columnUpdates.empty())
            {
                auto update = columnUpdates.top();
                columnSolvers[update.index]->insertUpdate({i, update.value});
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
            //  But it must indicate the index on the row.
            while (!rowUpdates.empty())
            {
                auto update = rowUpdates.top();
                rowSolvers[update.index]->insertUpdate({i, update.value});
                rowUpdates.pop();
            }
        }

        if (!changesMade)
            return false;
    }
    return true;
}

bool NonogramSolver::isSolved()
{
    for (const auto &solver : rowSolvers)
        if (!solver->isSolved())
            return false;

    return true;
}
