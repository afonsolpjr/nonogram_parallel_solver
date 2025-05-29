#include "../src/Nonogram.h"
#include <vector>
#include <iostream>
class CombinationGenerator
{

public:
    // retornará um array de combinacoes possiveis de uma linha
    // cada combinacao é um array de inteiros, onde 1 representa um bloco preenchido e 0 um espaço vazio
    std::vector<std::vector<int>> static generate(Line line, int slack)
    {
        std::vector<std::vector<int>> combinations;
        int block_size = line.getHint(0);

        // funcao recursiva, caso base são aqueles onde só tem um bloco a ser construido
        if (line.getHintSize() == 1)
        {
            int length = line.getLength();
            for (int start = 0; start <= slack; start++)
                combinations.push_back(fill_line(length, block_size, start));
            return combinations;
        }
        else
        {
            // caso geral, onde cada "uso" de uma folga gera novas combinações restantes
            for (int start = 0; start <= slack; start++)
            {
                std::vector<std::vector<int>> slack_combinations;

                int base_size = start + block_size + 1;
                std::vector<int> base_combination = fill_line(base_size, block_size, start);

                // gera as combinações para o restante dos blocos
                Line remaining_line(line.getLength() - base_size, false);

                for (int j = 1; j < line.getHintSize(); ++j)
                    remaining_line.addHint(line.getHint(j));

                std::vector<std::vector<int>> remaining_combinations = CombinationGenerator::generate(remaining_line, slack - start);

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

    void static main()
    {
        // debug only
        Line line(10, false);
        line.addHint(5);
        line.addHint(1);

        int sum = 0;
        int slack;
        for (int i = 0; i < line.getHintSize(); ++i)
            sum += line.getHint(i);

        slack = line.getLength() - sum - (line.getHintSize() - 1);
        std::vector<std::vector<int>> combinations = generate(line, slack);

        for (const auto &combination : combinations)
        {
            for (int cell : combination)
            {
                std::cout << cell << " ";
            }
            std::cout << std::endl;
        }
    }

private:
    std::vector<int> static fill_line(int length, int block_size, int start)
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
};