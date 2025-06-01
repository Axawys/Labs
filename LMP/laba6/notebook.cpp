#include "notebook.h"
#include <iostream>

void Notebook::addContact(const Contact& contact) {
    contactsByName.insert({contact.getName(), contact});
    contactsByDOB.insert({contact.getDOB(), contact});
}

void Notebook::removeByName(const std::string& name) {
    auto range = contactsByName.equal_range(name);
    if (range.first == range.second) {
        std::cout << "Контакт не найден.\n";
        return;
    }
    for (auto it = range.first; it != range.second; ++it) {
        // Удаляем из contactsByDOB
        auto dobRange = contactsByDOB.equal_range(it->second.getDOB());
        for (auto dobIt = dobRange.first; dobIt != dobRange.second; ++dobIt) {
            if (dobIt->second.getName() == name) {
                contactsByDOB.erase(dobIt);
                break;
            }
        }
    }
    contactsByName.erase(range.first, range.second);
    std::cout << "Контакт(ы) удалены.\n";
}

void Notebook::searchByName(const std::string& name) const {
    auto range = contactsByName.equal_range(name);
    for (auto it = range.first; it != range.second; ++it) {
        it->second.print();
    }
}

void Notebook::searchByDOB(const std::string& dob) const {
    auto range = contactsByDOB.equal_range(dob);
    for (auto it = range.first; it != range.second; ++it) {
        it->second.print();
    }
}

void Notebook::searchByPhone(const std::string& phone) const {
    for (const auto& pair : contactsByName) {
        if (pair.second.getPhone() == phone) {
            pair.second.print();
        }
    }
}

void Notebook::printAllSortedByName() const {
    std::cout << "Сортировка по имени:\n";
    for (const auto& pair : contactsByName) {
        pair.second.print();
    }
}

void Notebook::printAllSortedByDOB() const {
    std::cout << "Сортировка по дате рождения:\n";
    for (const auto& pair : contactsByDOB) {
        pair.second.print();
    }
}
