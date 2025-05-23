#pragma once

class Cell {
private:
    bool is_set;

public:
    Cell();
    void set();
    void unset();
    bool isSet() const;
};