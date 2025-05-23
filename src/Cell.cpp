#include "Cell.h"

Cell::Cell() : is_set(false) {}

void Cell::set() {
    is_set = true;
}

void Cell::unset() {
    is_set = false;
}

bool Cell::isSet() const {
    return is_set;
}
