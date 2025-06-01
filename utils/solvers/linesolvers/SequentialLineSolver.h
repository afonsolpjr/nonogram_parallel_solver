#pragma once
#include "BaseLineSolver.h"

struct Update
{
    int index, value;
};
class SequentialLineSolver : public BaseLineSolver<Update>
{
public:
    SequentialLineSolver(Line &line_ref);

    std::stack<Update> resolveCommonPatterns();
    void insertUpdate(Update update);
};
