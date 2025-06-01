#ifndef NOTEBOOK_H
#define NOTEBOOK_H

#include "contact.h"
#include <map>
#include <string>

class Notebook {
public:
    void addContact(const Contact& contact);
    void removeByName(const std::string& name);
    void searchByName(const std::string& name) const;
    void searchByDOB(const std::string& dob) const;
    void searchByPhone(const std::string& phone) const;
    void printAllSortedByName() const;
    void printAllSortedByDOB() const;

private:
    std::multimap<std::string, Contact> contactsByName;
    std::multimap<std::string, Contact> contactsByDOB;
};

#endif
