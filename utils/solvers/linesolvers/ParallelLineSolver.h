#pragma once

#include <vector>
#include <list>
#include <stack>
#include "BaseLineSolver.h"

struct UpdateJob
{
    int line_index; // Index of line that is receiving the update. Not useful for the line solver though.
    int index;      // Index of element on line
    bool status;    // true for filled, false for empty
};
class ParallelLineSolver : public BaseLineSolver<UpdateJob>
{
public:
    ParallelLineSolver(Line &lineRef);

    /// @brief Check for unanimity on cells values across the possibilities.
    /// @return stack of updates containing information of which collumn needs reassessing on the next phase.
    std::stack<UpdateJob> resolveCommonPatterns();

    /// @brief Insert a update within the Line Solver.
    /// @param update representation of what cell changed on the last phase, and what status it has now
    void insertUpdate(UpdateJob updateJob);
};