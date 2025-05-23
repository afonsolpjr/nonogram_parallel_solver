#include "Line.h"
#include <iostream>

Line::Line(int length, bool init=false){
    this->length = length;
    if (init) {
        for (int i = 0; i < length; ++i) {
            Cell cell;
            cells.push_back(cell);
        }
    }
    else
        cells.reserve(length);
}


/// @brief  Sets the cell at the specified index.
/// @param index 
void Line::setCell(int index)
{
    if (index >= 0 && index < length)
        cells[index].set();
}

/// @brief  Adds a cell to the line.
/// @param cell
void Line::addCell(const Cell& cell)
{
    if (cells.size() < length)
        cells.push_back(cell);
}

/// @brief  Prints the line to the console.
void Line::print() const
{
    for (const auto &cell : cells)
        std::cout << (cell.isSet() ? '1' : '0');
    std::cout << std::endl;
}

/// @brief  Returns the hint at the specified index.
int Line::getHint(int index) const
{
    return hints.getHint(index);
}
