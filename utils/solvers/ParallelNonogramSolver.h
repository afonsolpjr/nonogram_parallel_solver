#pragma once

#include "linesolvers/ParallelLineSolver.h"
#include <vector>
#include <unordered_set>
#include <mutex>
#include <condition_variable>
#include "BaseSolver.h"

class ParallelNonogramSolver : public BaseSolver<ParallelLineSolver>
{
public:
    int nThreads;
    std::mutex mutex;
    std::condition_variable cond;
    ParallelNonogramSolver(Nonogram &nonogram_ref, int nThreads);

    bool solve();
    void worker(int id);

private:
    std::unordered_set<int> rowJobs, columnJobs;
    int changesMade=0;
    
    //  @brief Insert a job on a jobset.
    /// @param index Index of the row/collumn to be processed.
    /// @param isRow Identifier for what type of job we need to set. If true sets a rowJob, if false a columnJob.
    void insertUpdateAndJob(UpdateJob update, bool isRow);

    /// @brief Get a job from a jobset.
    /// @param isRow Identifier for what type of job we need to get. If true get a rowJob, if false a columnJob.
    /// @return Index of a line to process, or a negative value if there is no jobs.
    int getJob(bool isRow);

    /// @brief Simple barrier for the threads. Also checks for completion.
    /// @return true if puzzle complete, false otherwise.
    bool completionCheckBarrier();

    /// @brief After initialization barrier. Last threads create jobs for all rows and collumns.
    void init_barrier();
};