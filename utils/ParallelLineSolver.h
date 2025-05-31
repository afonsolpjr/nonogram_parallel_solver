#pragma once

#include <vector>
#include <list>
#include <stack>
#include <iostream>
#include <mutex>
#include "../src/Nonogram.h"

struct UpdateJob{
    int line_index,element_index;
    bool value;
}
class ParallelLineSolver
{
public:
    Line *line;
    std::list<std::vector<int>> possibilities;
    std::stack<UpdateJob> updates;
    int cells_solved;
    std::mutex mutex;

    ParallelLineSolver(Line &line_ref);

    void init();
    std::stack<UpdateJob> resolveCommonPatterns();
    void updatePossibilities();
    void main();
    void print_possibility(const std::vector<int> &possibility);
    void print_possibilities();
    void insertUpdateJob(UpdateJob update);
    bool isSolved();


private:
    static std::list<std::vector<int>> generatePossibilities(const Line& line, int slack);
    static std::vector<int> composeBlockLine(int length, int block_size, int start);
    void eliminatePossibilities(int index, int status);
    void play(int index, int cell_value);
};