#pragma once
#include <vector>
#include <string>
class Hint
{
private:
    std::vector<int> blocks; // Each number represents a sequence of filled cells in a row or column
public:
    Hint() = default;

    /**
     * @brief Initialize a hint for a line.
     * @param blocks An ordernet int vector, containing the hitns (sizes of continuous blocks).
     */
    Hint(const std::vector<int> &blocks);

    /// @brief Adds a hint of a block sequence.
    /// @param size
    void addBlock(int size);

    /// @brief Returns the number of blocks on line.
    int getSize() const; // Get the total number of hints

    /// @brief Returns the size of the hint at a specific index.
    /// @param index The index of the hint.
    int getHint(int index) const; // Get the size of the hint at a specific index

    ///@brief Converts a Hint to a line.
    /// @return String containing the hints separated by space.
    std::string toString() const; // Convert the hint to a string representation

    /// @brief Returns the vector of blocks hints.
    const std::vector<int> &getBlocks() const;
};