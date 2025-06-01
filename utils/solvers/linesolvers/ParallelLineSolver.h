#pragma once

#include <vector>
#include <list>
#include <stack>
#include <mutex>
#include "BaseLineSolver.h"

struct UpdateJob
{
    int line_index, element_index;
    bool status;
};
class ParallelLineSolver : public BaseLineSolver<UpdateJob>
{
public:
    ParallelLineSolver(Line &lineRef);

    std::stack<UpdateJob> resolveCommonPatterns();
    void insertUpdate(UpdateJob updateJob);

    void init();

private:
    std::mutex mutex;
};