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

    for (int i = 0; i < line->getLength(); i++)
    {
        if (!(*line)[i].isEmpty())
            continue;

        bool common = true;
        bool common_value = possibilities.front()[i];
        for (const auto &possibility : possibilities)
        {
            if (possibility[i] != common_value)
                common = false;
        }
        if (common)
        {
            play(i, common_value);
            result.push({i,0, common_value});
        }
    }
    return result;
}

void ParallelLineSolver::insertUpdate(UpdateJob updateJob)
{
    std::lock_guard<std::mutex> lock(mutex);

    cells_solved++;
    updates.push(updateJob);
}
