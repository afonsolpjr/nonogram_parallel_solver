#pragma once

#include <vector>
#include <../src/Nonogram.h>
#include "CombinationGenerator.cpp"

class LineSolver
{

public:
    std::vector<std::vector<int>> possibilities;
    Line* line;

    LineSolver(Line& line_ref) : line(&line_ref) 
    {
        int total_blocks = 0;
        for (int block_size : line->getHints().getBlocks())
            total_blocks += block_size;
        this->possibilities = generate_possibilities(*line,line->getLength());
    }

    void checkCommonalities(){
        for(int i = 0; i < line->getLength(); i++)
        {
            bool common = true;
            int common_value = possibilities[0][i];
            for(const auto& possibility: possibilities){
                if(possibility[i] != common_value)
                    common = false;
            }
            if(common)
            {
                switch(common_value){
                case 0:
                    this->line->blockCell(i);
                    break;

                case 1:
                    this->line->setCell(i);
                    break;
                }
                break;
            }

            // eliminate possibilities on column/row
        }
    }
private:
    std::vector<std::vector<int>> static generate_possibilities(Line line, int slack)
    {
        std::vector<std::vector<int>> combinations;
        int block_size = line.getHint(0);

        // funcao recursiva, caso base são aqueles onde só tem um bloco a ser construido
        if (line.getHintSize() == 1)
        {
            int length = line.getLength();
            for (int start = 0; start <= slack; start++)
                combinations.push_back(composeBlockLine(length, block_size, start));
            return combinations;
        }
        else
        {
            // caso geral, onde cada "uso" de uma folga gera novas combinações restantes
            for (int start = 0; start <= slack; start++)
            {
                std::vector<std::vector<int>> slack_combinations;

                int base_size = start + block_size + 1;
                std::vector<int> base_combination = composeBlockLine(base_size, block_size, start);

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
};