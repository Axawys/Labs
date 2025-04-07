#ifndef RECTANGLE_H
#define RECTANGLE_H

class Rectangle {
private:
    double x1, y1, x2, y2;

public:
    Rectangle();
    Rectangle(double x1, double y1, double x2, double y2);

    void move(double dx, double dy);
    void resize(double newWidth, double newHeight);
    void print() const;

    double width() const;
    double height() const;

    static Rectangle boundingBox(const Rectangle& r1, const Rectangle& r2);
    static Rectangle intersection(const Rectangle& r1, const Rectangle& r2);
};

#endif
