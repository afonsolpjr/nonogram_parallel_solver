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
            result.push({i, common_value});
        }
    }
    return result;
}

void SequentialLineSolver::insertUpdate(Update update)
{
    cells_solved++;
    updates.push(update);
}
