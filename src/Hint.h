#pragma once
#include <vector>
#include <string>
class Hint {
private:
    std::vector<int> blocks; // Each number represents a sequence of filled cells in a row or column
public:
    Hint() = default;
    Hint(const std::vector<int>& blocks);
    void addBlock(int size);
    int getSize() const; // Get the total number of hints
    int getHint(int index) const; // Get the size of the hint at a specific index
    std::string toString() const; // Convert the hint to a string representation
    const std::vector<int>& getBlocks() const;
};