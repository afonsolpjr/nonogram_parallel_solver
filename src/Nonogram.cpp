#include <iostream>
#include "Nonogram.h"
#include <stdexcept>


Nonogram::Nonogram(int width, int height) : width(width), height(height){}

/// @brief Returns a reference to the row at the specified index.
/// @param index [int]
/// @return [Line&] Reference to the row at the specified index.
Line& Nonogram::getRow(int index) {
    if (index < 0 || index >= height) {
        throw std::out_of_range("Index of row out of range");
    }
    return rows[index];
}
/// @brief Returns a reference to the column at the specified index.
/// @param index [int]
/// @return [Line&] Reference to the column at the specified index.
Line& Nonogram::getColumn(int index) {
    if (index < 0 || index >= width) {
        throw std::out_of_range("Index of column out of range");
    }
    return columns[index];
}

/// @brief Prints the Nonogram grid to the console.
/// @details Each row is printed on a new line, and cells are represented as '1' (filled) or '0' (empty).
void Nonogram::print() const {
    for (const auto& row : rows) {
        row.print();
        std::cout << '\n';
    }
}