#include "ParallelLineSolver.h"

ParallelLineSolver::ParallelLineSolver(Line &lineRef)
{
    line = &lineRef;
    cells_solved = 0;
}


std::stack<UpdateJob> ParallelLineSolver::resolveCommonPatterns()
{
    std::stack<UpdateJob> result;
    std::list<int> commonIndexes;

    commonIndexes = getCommonIndexes();

    for (int commonIndex : commonIndexes)
    {
        play(commonIndex, (*possibilities.begin())[commonIndex]);
        result.push({commonIndex, 0, (*possibilities.begin())[commonIndex]});
    }

    return result;
}

void ParallelLineSolver::insertUpdate(UpdateJob updateJob)
{
    std::lock_guard<std::mutex> lock(mutex);

    cells_solved++;
    updates.push(updateJob);
}
