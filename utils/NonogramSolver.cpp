#include "LineSolver.cpp"
#include "../src/Nonogram.h"
#include <vector>
class NonogramSolver
{
public:
    Nonogram *nonogram;
    std::vector<LineSolver *> row_solvers;
    std::vector<LineSolver *> column_solvers;
    NonogramSolver(Nonogram &nonogram_ref) : nonogram(&nonogram_ref)
    {
        for (int i = 0; i < nonogram->getHeight(); i++)
            row_solvers.push_back(new LineSolver(nonogram->getRow(i)));

        for (int i = 0; i < nonogram->getWidth(); i++)
            column_solvers.push_back(new LineSolver(nonogram->getColumn(i)));    
    }
};
