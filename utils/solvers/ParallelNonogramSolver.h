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
    int threadsAtBarrier = 0;
    bool completed = false;
    ParallelNonogramSolver(Nonogram &nonogram_ref, int nThreads);
    
    /// @brief solve the puzzle
    /// @return true if solved, false otherwise. 
    bool solve();
    
    /// @brief Work to be executed on main dual-phasing of the solution method.
    /// @param id identification for a thread (debug purpose only)
    void worker(int id);

    /// @brief Initializate a solver, generating possibilities across the line solvers.
    void init();

    /// @brief worker executed by threads for initialization of the solver
    void init_worker();

private:
    std::unordered_set<int> rowJobs, columnJobs;
    int changesMade = 0;

    /// @brief Insert a job on a jobset.
    /// @param index Index of the row/collumn to be processed.
    /// @param isRow Identifier for what type of job we need to set. If true sets a rowJob, if false a columnJob.
    void insertUpdateAndJob(UpdateJob update, bool isRow);

    /// @brief Get a job from a jobset.
    /// @param isRow Identifier for what type of job we need to get. If true get a rowJob, if false a columnJob.
    /// @return Index of a line to process, or a negative value if there is no jobs.
    int getJob(bool isRow);

    /// @brief Simple barrier for the threads. Also checks for completion and estagnation of the problem
    /// @param checkForUniqueness check for solver estagnation if true.
    /// @return true if puzzle complete, false otherwise.
    bool completionCheckBarrier(bool checkForUniqueness);

    /// @brief Wait for other threads initialization. The last thread create jobs for all rows and collumns, so at least once the rows and collumns have their possibilities checked.
    void init_barrier();

    /// @brief Generate possibilities of the linesolvers
    /// @param isRowInit if true, generate possibilities of rows. If false, of the collumns.
    void init_work(bool isRowInit);

    /// @brief Representation of a solving phase, allocating jobs.
    /// @param phaseUpdates stack where to save any update made on the phase
    /// @param isRowPhase if true, try to get a rowJob. if false, a collumnJob.
    void processPhase(std::stack<UpdateJob> &phaseUpdates, bool isRowPhase);

    /// @brief Check for updates in a line.
    /// @param index index of the line solver
    /// @param phaseUpdates stack where to save any updates made
    /// @param isRowPhase if true, utilize rowSolvers. if false, collumnSolvers.
    void phaseWork(int index, std::stack<UpdateJob> &phaseUpdates, bool isRowPhase);

    /// @brief Synchronizedly register updates on the pertinent line solver, and set a job for the next phase.
    /// @param phaseUpdates updates to be registered on opposite solvers
    /// @param isRowPhase if true, register on collumn solvers. if false, on rowSolvers.
    void registerPhaseUpdates(std::stack<UpdateJob> &phaseUpdates, bool isRowPhase);
};