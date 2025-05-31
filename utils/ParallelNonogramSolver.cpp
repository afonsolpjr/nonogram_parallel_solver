#include "ParallelNonogramSolver.h"
#include <vector>
#include <thread>

ParallelNonogramSolver::ParallelNonogramSolver(Nonogram &nonogram_ref, int nThreads) : nonogram(&nonogram_ref),
                                                                                       nThreads(nThreads)
{
    for (int i = 0; i < nonogram->getHeight(); i++)
        rowSolvers.push_back(new ParallelLineSolver(nonogram->getRow(i)));

    for (int i = 0; i < nonogram->getWidth(); i++)
        columnSolvers.push_back(new ParallelLineSolver(nonogram->getColumn(i)));
}

void ParallelNonogramSolver::test(int nThreads)
{
    std::vector<std::thread> thread_pool;

    for (size_t i = 0; i < nThreads; i++)
    {
        thread_pool.push_back(std::thread(worker, i));
    }
}

bool ParallelNonogramSolver::solve()
{
    std::vector<std::thread> thread_pool;

    // gerar trabalhos para geracao das possibilidades

    for (int i = 0; i < nonogram->getHeight(); i++)
        rowsJobs.insert(i);

    for (int i = 0; i < nonogram->getWidth(); i++)
        columnJobs.insert(i);

    return true;
}

void ParallelNonogramSolver::worker()
{
    int index;
    // take possibility generation jobs
    // first for rows...
    while (1)
    {
        index = getJob(true); // row job
        if (index < 0)
            break;
        rowSolvers[index]->init();
    }

    // then collumns...
    while (1)
    {
        index = getJob(false); // column job
        if (index < 0)
            break;
        columnSolvers[index]->init();
    }

    init_barrier();

    // main dual-phasing loop
    
    while (1)
    {
        while (1) // row phase
        {
            index = getJob(true); // row job
            if (index < 0)
                break;
            rowSolvers[index]->resolveCommonPatterns();
        }

        if (completionCheckBarrier())
            return;

        while (1) // column phase
        {
            index = getJob(false); // column job
            if (index < 0)
                break;
            columnSolvers[index]->resolveCommonPatterns();
        }

        if (completionCheckBarrier())
            return;
    }
}

bool ParallelNonogramSolver::isSolved()
{
    for (const auto &solver : rowSolvers)
        if (!solver->isSolved())
            return false;

    return true;
}

/// @brief Insert a job on a jobset.
/// @param index Index of the row/collumn to be processed.
/// @param isRow Identifier for what type of job we need to set. If true sets a rowJob, if false a columnJob.
void ParallelNonogramSolver::insertJob(int index, bool isRow)
{
    std::lock_guard<std::mutex> lock(mutex);

    auto &set = isRow ? rowsJobs : columnJobs;

    set.insert(index);
}

/// @brief Get a job from a jobset.
/// @param isRow Identifier for what type of job we need to get. If true get a rowJob, if false a columnJob.
/// @return Index of a line to process, or a negative value if there is no jobs.
int ParallelNonogramSolver::getJob(bool isRow)
{
    std::lock_guard<std::mutex> lock(mutex);

    auto &set = isRow ? rowsJobs : columnJobs;

    if (set.size() == 0)
        return -1;

    int item = *set.begin();
    set.erase(set.begin());

    return item;
}

bool ParallelNonogramSolver::completionCheckBarrier()
{
    static int threadsAtBarrier = 0;
    static bool completed = false;
    std::unique_lock<std::mutex> lock(mutex);

    threadsAtBarrier++;

    if (threadsAtBarrier < nThreads)
    {
        cond.wait(lock);
        return completed;
    }
    else if (isSolved())
    {
        completed = true;
        cond.notify_all();
        return true;
    }
    else
    {
        threadsAtBarrier = 0;
        cond.notify_all();
        return false;
    }
}

void ParallelNonogramSolver::init_barrier()
{
    static int threadsAtBarrier = 0;
    std::unique_lock<std::mutex> lock(mutex);
    threadsAtBarrier++;
    if (threadsAtBarrier < nThreads)
        cond.wait(lock);
    else
    {
        threadsAtBarrier = 0;
        // creating all initial jobs. at least once we need to check line similatiries between possibilities
        for (int i = 0; i < nonogram->getHeight(); i++)
            rowsJobs.insert(i);
        for (int i = 0; i < nonogram->getWidth(); i++)
            columnJobs.insert(i);

        cond.notify_all();
        printf("\n\tInicialização feita. Todas possibilidades geradas!\n");
    }
}