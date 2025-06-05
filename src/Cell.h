#pragma once
#include <string>

class Cell
{
private:
    int state = 0;

public:
    Cell() = default;

    void set() { state = 1; }
    void unset() { state = 0; }
    void block() { state = -1; }

    bool isSet() const { return state == 1; }
    bool isEmpty() const { return state == 0; }
    bool isBlocked() const { return state == -1; }

    std::string toString(bool zeros = false)
    {

        switch (state)
        {
        case -1:
            return zeros ? "0" : "-";
            break;
        case 0:
            return zeros ? "0" : ".";
            break;
        case 1:
            return zeros ? "1" : "#";
            break;
        }

        return "erro";
    }

    int getState() const { return state; }
};