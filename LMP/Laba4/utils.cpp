#include "utils.h"
#include <algorithm>

void sortTrainsByDestination(TRAIN arr[], int size) {
    std::sort(arr, arr + size, [](const TRAIN& a, const TRAIN& b) {
        return a.getDepartureTime() < b.getDepartureTime();
    });
}

void printTrainsAfterTime(const TRAIN arr[], int size, int time) {
    bool found = false;
    for (int i = 0; i < size; ++i) {
        if (arr[i].getDepartureTime() > time) {
            std::cout << arr[i] << std::endl;
            found = true;
        }
    }
    if (!found) {
        std::cout << "Нет поездов, отправляющихся после заданного времени.\n";
    }
}
