#include "train.h"

TRAIN::TRAIN() : destination(""), trainNumber(0), departureTime(0) {}

TRAIN::TRAIN(const std::string& dest, int number, int time)
    : destination(dest), trainNumber(number), departureTime(time) {}

void TRAIN::setDestination(const std::string& dest) {
    destination = dest;
}

void TRAIN::setTrainNumber(int number) {
    trainNumber = number;
}

void TRAIN::setDepartureTime(int time) {
    departureTime = time;
}

std::string TRAIN::getDestination() const {
    return destination;
}

int TRAIN::getTrainNumber() const {
    return trainNumber;
}

int TRAIN::getDepartureTime() const {
    return departureTime;
}

std::ostream& operator<<(std::ostream& os, const TRAIN& train) {
    os << "Пункт назначения: " << train.destination << "\n";
    os << "Номер поезда: " << train.trainNumber << "\n";
    os << "Время отправления: " << (train.departureTime / 100) << ":";
    if (train.departureTime % 100 < 10)
        os << "0";
    os << (train.departureTime % 100) << "\n";
    return os;
}

std::istream& operator>>(std::istream& is, TRAIN& train) {
    std::cout << "Введите пункт назначения: ";
    is >> train.destination;
    std::cout << "Введите номер поезда: ";
    is >> train.trainNumber;
    int time;
while (true) {
    std::cout << "Введите время отправления (в формате ЧЧММ, например 1420): ";
    is >> time;
    int hours = time / 100;
    int minutes = time % 100;
    if (hours >= 0 && hours <= 23 && minutes >= 0 && minutes <= 59) {
        train.departureTime = time;
        break;
    } else {
        std::cout << "Ошибка: недопустимое время. Попробуйте снова.\n";
    }
}

    return is;
}
