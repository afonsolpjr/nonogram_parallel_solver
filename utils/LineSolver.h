#pragma once

#include <vector>
#include <list>
#include <iostream>
#include "../src/Nonogram.h"

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

    LineSolver(Line &line_ref);

    std::list<Update> resolveCommonPatterns();
    void updatePossibilities();
    void main();
    void print_possibility(const std::vector<int> &possibility);
    void print_possibilities();
    void insertUpdate(Update update);
    bool isSolved();

private:
    static std::list<std::vector<int>> generate_possibilities(Line line, int slack);
    static std::vector<int> composeBlockLine(int length, int block_size, int start);
    void eliminatePossibilities(int index, int status);
    void play(int index, int cell_value);
};