#pragma once

#include <vector>
#include <list>
#include <iostream>
#include <stack>
#include "../../../src/Nonogram.h"

template <typename UpdateType>
class BaseLineSolver
{
public:
    Line *line;
    std::list<std::vector<bool>> possibilities;
    int cells_solved;
    std::stack<UpdateType> updates;

    /// @brief Check updates registered within the object.
    void updatePossibilities();

    /// @brief Insert a update within the Line Solver.
    /// @param update 
    virtual void insertUpdate(UpdateType update) = 0;

    /// @brief Check for unanimity on cells values across the possibilities.
    /// @return stack of updates containing information of which collumn needs reassessing on the next phase.
    std::stack<UpdateType> resolveCommonPatterns();

  
    /// @brief Check if line is solved.
    /// @return True if line is completed.
    bool isSolved();

    /// @brief Generate all posibilities based on line length and the hints.
    void generatePossibilities();

    /* Debug functions */
    void print_possibility(const std::vector<bool> &possibility);
    void print_possibilities();

protected:
    /// @brief Generate all combinations of k elements of the vector n 
    /// @param n vector 
    /// @param k number of elements to choose
    /// @return vector of k-sized combinations of the elements of n
    std::vector<std::vector<int>> generateCombinations(std::vector<int> n, int k);

    /// @brief Eliminate possibilities where the index of the cell has a specific status.
    /// @param index index of the cell 
    /// @param status status to exclude
    void eliminatePossibilities(int index, bool status);

    /// @brief Mark a cell as painted of blocked.
    /// @param index index of the cell
    /// @param cell_value value to mark on the game
    void play(int index, bool cell_value);

    /// @brief Get all indexes which have value commonalities across the possibilities.
    /// @return list of the indexes
    std::list<int> getCommonIndexes();

    /// @brief Transforms a simple combination representation into a possibility of the line.
    /// @param combination the combination created by the generateCombinations function
    /// \see generateCombinations
    void combinationToPossibility(std::vector<int> combination);
};

template <typename UpdateType>
void BaseLineSolver<UpdateType>::updatePossibilities()
{
    while (!updates.empty())
    {
        auto update = updates.top();
        eliminatePossibilities(update.index, update.status);
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
void BaseLineSolver<UpdateType>::generatePossibilities()
{
    if (!line->getHintSize())
    {
        possibilities.emplace_back(std::vector<bool>(line->getLength(), false));
        return;
    }

    int filledCells = 0;

    for (const auto blockSize : line->getHints().getBlocks())
        filledCells += blockSize;

    int blockedCells = line->getHintSize() - 1;
    int freeCells = line->getLength() - (filledCells + blockedCells);
    int nGroups = line->getHintSize();
    int availableSpace = freeCells + nGroups;

    auto choices = std::vector<int>(availableSpace);

    for (int i = 0; i < availableSpace; i++)
        choices[i] = i;

    auto combinations = generateCombinations(choices, nGroups);

    for (auto &combination : combinations)
    {
        combinationToPossibility(combination);
    }
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
std::list<int> BaseLineSolver<UpdateType>::getCommonIndexes()
{
    std::list<int> candidates;

    for (int i = 0; i < line->getLength(); i++)
    {
        if (!(*line)[i].isEmpty())
            continue;
        candidates.push_back(i);
    }

    for (auto possibility = std::next(possibilities.begin()); possibility != possibilities.end(); possibility++)
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

    return candidates;
}

template <typename UpdateType>
void BaseLineSolver<UpdateType>::print_possibilities()
{
    for (const auto &possibility : possibilities)
        print_possibility(possibility);
}

template <typename UpdateType>
std::vector<std::vector<int>> BaseLineSolver<UpdateType>::generateCombinations(std::vector<int> n, int k)
{
    std::vector<std::vector<int>> result;
    if (k == 1)
    {
        for (int i : n)
            result.push_back({i});
        return result;
    }
    else
    {
        for (int i = 0; i < n.size() - k + 1; i++)
        {
            auto head = n[i];

            auto tail = generateCombinations(std::vector<int>(n.begin() + i + 1, n.end()), k - 1);

            for (auto &vec : tail)
            {
                vec.insert(vec.begin(), head);
                result.push_back(vec);
            }
        }
        return result;
    }
}

template <typename UpdateType>
void BaseLineSolver<UpdateType>::combinationToPossibility(std::vector<int> combination)
{
    auto possibility = std::vector<bool>(line->getLength(), false);

    int nBlocksFilled = 0;
    int nHintsDone = 0;
    while (nHintsDone < line->getHintSize())
    {
        int starting_index = combination[nHintsDone] + nBlocksFilled;
        for (int i = starting_index; i < (starting_index + line->getHint(nHintsDone)); i++)
            possibility[i] = true;

        nBlocksFilled += line->getHint(nHintsDone);
        nHintsDone++;
    }
    possibilities.push_back(possibility);
}