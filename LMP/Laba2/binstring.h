#pragma once
#include "symbstring.h"

class BinString : public SymbString {
public:
    BinString(const std::string& id, const std::string& binary);
    void show() const override;

    friend BinString operator+(const BinString& lhs, const BinString& rhs);

private:
    static std::string validateBinary(const std::string& binary);
};
