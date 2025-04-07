#include "binstring.h"
#include <iostream>

BinString::BinString(const std::string& id, const std::string& binary)
    : SymbString(id, binary) {}

void BinString::show() const {
    std::cout << "Binary object - ID: " << id << ", Binary: " << data << '\n';
}
