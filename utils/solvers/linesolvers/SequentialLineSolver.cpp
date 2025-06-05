#include "SequentialLineSolver.h"
#include <chrono>
SequentialLineSolver::SequentialLineSolver(Line &line_ref)
{
    line = &line_ref;
    cells_solved = 0;
}

std::stack<Update> SequentialLineSolver::resolveCommonPatterns()
{
    static int count = 0;
    std::stack<Update> result;
    std::list<int> commonIndexes;

    commonIndexes = getCommonIndexes();

    for (int commonIndex : commonIndexes)
    {

        play(commonIndex, (*possibilities.begin())[commonIndex]);
        result.push({commonIndex, (*possibilities.begin())[commonIndex]});
    }

    return result;
}

void SequentialLineSolver::insertUpdate(Update update)
{
    cells_solved++;
    updates.push(update);
}
