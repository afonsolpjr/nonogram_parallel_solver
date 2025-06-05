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

    /**
     * @brief Creates a line and optionally its cells.
     * @param length length of the line
     * @param init if true, initialize cells. otherwise, only allocates space.
     */
    Line(int length, bool init = false);

    /// @brief  Adds a cell to the line.
    /// @param cell
    void addCell(Cell *cell);

    /// @brief Sets, on the cells* vector, the i-th cell pointer as the 'cell' parameter.
    /// @param index Index of the cell on this line
    /// @param cell pointer to a cell structure
    void bindCell(int index, Cell *cell);

    
    /**
     * @brief Adds a hint to the line.
     * @param block_size 
     */
    void addHint(int block_size) { hints.addBlock(block_size); }
    
    
    /**
     * @brief 
     * @return Return Hints structure.
     */
    const Hint &getHints() { return hints; }
    int getHintSize() const { return hints.getSize(); }

    /// @brief  Returns the hint at the specified index.
    int getHint(int index) const;

    /// @brief Returns line length.
    int getLength() const { return length; }

    /// @brief  Sets the cell at the specified index.
    /// @param index
    void setCell(int index);

    /// @brief Blocks the cell at the specified index.
    /// @param index
    void blockCell(int index);

    /// @brief returns the string representation of the line.
    /// @return
    std::string toString(bool zeros = false) const;

    /// @brief  Prints the line to the console.
    void print() const;

    // Overrides for [] operator
    Cell &operator[](int index) { return *cells[index]; }
    const Cell &operator[](int index) const { return *cells[index]; }
};