#include "Hint.h"
#include <stdexcept>

Hint::Hint(const std::vector<int>& blocks) : blocks(blocks) {}

void Hint::addBlock(int size) {
    blocks.push_back(size);
}

const std::vector<int>& Hint::getBlocks() const {
    return blocks;
}

int Hint::getSize() const {
    return blocks.size();
}

int Hint::getHint(int index) const {
    if (index < 0 || index >= blocks.size()) {
        throw std::out_of_range("Index of hint out of range");
    }
    return blocks[index];
}

std::string Hint::toString() const {
    std::string result;
    for (size_t i = 0; i < blocks.size(); ++i) {
        result += std::to_string(blocks[i]);
        if (i < blocks.size() - 1) {
            result += " ";
        }
    }
    return result;
}