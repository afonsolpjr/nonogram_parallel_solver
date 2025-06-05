#include "Line.h"
#include <iostream>

Line::Line(int length, bool init)
{
    this->length = length;
    this->hints = Hint();
    if (init)
    {
        for (int i = 0; i < length; ++i)
        {
            Cell *cell = new Cell();
            cells.push_back(cell);
        }
    }
    else
        cells.resize(length);
}

void Line::setCell(int index)
{
    if (index >= 0 && index < length)
        cells[index]->set();
}

void Line::blockCell(int index)
{
    if (index >= 0 && index < length)
        cells[index]->block();
}

void Line::addCell(Cell *cell)
{
    if (cells.size() < length)
        cells.push_back(cell);
}

std::string Line::toString(bool zeros) const
{
    std::string result;
    for (const auto cell : cells)
        result += cell->toString() + ' ';
    return result;
}

void Line::print() const
{
    std::cout << this->toString() + '\n'
              << std::endl;
}

int Line::getHint(int index) const
{
    return hints.getHint(index);
}

void Line::bindCell(int index, Cell *cell)
{
    this->cells[index] = cell;
}