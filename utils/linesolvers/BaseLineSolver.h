#pragma once

#include <vector>
#include <list>
#include <iostream>
#include <stack>
#include "../../src/Nonogram.h"
template <typename UpdateType>
class BaseLineSolver
{
public:
    Line *line;
    std::list<std::vector<bool>> possibilities;
    int cells_solved;
    std::stack<UpdateType> updates;

    void updatePossibilities();
    void insertUpdate(UpdateType update);
    std::stack<UpdateType> resolveCommonPatterns();

    void print_possibilities();
    void print_possibility(const std::vector<bool> &possibility);
    bool isSolved();

protected:
    std::list<std::vector<bool>> generatePossibilities(const Line &line, int slack);
    std::vector<bool> composeBlockLine(int length, int block_size, int start);
    void eliminatePossibilities(int index, bool status);
    void play(int index, bool cell_value);
};


template <typename UpdateType>
void BaseLineSolver<UpdateType>::updatePossibilities()
{
    while (!updates.empty())
    {
        auto update = updates.top();
        eliminatePossibilities(update.index, update.value);
        updates.pop();
    }
}

template <typename UpdateType>
void BaseLineSolver<UpdateType>::print_possibility(const std::vector<bool> &possibility)
{
    for (auto cell : possibility)
    {
        std::cout << cell << " ";
    }
    std::cout << std::endl;
}

template <typename UpdateType>
bool BaseLineSolver<UpdateType>::isSolved()
{
    return cells_solved == line->getLength();
}

template <typename UpdateType>
std::list<std::vector<bool>> BaseLineSolver<UpdateType>::generatePossibilities(const Line &line, int slack)
{
    std::list<std::vector<bool>> combinations;
    int block_size = line.getHint(0);
    if (line.getHintSize() == 1) // base recursive case: there is only one hint on the line
    {
        int length = line.getLength();
        for (int start = 0; start <= slack; start++)
            combinations.push_back(composeBlockLine(length, block_size, start));
        return combinations;
    }
    else // having more than one hint...
    {
        for (int start = 0; start <= slack; start++)
        {
            std::list<std::vector<bool>> slack_combinations;

            int base_size = start + block_size + 1;

            // for each use of a slack cell, we will have a base combination that will recursively define how the remaining blocks will be displayed
            std::vector<bool> base_combination = composeBlockLine(base_size, block_size, start);

            // creating new line with remaining hints, and getting remaining combinations
            Line remaining_line(line.getLength() - base_size, false);
            for (int j = 1; j < line.getHintSize(); ++j)
                remaining_line.addHint(line.getHint(j));

            std::list<std::vector<bool>> remaining_combinations = generatePossibilities(remaining_line, slack - start);
            // merging the actual base combination with the remaning ones
            for (const auto &remaining_combination : remaining_combinations)
            {
                std::vector<bool> combination = base_combination;
                combination.insert(combination.end(), remaining_combination.begin(), remaining_combination.end());
                // putting everything based on this "slack use" on a list
                slack_combinations.push_back(combination);
            }
            // merging
            combinations.insert(combinations.end(), slack_combinations.begin(), slack_combinations.end());
        }
    }
    return combinations;
}

template <typename UpdateType>
std::vector<bool> BaseLineSolver<UpdateType>::composeBlockLine(int length, int block_size, int start)
{
    std::vector<bool> combination(length, false);
    for (int i = start; i < start + block_size; ++i)
    {
        if (i < length)
        {
            combination[i] = true;
        }
    }
    return combination;
}

template <typename UpdateType>
void BaseLineSolver<UpdateType>::eliminatePossibilities(int index, bool status)
{
    for (auto it = possibilities.begin(); it != possibilities.end();)
    {
        if ((*it)[index] != status)
            it = possibilities.erase(it);
        else
            ++it;
    }
}

template <typename UpdateType>
void BaseLineSolver<UpdateType>::play(int index, bool cell_value)
{
    if (cell_value)
        this->line->setCell(index);
    else
        this->line->blockCell(index);
    cells_solved++;
}

template <typename UpdateType>
void BaseLineSolver<UpdateType>::print_possibilities()
{
    for (const auto &possibility : possibilities)
        print_possibility(possibility);
}
