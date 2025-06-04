#include "SequentialLineSolver.h"

SequentialLineSolver::SequentialLineSolver(Line &line_ref)
{
    line = &line_ref;
    cells_solved = 0;

    int total_blocks = 0;
    for (int block_size : line->getHints().getBlocks())
    {
        total_blocks += block_size;
    }
    int slack = line->getLength() - (total_blocks + line->getHintSize() - 1);
    this->possibilities = generatePossibilities(*line, slack);
}

std::stack<Update> SequentialLineSolver::resolveCommonPatterns()
{
    std::stack<Update> result;
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

    for (int commonIndex : candidates)
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
