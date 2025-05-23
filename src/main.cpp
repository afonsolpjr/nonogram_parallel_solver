#include <iostream>
#include "../utils/RandomGenerator.cpp"
int main() {


    std::cout << "Hello, Nonogram Solver!" << std::endl;
    //seeding rng 
    std::srand(static_cast<unsigned int>(time(0))); 


    Nonogram nonogram = RandomGenerator::random(5, 5);
    
    return 0;
}