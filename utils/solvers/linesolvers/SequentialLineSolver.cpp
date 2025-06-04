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
