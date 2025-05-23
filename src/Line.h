#pragma once
#include <vector>
#include "Cell.h"
#include "Hint.h"
class Line
{
private:
    std::vector<Cell> cells;
    int length;
    Hint hints;
public:
    Line(int length, bool init);
    void setCell(int index);
    void addCell(const Cell& cell);
    void print() const;
    int getHint(int index) const;

    //definindo operador de indexacao
    Cell& operator[](int index) { return cells[index]; }
};