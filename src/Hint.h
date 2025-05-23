#pragma once
#include <vector>

class Hint {
private:
    std::vector<int> blocks; // Each number represents a sequence of filled cells in a row or column
public:
    Hint() = default;
    Hint(const std::vector<int>& blocks);
    void addBlock(int size);
    int getSize() const; // Get the total number of hints
    int getHint(int index) const; // Get the size of the hint at a specific index
    const std::vector<int>& getBlocks() const;
};