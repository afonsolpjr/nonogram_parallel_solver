#include "NonogramRun.h"
#include <iostream>
#include <chrono>
#include <sstream>
#include <iomanip>

NonogramRun::NonogramRun(const RawPuzzleData gameData, int nThreads) : gameData(gameData), nThreads(nThreads)
{
    puzzle = NonogramPuzzleFactory::fromBool(gameData.grid, gameData.dimension);
}

bool NonogramRun::run()
{
    if (nThreads==1)
    {

        ParallelNonogramSolver solver(puzzle, nThreads);
        auto start = std::chrono::high_resolution_clock::now();
        solver.init();
        init_time = std::chrono::high_resolution_clock::now() - start;

        start = std::chrono::high_resolution_clock::now();
        solver.solve();
        solve_time = std::chrono::high_resolution_clock::now() - start;
    }
    else if (nThreads>1)
    {
        nThreads=1;
        NonogramSolver solver(puzzle);
        auto start = std::chrono::high_resolution_clock::now();
        solver.init();
        init_time = std::chrono::high_resolution_clock::now() - start;

        start = std::chrono::high_resolution_clock::now();
        solver.solve();
        solve_time = std::chrono::high_resolution_clock::now() - start;
    }

    return verifyCorrectness();
}

void NonogramRun::printStats()
{
    printf("Tamanho do Jogo: %dx%d\n", puzzle.getHeight(), puzzle.getWidth());
    std::cout << "Tempo de inicialização: " << init_time.count() << "s\n";
    std::cout << "Tempo de resolução: " << solve_time.count() << "s\n";
    std::cout << "Total: " << (init_time + solve_time).count() << "s\n";
    std::cout << "Solução " << (is_correct ? "CORRETA" : "INCORRETA") << "\n\n";
}

bool NonogramRun::verifyCorrectness()
{
    std::vector<bool> puzzleVector = puzzleToVector(puzzle);
    if (NonogramPuzzleFactory::equalGrids(gameData.grid, puzzleVector))
        is_correct = true;
    return is_correct;
}

bool NonogramRun::verifyCorrectness(const RawPuzzleData gameData, const Nonogram &puzzle)
{
    std::vector<bool> puzzleVector = puzzleToVector(puzzle);
    if (NonogramPuzzleFactory::equalGrids(gameData.grid, puzzleVector))
        return true;
    else
        return false;
}

std::vector<bool> NonogramRun::puzzleToVector(const Nonogram &puzzle)
{
    std::vector<bool> result(puzzle.getHeight() * puzzle.getWidth(), false);

    for (int i = 0; i < puzzle.getHeight(); i++)
    {
        for (int j = 0; j < puzzle.getWidth(); j++)
        {
            if (puzzle[i][j].isSet())
                result[i * puzzle.getWidth() + j] = true;
        }
    }
    return result;
}

void NonogramRun::appendResult(FILE *filePtr)
{
    fputs(toCSV().c_str(),filePtr);
    fflush(filePtr);
}


const std::string NonogramRun::toCSV()
{
    std::ostringstream csvLine;
    csvLine << gameData.gameId << ","
            << gameData.dimension << ","
            << nThreads << ","
            << std::fixed << std::setprecision(6) << init_time.count() << ","
            << std::fixed << std::setprecision(6) << solve_time.count() << '\n';
    return csvLine.str();
}