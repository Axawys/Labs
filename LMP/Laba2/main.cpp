#include <iostream>
#include <locale.h>
#include "factory.h"

int main() {
    setlocale(LC_ALL, "");
    Factory factory;
    int choice;
    std::string id, data;

    while (true) {
        std::cout << "\n1. Create SymbString\n2. Create BinString\n3. Delete object\n4. Show all\n5. Show by ID\n0. Exit\nChoice: ";
        std::cin >> choice;
        if (choice == 0) break;

        switch (choice) {
            case 1:
                std::cout << "Enter ID: "; std::cin >> id;
                std::cout << "Enter data: "; std::cin >> data;
                factory.createSymbString(id, data);
                break;
            case 2:
                std::cout << "Enter ID: "; std::cin >> id;
                std::cout << "Enter binary data: "; std::cin >> data;
                factory.createBinString(id, data);
                break;
            case 3:
                std::cout << "Enter ID to delete: "; std::cin >> id;
                factory.deleteById(id);
                break;
            case 4:
                factory.showAll();
                break;
            case 5:
                std::cout << "Enter ID to show: "; std::cin >> id;
                factory.showById(id);
                break;
        }
    }

    return 0;
}
