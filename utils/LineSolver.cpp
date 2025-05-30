#include <vector>
#include <list>
#include "../src/Nonogram.h"
#include <iostream>

struct Update
{
    int index, value;
};
class LineSolver
{

public:
    Line *line;

    std::list<std::vector<int>> possibilities;

    std::list<Update> updates;

    int cells_solved;

    LineSolver(Line &line_ref) : line(&line_ref), cells_solved(0)
    {
        int total_blocks = 0;
        for (int block_size : line->getHints().getBlocks())
        {
            total_blocks += block_size;
        }
        int slack = line->getLength() - (total_blocks + line->getHintSize() - 1);
        this->possibilities = generate_possibilities(*line, slack);
    }

    /// @brief Check for communalities on the lines possibilities, and resolve them, marking the cell as the common value found.
    /// @return List of updates(possibility exclusions) to do on a row/column.
    std::list<Update> resolveCommonPatterns()
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

    /// @brief Conform the possibilities with each update made in the updates list.
    void updatePossibilities()
    {
        // Each update data has the cell and value to filter on all possibilities.
        for (auto update = updates.begin(); update != updates.end();)
        {
            eliminatePossibilities(update->index, update->value);
            update = updates.erase(update);
        }
    }

    void main()
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

    /* Print a possibility */
    void print_possibility(const std::vector<int> &possibility)
    {
        for (int cell : possibility)
        {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }

    // Print all possibilities
    void print_possibilities()
    {
        for (const auto &possibility : possibilities)
            print_possibility(possibility);
    }

    void insertUpdate(Update update)
    {
        cells_solved++;
        updates.push_back(update);
    }

    bool isSolved()
    {
        return cells_solved == line->getLength();
    }

private:
    /**
     * @brief Generate all possibilities based on the "slack" number of a line.(number of free empty spaces)
     * Slack is understood as the (sum of blocks length) +  (number of blocks - 1)
     * @param line
     * @param slack
     * @return
     */
    std::list<std::vector<int>> static generate_possibilities(Line line, int slack)
    {
        std::list<std::vector<int>> combinations;
        int block_size = line.getHint(0);
        // funcao recursiva, caso base são aqueles onde só tem um bloco a ser construido
        if (line.getHintSize() == 1)
        {

            int length = line.getLength();
            // printf("tentando construir linha unica de tam = %d, length = %d, slack = %d\n", block_size, length, slack);

            for (int start = 0; start <= slack; start++)
                combinations.push_back(composeBlockLine(length, block_size, start));
            return combinations;
        }
        else
        {
            // caso geral, onde cada "uso" de uma folga gera novas combinações restantes
            for (int start = 0; start <= slack; start++)
            {
                std::list<std::vector<int>> slack_combinations;

                int base_size = start + block_size + 1;
                std::vector<int> base_combination = composeBlockLine(base_size, block_size, start);

                // gera as combinações para o restante dos blocos
                Line remaining_line(line.getLength() - base_size, false);

                for (int j = 1; j < line.getHintSize(); ++j)
                    remaining_line.addHint(line.getHint(j));

                std::list<std::vector<int>> remaining_combinations = LineSolver::generate_possibilities(remaining_line, slack - start);

                // combina as combinações base com as combinações restantes
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

    /**
     * @brief Generate a "line" with 'block_size' contiguous 1's, beginning at position 'start'
     * @param length length of the line
     * @param block_size length of contiguous 1's
     * @param start position to start the block
     * @return
     */
    std::vector<int> static composeBlockLine(int length, int block_size, int start)
    {
        std::vector<int> combination(length, 0);
        for (int i = start; i < start + block_size; ++i)
        {
            if (i < length)
            {
                combination[i] = 1; // Set the cell to 1 (filled)
            }
        }

        return combination;
    }

    /**
     * @brief Eliminate possibilities with 'status' value on the indexed cell.
     * @param index Index of the block to check values in possibilities
     * @param status recently changed status of the cell
     */
    void eliminatePossibilities(int index, int status)
    {

        /* manage possibilities
            this would be a critical section on a concurrent program */
        for (auto it = possibilities.begin(); it != possibilities.end();)
        {
            if ((*it)[index] != status)
                it = possibilities.erase(it); // erase retorna o próximo elemento válido
            else
                ++it;
        }
    }

    /// @brief Make a move on the game. That is, paint a block or mark it as blocked(empty with no chances of painting)
    /// @param index index of the cell on the line
    /// @param block_value status of the cell
    void play(int index, int cell_value)
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
};