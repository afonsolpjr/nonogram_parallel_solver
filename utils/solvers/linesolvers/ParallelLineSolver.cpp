#include "ParallelLineSolver.h"

ParallelLineSolver::ParallelLineSolver(Line &lineRef)
{
    line = &lineRef;
    cells_solved = 0;
}

void ParallelLineSolver::init()
{
    int total_blocks = 0;
    for (int block_size : line->getHints().getBlocks())
    {
        total_blocks += block_size;
    }
    int slack = line->getLength() - (total_blocks + line->getHintSize() - 1);
    this->possibilities = generatePossibilities(*line, slack);
}

std::stack<UpdateJob> ParallelLineSolver::resolveCommonPatterns()
{
    std::stack<UpdateJob> result;
    std::list<int> candidates;

    for (int i = 0; i < line->getLength(); i++)
        candidates.push_back(i);

    for (auto possibility = std::next(possibilities.begin()); possibility != possibilities.end();)
    {
        auto prev = std::prev(possibility);
        for (auto index = candidates.begin(); index != candidates.end();)
        {
            if ((*possibility)[*index] != (*prev)[*index])
                index = candidates.erase(index);
            else
                index++;
        }
    }

    for (int commonIndex : candidates) {
    play(commonIndex, (*possibilities.begin())[commonIndex]);
    result.push({commonIndex,0, (*possibilities.begin())[commonIndex]});
}

    return result;
}

void ParallelLineSolver::insertUpdate(UpdateJob updateJob)
{
    std::lock_guard<std::mutex> lock(mutex);

    cells_solved++;
    updates.push(updateJob);
}
