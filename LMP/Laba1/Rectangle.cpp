#include <iostream>
#include <algorithm>
#include "Rectangle.h"

Rectangle::Rectangle() : x1(0), y1(0), x2(0), y2(0) {}

Rectangle::Rectangle(double a, double b, double c, double d) {
    x1 = std::min(a, c);
    y1 = std::min(b, d);
    x2 = std::max(a, c);
    y2 = std::max(b, d);
}

void Rectangle::move(double dx, double dy) {
    x1 += dx;
    y1 += dy;
    x2 += dx;
    y2 += dy;
}

void Rectangle::resize(double newWidth, double newHeight) {
    x2 = x1 + std::abs(newWidth);
    y2 = y1 + std::abs(newHeight);
}

void Rectangle::print() const {
    std::cout << "Rectangle: [(" << x1 << ", " << y1 << "), (" << x2 << ", " << y2 << ")]\n";
}

double Rectangle::width() const {
    return x2 - x1;
}

double Rectangle::height() const {
    return y2 - y1;
}

Rectangle Rectangle::boundingBox(const Rectangle& r1, const Rectangle& r2) {
    double nx1 = std::min(r1.x1, r2.x1);
    double ny1 = std::min(r1.y1, r2.y1);
    double nx2 = std::max(r1.x2, r2.x2);
    double ny2 = std::max(r1.y2, r2.y2);
    return Rectangle(nx1, ny1, nx2, ny2);
}

Rectangle Rectangle::intersection(const Rectangle& r1, const Rectangle& r2) {
    double nx1 = std::max(r1.x1, r2.x1);
    double ny1 = std::max(r1.y1, r2.y1);
    double nx2 = std::min(r1.x2, r2.x2);
    double ny2 = std::min(r1.y2, r2.y2);
    if (nx1 < nx2 && ny1 < ny2) {
        return Rectangle(nx1, ny1, nx2, ny2);
    } else {
        return Rectangle(0, 0, 0, 0); // no intersection
    }
}
