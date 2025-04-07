#include "symbstring.h"
#include <sstream>
#include <iomanip>

SymbString::SymbString(const std::string& id, const std::string& data)
    : id(id), data(data) {}

std::string SymbString::getId() const {
    return id;
}

void SymbString::show() const {
    std::cout << "ID: " << id << ", Data: " << data << '\n';
}

void SymbString::ShowOct() const {
    for (char c : data)
        std::cout << std::oct << int(c) << ' ';
    std::cout << '\n';
}

void SymbString::ShowDec() const {
    for (char c : data)
        std::cout << std::dec << int(c) << ' ';
    std::cout << '\n';
}

void SymbString::ShowHex() const {
    for (char c : data)
        std::cout << std::hex << int(c) << ' ';
    std::cout << '\n';
}
