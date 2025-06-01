#ifndef CONTACT_H
#define CONTACT_H

#include <string>

class Contact {
public:
    Contact() = default;
    Contact(const std::string& name, const std::string& dob, const std::string& phone);

    std::string getName() const;
    std::string getDOB() const;
    std::string getPhone() const;

    void print() const;

private:
    std::string name;
    std::string dob;    // Date of birth (дд.мм.гггг)
    std::string phone;
};

#endif
