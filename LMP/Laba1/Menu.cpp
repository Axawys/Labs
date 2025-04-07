#include <iostream>
#include "Rectangle.h"
#include "Menu.h"

void showMenu() {
    Rectangle r1, r2, result;
    int choice;
    do {
        std::cout << "\nМеню:\n";
        std::cout << "1. Задать прямоугольник 1\n";
        std::cout << "2. Задать прямоугольник 2\n";
        std::cout << "3. Переместить прямоугольник 1\n";
        std::cout << "4. Изменить размеры прямоугольника 1\n";
        std::cout << "5. Построить минимальный прямоугольник, содержащий оба\n";
        std::cout << "6. Построить пересечение прямоугольников\n";
        std::cout << "7. Показать оба прямоугольника\n";
        std::cout << "0. Выход\n";
        std::cout << "Ваш выбор: ";
        std::cin >> choice;

        double x1, y1, x2, y2;
        double dx, dy, w, h;

        switch (choice) {
        case 1:
            std::cout << "Введите координаты двух противоположных углов: ";
            std::cin >> x1 >> y1 >> x2 >> y2;
            r1 = Rectangle(x1, y1, x2, y2);
            break;
        case 2:
            std::cout << "Введите координаты двух противоположных углов: ";
            std::cin >> x1 >> y1 >> x2 >> y2;
            r2 = Rectangle(x1, y1, x2, y2);
            break;
        case 3:
            std::cout << "Введите смещение dx и dy: ";
            std::cin >> dx >> dy;
            r1.move(dx, dy);
            break;
        case 4:
            std::cout << "Введите новую ширину и высоту: ";
            std::cin >> w >> h;
            r1.resize(w, h);
            break;
        case 5:
            result = Rectangle::boundingBox(r1, r2);
            std::cout << "Минимальный прямоугольник: ";
            result.print();
            break;
        case 6:
            result = Rectangle::intersection(r1, r2);
            std::cout << "Общий прямоугольник: ";
            result.print();
            break;
        case 7:
            std::cout << "Прямоугольник 1: ";
            r1.print();
            std::cout << "Прямоугольник 2: ";
            r2.print();
            break;
        case 0:
            std::cout << "Выход.\n";
            break;
        default:
            std::cout << "Неверный выбор\n";
        }
    } while (choice != 0);
}
