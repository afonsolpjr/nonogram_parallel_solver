#include "ParallelNonogramSolver.h"
#include <vector>
#include <thread>

ParallelNonogramSolver::ParallelNonogramSolver(Nonogram &nonogram_ref, int nThreads) : BaseSolver(nonogram_ref)
{
    this->nThreads = nThreads;

    // printf("\nNonogram solver iniciado"
    //        "\nSolvers subjacentes: rows = %ld \t cols = %ld"
    //        "\ncompleted =%d \t threadsAtBarrier = %d",
    //        rowSolvers.size(), columnSolvers.size(), completed, threadsAtBarrier);
}

void ParallelNonogramSolver::init()
{
    std::vector<std::thread> thread_pool;

    // gerar trabalhos para geracao das possibilidades
    for (int i = 0; i < nonogram->getHeight(); i++)
        rowJobs.insert(i);

    for (int i = 0; i < nonogram->getWidth(); i++)
        columnJobs.insert(i);

    for (int i = 0; i < nThreads; i++)
        thread_pool.emplace_back(&ParallelNonogramSolver::init_worker, this);

    for (auto &thread : thread_pool)
        thread.join();
}

void ParallelNonogramSolver::init_worker()
{
    // take possibility generation jobs
    // first for rows...
    init_work(true);

    // then collumns...
    init_work(false);

    init_barrier();
    return;
}

bool ParallelNonogramSolver::solve()
{
    std::vector<std::thread> thread_pool;

    // disparar threads

    for (int i = 0; i < nThreads; i++)
        thread_pool.emplace_back(&ParallelNonogramSolver::worker, this, i);

    for (auto &thread : thread_pool)
        thread.join();

    return isSolved();
}

void ParallelNonogramSolver::worker(int id)
{
    int index;
    std::stack<UpdateJob> phaseUpdates;

    // main dual-phasing loop
    while (true)
    {
        processPhase(phaseUpdates, true);

        if (completionCheckBarrier(false))
            break;

        processPhase(phaseUpdates, false);

        if (completionCheckBarrier(true))
            break;
    }
}

void ParallelNonogramSolver::init_barrier()
{
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
        solvers[index]->generatePossibilities();
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

bool ParallelNonogramSolver::completionCheckBarrier(bool checkForUniqueness)
{

    std::unique_lock<std::mutex> lock(mutex);

    threadsAtBarrier++;
    if (threadsAtBarrier < nThreads)
    {
        cond.wait(lock);
        return completed;
    }
    else if ((checkForUniqueness && changesMade == 0) || isSolved())
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
