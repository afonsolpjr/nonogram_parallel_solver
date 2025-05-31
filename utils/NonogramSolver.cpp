#include "../src/Nonogram.h"
#include "NonogramSolver.h"
#include <vector>

NonogramSolver::NonogramSolver(Nonogram &nonogram_ref) : nonogram(&nonogram_ref)
{
    for (int i = 0; i < nonogram->getHeight(); i++)
        rowSolvers.push_back(new LineSolver(nonogram->getRow(i)));

    for (int i = 0; i < nonogram->getWidth(); i++)
        columnSolvers.push_back(new LineSolver(nonogram->getColumn(i)));
}

void NonogramSolver::main()
{
    for (const auto &solver : rowSolvers)
        solver->print_possibilities();
}

void NonogramSolver::solve()
{
    while (!isSolved())
    {
        int changesMade = 0;
        for (int i = 0; i < rowSolvers.size(); i++)
        {
            if (rowSolvers[i]->isSolved())
                continue;

            rowSolvers[i]->updatePossibilities();
            std::list<Update> columnUpdates;
            columnUpdates = rowSolvers[i]->resolveCommonPatterns();
            changesMade += columnUpdates.size();
            // At first, the index value on updates indicate the column.
            //  But it must indicate the index on the column.
            for (const auto &update : columnUpdates)
                columnSolvers[update.index]->insertUpdate({i, update.value});
        }

        for (int i = 0; i < columnSolvers.size(); i++)
        {
            if (columnSolvers[i]->isSolved())
                continue;

            columnSolvers[i]->updatePossibilities();
            std::list<Update> rowUpdates;
            rowUpdates = columnSolvers[i]->resolveCommonPatterns();
            changesMade += rowUpdates.size();
            // At first, the index value on updates indicate the row.
            //  But it must indicate the index on the row.
            for (const auto &update : rowUpdates)
                rowSolvers[update.index]->insertUpdate({i, update.value});
        }

        if (!changesMade)
        {
            printf("Acho que não tem solução ein\n");
            return;
        }
        else
            nonogram->print();
    }
}

bool NonogramSolver::isSolved()
{
    for (const auto &solver : rowSolvers)
        if (!solver->isSolved())
            return false;

    return true;
}
