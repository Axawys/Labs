#include "binstring.h"
#include <iostream>
#include <stdexcept>
#include <bitset>
#include <algorithm>

BinString::BinString(const std::string& id, const std::string& binary)
    : SymbString(id, validateBinary(binary)) {}

void BinString::show() const {
    std::cout << "Binary object - ID: " << id << ", Binary: " << data << '\n';
}

std::string BinString::validateBinary(const std::string& binary) {
    for (char c : binary) {
        if (c != '0' && c != '1') {
            throw std::invalid_argument("Ошибка: BinString может содержать только символы '0' и '1'.");
        }
    }
    return binary;
}

static std::string intToBinaryString(int num) {
    if (num == 0) return "0";

    std::string result;
    while (num > 0) {
        result += (num % 2 == 0 ? '0' : '1');
        num /= 2;
    }
    std::reverse(result.begin(), result.end());
    return result;
}

BinString operator+(const BinString& lhs, const BinString& rhs) {
    int lhsVal = std::stoi(lhs.data, nullptr, 2);
    int rhsVal = std::stoi(rhs.data, nullptr, 2);
    int sum = lhsVal + rhsVal;

    std::string resultData = intToBinaryString(sum);
    return BinString(lhs.id + "+" + rhs.id, resultData);
}
