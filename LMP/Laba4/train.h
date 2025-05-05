#ifndef TRAIN_H
#define TRAIN_H

#include <string>
#include <iostream>

class TRAIN {
private:
    std::string destination;
    int trainNumber;
    int departureTime; // формат: HHMM, например 1430 — 14:30

public:
    TRAIN();
    TRAIN(const std::string& dest, int number, int time);

    void setDestination(const std::string& dest);
    void setTrainNumber(int number);
    void setDepartureTime(int time);

    std::string getDestination() const;
    int getTrainNumber() const;
    int getDepartureTime() const;

    friend std::ostream& operator<<(std::ostream& os, const TRAIN& train);
    friend std::istream& operator>>(std::istream& is, TRAIN& train);
};

#endif
