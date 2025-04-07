#include <iostream>
#include "Rectangle.h"
#include "Menu.h"

void showMenu() {
    Rectangle r1, r2, result;
    int choice;
    do {
        std::cout << "\n====== МЕНЮ ======\n";
        std::cout << "1. Ввести прямоугольник 1\n";
        std::cout << "2. Ввести прямоугольник 2\n";
        std::cout << "3. Переместить прямоугольник 1\n";
        std::cout << "4. Изменить размеры прямоугольника 1\n";
        std::cout << "5. Построить наименьший прямоугольник, содержащий оба\n";
        std::cout << "6. Найти пересечение прямоугольников\n";
        std::cout << "7. Показать прямоугольники\n";
        std::cout << "0. Выход\n";
        std::cout << "Выберите действие: ";
        std::cin >> choice;

        double x1, y1, x2, y2;
        double dx, dy, w, h;

        switch (choice) {
        case 1:
            std::cout << "Введите координаты двух противоположных углов прямоугольника 1 (x1 y1 x2 y2): ";
            std::cin >> x1 >> y1 >> x2 >> y2;
            r1 = Rectangle(x1, y1, x2, y2);
            break;
        case 2:
            std::cout << "Введите координаты двух противоположных углов прямоугольника 2 (x1 y1 x2 y2): ";
            std::cin >> x1 >> y1 >> x2 >> y2;
            r2 = Rectangle(x1, y1, x2, y2);
            break;
        case 3:
            std::cout << "Введите смещения dx и dy: ";
            std::cin >> dx >> dy;
            r1.move(dx, dy);
            std::cout << "Прямоугольник 1 перемещён.\n";
            break;
        case 4:
            std::cout << "Введите новую ширину и высоту: ";
            std::cin >> w >> h;
            r1.resize(w, h);
            std::cout << "Размеры прямоугольника 1 изменены.\n";
            break;
        case 5:
            result = Rectangle::boundingBox(r1, r2);
            std::cout << "Наименьший прямоугольник, содержащий оба:\n";
            result.print();
            break;
        case 6:
            result = Rectangle::intersection(r1, r2);
            std::cout << "Область пересечения прямоугольников:\n";
            result.print();
            break;
        case 7:
            std::cout << "Прямоугольник 1: ";
            r1.print();
            std::cout << "Прямоугольник 2: ";
            r2.print();
            break;
        case 0:
            std::cout << "Завершение программы.\n";
            break;
        default:
            std::cout << "Неверный выбор. Повторите ввод.\n";
        }
    } while (choice != 0);
}
