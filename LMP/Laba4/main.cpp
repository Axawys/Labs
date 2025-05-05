#include <iostream>
#include "train.h"
#include "utils.h"

int main() {
    const int SIZE = 3;
    TRAIN trains[SIZE];

    std::cout << "Введите данные для " << SIZE << " поездов:\n";
    for (int i = 0; i < SIZE; ++i) {
        std::cout << "Поезд #" << i + 1 << ":\n";
        std::cin >> trains[i];
    }

    sortTrainsByDestination(trains, SIZE);

    std::cout << "\nВведите время в формате ЧЧММ, после которого вывести поезда: ";
    int inputTime;
    std::cin >> inputTime;

    std::cout << "\nПоезда, отправляющиеся после " << inputTime / 100 << ":";
    if (inputTime % 100 < 10) std::cout << "0";
    std::cout << inputTime % 100 << ":\n";

    printTrainsAfterTime(trains, SIZE, inputTime);

    return 0;
}
