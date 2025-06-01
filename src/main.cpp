#include <iostream>
#include "../utils/NonogramRun.h"
#include "../utils/solvers/ParallelNonogramSolver.h"
#include "../utils/solvers/NonogramSolver.h"


#include <chrono>


void executeGame(int game_number)
{
    std::vector<RawPuzzleData> gamesData = NonogramPuzzleFactory::loadGamesFromFile("variooos.txt");

    NonogramRun execution(gamesData[game_number]);
    execution.run();
    execution.printStats();
    execution.puzzle.print();
}

int main()
{
    using namespace std::chrono;

    std::cout << "Hello, Nonogram Solver!" << std::endl;

    executeGame(106);
    return 0;
}
