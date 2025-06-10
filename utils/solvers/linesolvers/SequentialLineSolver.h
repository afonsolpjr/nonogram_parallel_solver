#pragma once
#include "BaseLineSolver.h"

struct Update
{
    int index;
    bool status;
};
class SequentialLineSolver : public BaseLineSolver<Update>
{
public:
    SequentialLineSolver(Line &line_ref);

    /// @brief Check for unanimity on cells values across the possibilities.
    /// @return stack of updates containing information of which collumn needs reassessing on the next phase.
    std::stack<Update> resolveCommonPatterns();

    /// @brief Insert a update within the Line Solver.
    /// @param update representation of what cell changed on the last phase, and what status it has now
    void insertUpdate(Update update);
};
