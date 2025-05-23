#include "Hint.h"
#include <stdexcept>

Hint::Hint(const std::vector<int>& blocks) : blocks(blocks) {}

/// @brief Adds a hint of a block sequence.
/// @param size 
void Hint::addBlock(int size) {
    blocks.push_back(size);
}

/// @brief Returns the vector of blocks hints.
const std::vector<int>& Hint::getBlocks() const {
    return blocks;
}

/// @brief Returns the number of blocks on line.
int Hint::getSize() const {
    return blocks.size();
}

/// @brief Returns the size of the hint at a specific index.
/// @param index The index of the hint.
int Hint::getHint(int index) const {
    if (index < 0 || index >= blocks.size()) {
        throw std::out_of_range("Index of hint out of range");
    }
    return blocks[index];
}