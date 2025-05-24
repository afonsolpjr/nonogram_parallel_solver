#include <iostream>
#include <string>
#include "Nonogram.h"
#include <stdexcept>

// #define DEBUG 1

#ifdef DEBUG
#define DBG(x) std::cout << x << std::endl
#else
#define DBG(x)
#endif

Nonogram::Nonogram(int width, int height) : width(width), height(height)
{
    rows.reserve(height);
    columns.reserve(width);
    for (int i = 0; i < height; ++i)
    {
        Line line(width, true);
        rows.push_back(line);
    }
    for (int i = 0; i < width; ++i)
    {
        Line line(height, false);

        for (size_t j = 0; j < height; j++)
            line.bindCell(j, &this->getRow(j)[i]);

        columns.push_back(line);
    }
}

/// @brief Returns a reference to the row at the specified index.
/// @param index [int]
/// @return [Line&] Reference to the row at the specified index.
Line &Nonogram::getRow(int index)
{
    if (index < 0 || index >= height)
    {
        throw std::out_of_range("Index of row out of range");
    }
    return rows[index];
}
/// @brief Returns a reference to the column at the specified index.
/// @param index [int]
/// @return [Line&] Reference to the column at the specified index.
Line &Nonogram::getColumn(int index)
{
    if (index < 0 || index >= width)
    {
        throw std::out_of_range("Index of column out of range");
    }
    return columns[index];
}

/// @brief Prints the Nonogram grid to the console.
/// @details Each row is printed on a new line, and cells are represented as '1' (filled) or '0' (empty).
void Nonogram::print() const
{
    // Print the column hints above grid, and row hints to the left
    int maxRowHintSize = 0;
    for (const auto row : this->rows)
    {
        if (row.getHintSize() > maxRowHintSize)
            maxRowHintSize = row.getHintSize();
    }

    int maxColumnHintSize = 0;
    for (const auto column : this->columns)
    {
        if (column.getHintSize() > maxColumnHintSize)
            maxColumnHintSize = column.getHintSize();
    }

    // imprimindo dica das colunas
    for (int i = maxColumnHintSize; i > 0; i--)
    {
        std::string toPrint;

        for (int j = 0; j < maxRowHintSize; j++)
            toPrint += "  ";
        for (const auto column : this->columns)
        {
            int index = column.getHintSize() - i;
            DBG("Tentando acessar dica " << index << " em [" << column.getHints().toString() << "]\n");

            if (index >= 0)
            {
                toPrint += (std::to_string(column.getHint(index)) + " ");
            }
            else
                toPrint += "  ";
        }

        std::cout << toPrint << std::endl;
    }
    // imprimindo dicas das linhas e a linha:
    for( const auto row : this->rows )
    {
        std::string toPrint;
        for(size_t i = maxRowHintSize; i>0; i--)
        {
            int index = row.getHintSize() - i;
            if(index >= 0)
                toPrint += std::to_string(row.getHint(index)) + " ";
            else
                toPrint += "  ";
        }
        std::cout << toPrint;
        row.print();
    }

}

/// @brief Unset All cells
void Nonogram::unsetCells()
{
    for (size_t i = 0; i < this->height; i++)
        for (size_t j = 0; j < this->width; j++)
            (*this)[i][j].unset();
}