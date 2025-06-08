#pragma once

#include <vector>
#include <list>
#include <stack>
#include "BaseLineSolver.h"

struct UpdateJob
{
    int line_index; // Index of line that is receiving the update. Not useful for the line solver though.
    int index; // Index of element on line
    bool status; // true for filled, false for empty
};
class ParallelLineSolver : public BaseLineSolver<UpdateJob>
{
public:
    ParallelLineSolver(Line &lineRef);

    std::stack<UpdateJob> resolveCommonPatterns();
    void insertUpdate(UpdateJob updateJob);
};