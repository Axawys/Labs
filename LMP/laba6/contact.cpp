#include "contact.h"
#include <iostream>

Contact::Contact(const std::string& name, const std::string& dob, const std::string& phone)
    : name(name), dob(dob), phone(phone) {}

std::string Contact::getName() const {
    return name;
}

std::string Contact::getDOB() const {
    return dob;
}

std::string Contact::getPhone() const {
    return phone;
}

void Contact::print() const {
    std::cout << "Имя: " << name << ", Дата рождения: " << dob << ", Телефон: " << phone << '\n';
}
