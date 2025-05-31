#include <iostream>
#include "../utils/NonogramRun.h"
#include "../utils/ParallelNonogramSolver.h"

#include <chrono>


void executeGame(int game_number)
{
    std::vector<RawPuzzleData> gamesData = NonogramPuzzleFactory::loadGamesFromFile("variooos.txt");

    NonogramRun execution(gamesData[21]);
    execution.run();
    execution.printStats();
    execution.puzzle.print();
}

void test_conc(){
    ParallelNonogramSolver::test(4);
}

int main()
{
    using namespace std::chrono;

    std::cout << "Hello, Nonogram Solver!" << std::endl;

    test_conc();
    return 0;
}
