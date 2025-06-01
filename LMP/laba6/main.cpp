#include "notebook.h"
#include <iostream>

int main() {
    Notebook nb;

    while (true) {
        std::cout << "\n1. Добавить контакт\n2. Удалить по имени\n3. Поиск по имени\n4. Поиск по дате рождения\n"
                     "5. Поиск по телефону\n6. Сортировать по имени\n7. Сортировать по дате\n8. Выход\nВыбор: ";

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        if (choice == 1) {
            std::string name, dob, phone;
            std::cout << "Имя: "; std::getline(std::cin, name);
            std::cout << "Дата рождения (дд.мм.гггг): "; std::getline(std::cin, dob);
            std::cout << "Телефон: "; std::getline(std::cin, phone);
            nb.addContact(Contact(name, dob, phone));
        } else if (choice == 2) {
            std::string name;
            std::cout << "Введите имя: ";
            std::getline(std::cin, name);
            nb.removeByName(name);
        } else if (choice == 3) {
            std::string name;
            std::cout << "Введите имя: ";
            std::getline(std::cin, name);
            nb.searchByName(name);
        } else if (choice == 4) {
            std::string dob;
            std::cout << "Введите дату рождения: ";
            std::getline(std::cin, dob);
            nb.searchByDOB(dob);
        } else if (choice == 5) {
            std::string phone;
            std::cout << "Введите номер телефона: ";
            std::getline(std::cin, phone);
            nb.searchByPhone(phone);
        } else if (choice == 6) {
            nb.printAllSortedByName();
        } else if (choice == 7) {
            nb.printAllSortedByDOB();
        } else if (choice == 8) {
            break;
        } else {
            std::cout << "Неверный ввод.\n";
        }
    }

    return 0;
}
