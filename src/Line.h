#pragma once
#include <vector>

#include "Cell.h"
#include "Hint.h"
class Line
{
private:
    std::vector<Cell*> cells;
    int length;
    Hint hints;
public:
    Line(int length, bool init);

    void setCell(int index);
    void blockCell(int index);
    void addCell(Cell* cell);
    void bindCell(int index, Cell* cell);
    void print() const;
    void addHint(int block_size) { hints.addBlock(block_size); }
    int getHint(int index) const;
    Hint getHints() const { return hints; }
    int getHintSize() const { return hints.getSize(); }
    int getLength() const { return length; }
    std::string toString(bool zeros = false) const;

    //definindo operador de indexacao
    Cell& operator[](int index) { return *cells[index]; }
    const Cell& operator[](int index) const { return *cells[index]; }

};