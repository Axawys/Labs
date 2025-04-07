#include "factory.h"
#include <iostream>
#include <algorithm>

void Factory::createSymbString(const std::string& id, const std::string& data) {
    objects.push_back(std::make_shared<SymbString>(id, data));
}

void Factory::createBinString(const std::string& id, const std::string& binary) {
    objects.push_back(std::make_shared<BinString>(id, binary));
}

void Factory::deleteById(const std::string& id) {
    objects.erase(
        std::remove_if(objects.begin(), objects.end(),
                       [&](const std::shared_ptr<SymbString>& obj) {
                           return obj->getId() == id;
                       }),
        objects.end());
}

void Factory::showAll() const {
    for (const auto& obj : objects)
        obj->show();
}

void Factory::showById(const std::string& id) const {
    for (const auto& obj : objects)
        if (obj->getId() == id) {
            obj->show();
            obj->ShowOct();
            obj->ShowDec();
            obj->ShowHex();
        }
}
