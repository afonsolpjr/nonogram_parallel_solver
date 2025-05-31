#pragma once
#include <vector>

#include "Cell.h"
#include "Hint.h"
class Line
{
private:
    std::vector<Cell *> cells;
    int length;
    Hint hints;

public:
    Line(int length, bool init = false);

    void addCell(Cell *cell);
    void bindCell(int index, Cell *cell);

    void addHint(int block_size) { hints.addBlock(block_size); }
    const Hint &getHints() { return hints; }
    int getHintSize() const { return hints.getSize(); }
    int getHint(int index) const;
    int getLength() const { return length; }

    void setCell(int index);
    void blockCell(int index);

    std::string toString(bool zeros = false) const;
    void print() const;

    // definindo operador de indexacao
    Cell &operator[](int index) { return *cells[index]; }
    const Cell &operator[](int index) const { return *cells[index]; }
};