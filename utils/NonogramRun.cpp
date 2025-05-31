#include "NonogramRun.h"
#include <iostream>
#include <chrono>

NonogramRun::NonogramRun(const RawPuzzleData gameData) : gameData(gameData)
{
    auto start = std::chrono::high_resolution_clock::now();
    puzzle = RandomGenerator::fromBool(gameData.grid, gameData.dimension);
    init_time = std::chrono::high_resolution_clock::now() - start;
}

void NonogramRun::run()
{
    auto start = std::chrono::high_resolution_clock::now();
    NonogramSolver solver(puzzle);
    solver.solve();
    solve_time = std::chrono::high_resolution_clock::now() - start;

    verifyCorrectness();
}

void NonogramRun::printStats() const
{
    printf("Tamanho do Jogo: %dx%d\n", puzzle.getHeight(), puzzle.getWidth());
    std::cout << "Tempo de inicialização: " << init_time.count() << "s\n";
    std::cout << "Tempo de resolução: " << solve_time.count() << "s\n";
    std::cout << "Solução " << (is_correct ? "CORRETA" : "INCORRETA") << "\n";
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