#include "ParallelNonogramSolver.h"
#include <vector>
#include <thread>

ParallelNonogramSolver::ParallelNonogramSolver(Nonogram &nonogram_ref, int nThreads) : BaseSolver(nonogram_ref)
{
    this->nThreads = nThreads;
    // printf("Nonogram solver iniciado\n"
    //        "Solvers subjacentes: rows = %ld \t cols = %ld",
    //        rowSolvers.size(), columnSolvers.size());
}

bool ParallelNonogramSolver::solve()
{
    std::vector<std::thread> thread_pool;

    // gerar trabalhos para geracao das possibilidades

    for (int i = 0; i < nonogram->getHeight(); i++)
        rowJobs.insert(i);

    for (int i = 0; i < nonogram->getWidth(); i++)
        columnJobs.insert(i);

    // disparar threads

    for (int i = 0; i < nThreads; i++)
        thread_pool.emplace_back(&ParallelNonogramSolver::worker, this, i);

    for (auto &thread : thread_pool)
        thread.join();

    return true;
}

void ParallelNonogramSolver::worker(int id)
{
    int index;
    std::stack<UpdateJob> phaseUpdates;

    // take possibility generation jobs
    // first for rows...
    init_work(true);

    // then collumns...
    init_work(false);

    init_barrier();

    // main dual-phasing loop
    while (true)
    {
        processPhase(phaseUpdates, true);
        if (completionCheckBarrier())
            return;

        processPhase(phaseUpdates, false);
        if (completionCheckBarrier())
            return;
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
            rowJobs.insert(i);
        for (int i = 0; i < nonogram->getWidth(); i++)
            columnJobs.insert(i);

        cond.notify_all();
    }
}

void ParallelNonogramSolver::init_work(bool isRowInit)
{
    int index;

    auto &solvers = isRowInit ? rowSolvers : columnSolvers;

    while (true)
    {
        index = getJob(isRowInit); // row job
        if (index < 0)
            break;
        solvers[index]->init();
    }
}

void ParallelNonogramSolver::processPhase(std::stack<UpdateJob> &phaseUpdates, bool isRowPhase)
{
    int index;

    while (true)
    {
        index = getJob(isRowPhase); // row job
        if (index < 0)
            break;
        phaseWork(index, phaseUpdates, isRowPhase);
    }
    registerPhaseUpdates(phaseUpdates, isRowPhase);
}

void ParallelNonogramSolver::phaseWork(int index, std::stack<UpdateJob> &phaseUpdates, bool isRowPhase)
{
    std::stack<UpdateJob> singleUpdates;

    auto &solvers = isRowPhase ? rowSolvers : columnSolvers;

    solvers[index]->updatePossibilities();
    singleUpdates = solvers[index]->resolveCommonPatterns();

    while (!singleUpdates.empty())
    {
        singleUpdates.top().index = index;
        phaseUpdates.push(singleUpdates.top());
        singleUpdates.pop();
    }
}

void ParallelNonogramSolver::registerPhaseUpdates(std::stack<UpdateJob> &phaseUpdates, bool isRowPhase)
{
    UpdateJob update;
    while (!phaseUpdates.empty())
    {
        update = phaseUpdates.top();
        insertUpdateAndJob(update, isRowPhase); // critical section
        phaseUpdates.pop();
    }
}

/// @brief Insert a job on a jobset.
/// @param index Index of the row/collumn to be processed.
/// @param isRow Identifier for what type of job we need to set. If true sets a rowJob, if false a columnJob.
void ParallelNonogramSolver::insertUpdateAndJob(UpdateJob update, bool isRowPhase)
{
    std::lock_guard<std::mutex> lock(mutex);

    auto &set = isRowPhase ? columnJobs : rowJobs; // if singleRowPhase, insert on columnJobs, otherwise on rowJobs
    auto &lines = isRowPhase ? columnSolvers : rowSolvers;
    lines[update.line_index]->insertUpdate(update);
    if (!lines[update.line_index]->isSolved())
        set.insert(update.line_index);

    changesMade++;
}

/// @brief Get a job from a jobset.
/// @param isRow Identifier for what type of job we need to get. If true get a rowJob, if false a columnJob.
/// @return Index of a line to process, or a negative value if there is no jobs.
int ParallelNonogramSolver::getJob(bool isRow)
{
    std::lock_guard<std::mutex> lock(mutex);

    auto &set = isRow ? rowJobs : columnJobs;

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
    else if (isSolved() || changesMade == 0)
    {
        completed = true;
        cond.notify_all();
        return true;
    }
    else
    {
        threadsAtBarrier = 0;
        // printf("\nTamanho dos jobs:"
        //        "\n\tRows: %ld"
        //        "\n\tColumns: %ld"
        //        "\n\t Mudanças feitas: %d\n",
        //        rowJobs.size(), columnJobs.size(), changesMade);
        changesMade = 0;
        cond.notify_all();
        return false;
    }
}
