#pragma once
#include <string>
#include <iostream>

class SymbString {
protected:
    std::string id;
    std::string data;
public:
    SymbString(const std::string& id, const std::string& data);
    virtual ~SymbString() = default;

    std::string getId() const;
    virtual void show() const;
    virtual void ShowOct() const;
    virtual void ShowDec() const;
    virtual void ShowHex() const;
};
