#include <iostream>
#include "../utils/NonogramRun.h"
#include <chrono>

int main()
{
    std::cout << "Hello, Nonogram Solver!" << std::endl;
    std::vector<RawPuzzleData> gamesData = NonogramPuzzleFactory::loadGamesFromFile("Games.txt");

    int gameNumber = 0;

    std::cin >> gameNumber;

    auto sequential = new NonogramRun(gamesData[gameNumber]);
    auto parallel = new NonogramRun(gamesData[gameNumber], true, 8);

    std::cout << "Running game number: " << gameNumber << std::endl;
    sequential->run();
    // sequential->puzzle.print();
    parallel->run();
    return 0;
}
