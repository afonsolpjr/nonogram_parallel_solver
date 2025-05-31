#include "LineSolver.h"
#include <iostream>

LineSolver::LineSolver(Line &line_ref) : line(&line_ref), cells_solved(0)
{
    int total_blocks = 0;
    for (int block_size : line->getHints().getBlocks())
    {
        total_blocks += block_size;
    }
    int slack = line->getLength() - (total_blocks + line->getHintSize() - 1);
    this->possibilities = generate_possibilities(*line, slack);
}

std::list<Update> LineSolver::resolveCommonPatterns()
{
    std::list<Update> result;
    for (int i = 0; i < line->getLength(); i++)
    {
        if (!(*line)[i].isEmpty())
            continue;

        bool common = true;
        int common_value = possibilities.front()[i];
        for (const auto &possibility : possibilities)
        {
            if (possibility[i] != common_value)
                common = false;
        }
        if (common)
        {
            play(i, common_value);
            result.push_back({i, common_value});
        }
    }
    return result;
}

void LineSolver::updatePossibilities()
{
    for (auto update = updates.begin(); update != updates.end();)
    {
        eliminatePossibilities(update->index, update->value);
        update = updates.erase(update);
    }
}

void LineSolver::main()
{
    for (const auto &possibility : possibilities)
    {
        for (int cell : possibility)
        {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }
}

void LineSolver::print_possibility(const std::vector<int> &possibility)
{
    for (int cell : possibility)
    {
        std::cout << cell << " ";
    }
    std::cout << std::endl;
}

void LineSolver::print_possibilities()
{
    for (const auto &possibility : possibilities)
        print_possibility(possibility);
}

void LineSolver::insertUpdate(Update update)
{
    cells_solved++;
    updates.push_back(update);
}

bool LineSolver::isSolved()
{
    return cells_solved == line->getLength();
}

std::list<std::vector<int>> LineSolver::generate_possibilities(Line line, int slack)
{
    std::list<std::vector<int>> combinations;
    int block_size = line.getHint(0);
    if (line.getHintSize() == 1)
    {
        int length = line.getLength();
        for (int start = 0; start <= slack; start++)
            combinations.push_back(composeBlockLine(length, block_size, start));
        return combinations;
    }
    else
    {
        for (int start = 0; start <= slack; start++)
        {
            std::list<std::vector<int>> slack_combinations;

            int base_size = start + block_size + 1;
            std::vector<int> base_combination = composeBlockLine(base_size, block_size, start);

            Line remaining_line(line.getLength() - base_size, false);

            for (int j = 1; j < line.getHintSize(); ++j)
                remaining_line.addHint(line.getHint(j));

            std::list<std::vector<int>> remaining_combinations = LineSolver::generate_possibilities(remaining_line, slack - start);

            for (const auto &remaining_combination : remaining_combinations)
            {
                std::vector<int> combination = base_combination;
                combination.insert(combination.end(), remaining_combination.begin(), remaining_combination.end());
                slack_combinations.push_back(combination);
            }

            combinations.insert(combinations.end(), slack_combinations.begin(), slack_combinations.end());
        }
    }
    return combinations;
}

std::vector<int> LineSolver::composeBlockLine(int length, int block_size, int start)
{
    std::vector<int> combination(length, 0);
    for (int i = start; i < start + block_size; ++i)
    {
        if (i < length)
        {
            combination[i] = 1;
        }
    }
    return combination;
}

void LineSolver::eliminatePossibilities(int index, int status)
{
    for (auto it = possibilities.begin(); it != possibilities.end();)
    {
        if ((*it)[index] != status)
            it = possibilities.erase(it);
        else
            ++it;
    }
}

void LineSolver::play(int index, int cell_value)
{
    switch (cell_value)
    {
    case 0:
        this->line->blockCell(index);
        break;
    case 1:
        this->line->setCell(index);
        break;
    }
    cells_solved++;
}