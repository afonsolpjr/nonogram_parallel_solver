#pragma once
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

    /**
     * @brief Creates an empty Nonogram object, allocating lines and collumns.
     * @param width
     * @param height
     */
    Nonogram(int width, int height);

    /// @brief Returns a reference to the row at the specified index.
    /// @param index [int]
    /// @return [Line&] Reference to the row at the specified index.
    Line &getRow(int index);

    /// @brief Returns a reference to the column at the specified index.
    /// @param index [int]
    /// @return [Line&] Reference to the column at the specified index.
    Line &getColumn(int index);

    int getWidth() const { return width; }
    int getHeight() const { return height; }
    Line &operator[](int index) { return rows[index]; }
    const Line &operator[](int index) const { return rows[index]; }

    /// @brief Prints the Nonogram grid to the console.
    /// @details Each row is printed on a new line, and cells are represented as '1' (filled) or '0' (empty).
    void print() const;

    /// @brief Unset All cells
    void unsetCells();
};