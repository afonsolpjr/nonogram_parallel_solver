#include <iostream>
#include "../utils/NonogramRun.h"


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
    std::vector<RawPuzzleData> gamesData = NonogramPuzzleFactory::loadGamesFromFile("variooos.txt");

    int gameNumber = 101;

    auto sequential = new NonogramRun(gamesData[gameNumber]);
    auto parallel = new NonogramRun(gamesData[gameNumber],true,4);

    sequential->run();
    // sequential->puzzle.print();
    parallel->run();
    return 0;
}
