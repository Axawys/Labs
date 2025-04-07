#include <iostream>
#include <locale.h>
#include "factory.h"

int main() {
    setlocale(LC_ALL, "");
    Factory factory;
    int выбор;
    std::string id, данные;

    while (true) {
        std::cout << "\n1. Создать SymbString\n2. Создать BinString\n3. Удалить объект\n4. Показать все\n5. Показать по ID\n0. Выход\nВаш выбор: ";
        std::cin >> выбор;
        if (выбор == 0) break;

        switch (выбор) {
            case 1:
                std::cout << "Введите ID: "; std::cin >> id;
                std::cout << "Введите данные: "; std::cin >> данные;
                factory.createSymbString(id, данные);
                break;
            case 2:
                std::cout << "Введите ID: "; std::cin >> id;
                std::cout << "Введите бинарные данные: "; std::cin >> данные;
                factory.createBinString(id, данные);
                break;
            case 3:
                std::cout << "Введите ID для удаления: "; std::cin >> id;
                factory.deleteById(id);
                break;
            case 4:
                factory.showAll();
                break;
            case 5:
                std::cout << "Введите ID для отображения: "; std::cin >> id;
                factory.showById(id);
                break;
        }
    }

    return 0;
}
