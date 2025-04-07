#pragma once
#include "symbstring.h"

class BinString : public SymbString {
public:
    BinString(const std::string& id, const std::string& binary);
    void show() const override;
};
