#include <iostream>
#include "../utils/RandomGenerator.cpp"
#include "../utils/NonogramSolver.cpp"
#include "../utils/CombinationGenerator.cpp"

int main() {

    int lenght;

    std::cout << "Hello, Nonogram Solver!" << std::endl;
    //seeding rng 
    std::srand(static_cast<unsigned int>(time(0))); 

    std::cout << "Insira a dimensao do jogo quadrado:" << std::endl;
    std::cin >> lenght;
    Nonogram nonogram = RandomGenerator::random(lenght,lenght);
    
    nonogram.print();

    NonogramSolver solver(nonogram);

    solver.solve();

    return 0;
}