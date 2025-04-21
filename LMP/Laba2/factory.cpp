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

void Factory::addBinStrings(const std::string& id1, const std::string& id2) {
    BinString *b1 = nullptr, *b2 = nullptr;

    for (const auto& obj : objects) {
        if (obj->getId() == id1)
            b1 = dynamic_cast<BinString*>(obj.get());
        else if (obj->getId() == id2)
            b2 = dynamic_cast<BinString*>(obj.get());
    }

    if (!b1 || !b2) {
        std::cout << "Один или оба объекта не найдены или не являются BinString.\n";
        return;
    }

    BinString result = *b1 + *b2;
    objects.push_back(std::make_shared<BinString>(result));
    std::cout << "Результат сложения добавлен как новый BinString:\n";
    result.show();
}
