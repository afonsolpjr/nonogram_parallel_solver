#include "Line.h"

class Nonogram
{
private:
    std::vector<Line> rows;        // Linhas do grid
    std::vector<Line> columns;     // Colunas do grid
    std::vector<Hint> rowHints;    // Dicas das linhas
    std::vector<Hint> columnHints; // Dicas das coluna
    int width;
    int height;

public:
    // construtores
    Nonogram() = default;
    Nonogram(int width, int height);

    Line &getRow(int index);
    Line &getColumn(int index);

    int getWidth() const { return width; }
    int getHeight() const { return height; }
    Line &operator[](int index) { return rows[index]; }

    void print() const;
    void unsetCells();
};