#pragma once

#include "ParallelLineSolver.h"
#include "../src/Nonogram.h"
#include <vector>
#include <unordered_set>
#include <mutex>
#include <condition_variable>

class ParallelNonogramSolver
{
public:
    Nonogram *nonogram;
    std::vector<ParallelLineSolver *> rowSolvers;
    std::vector<ParallelLineSolver *> columnSolvers;
    int nThreads;
    std::mutex mutex;
    std::condition_variable cond;
    ParallelNonogramSolver(Nonogram &nonogram_ref, int nThreads);

    bool solve();
    bool isSolved();
    int rowsSize() { return rowSolvers.size(); };

    void worker();
    void static test(int nThreads);

private:
    std::unordered_set<int> rowsJobs,
        columnJobs;

    //  @brief Insert a job on a jobset.
    /// @param index Index of the row/collumn to be processed.
    /// @param isRow Identifier for what type of job we need to set. If true sets a rowJob, if false a columnJob.
    void insertJob(int index, bool isRow);

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