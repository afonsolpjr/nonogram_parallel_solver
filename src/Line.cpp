#include "Line.h"
#include <iostream>

Line::Line(int length, bool init=false){
    this->length = length;
    this->hints = Hint();
    if (init) {
        for (int i = 0; i < length; ++i) {
            Cell* cell = new Cell();
            cells.push_back(cell);
        }
    }
    else
        cells.resize(length);
}


/// @brief  Sets the cell at the specified index.
/// @param index 
void Line::setCell(int index)
{
    if (index >= 0 && index < length)
        cells[index]->set();
}

void Line::blockCell(int index)
{
    if(index >= 0 && index < length)
        cells[index]->setBlocked();

    
}

/// @brief  Adds a cell to the line.
/// @param cell
void Line::addCell(Cell* cell)
{
    if (cells.size() < length)
        cells.push_back(cell);
}

/// @brief returns the string representation of the line.
/// @return 
std::string Line::toString(bool zeros) const
{
    std::string result;
    for ( const auto cell : cells)
        result += cell->toString() + ' ';
    return result;
} 

/// @brief  Prints the line to the console.
void Line::print() const
{
    std::cout << this->toString() + '\n' << std::endl;
}

/// @brief  Returns the hint at the specified index.
int Line::getHint(int index) const
{
    return hints.getHint(index);
}

void Line::bindCell(int index, Cell* cell){
    this->cells[index] = cell;
}