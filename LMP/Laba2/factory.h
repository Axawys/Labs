#pragma once
#include <vector>
#include <memory>
#include "symbstring.h"
#include "binstring.h"

class Factory {
private:
    std::vector<std::shared_ptr<SymbString>> objects;
public:
    void createSymbString(const std::string& id, const std::string& data);
    void createBinString(const std::string& id, const std::string& binary);
    void deleteById(const std::string& id);
    void showAll() const;
    void showById(const std::string& id) const;
    void addBinStrings(const std::string& id1, const std::string& id2);
};
