#include <iostream>
#include <vector>
#include <cmath>
#include <stdexcept>

class Vector {
private:
    std::vector<double> data;
public:
    //конструкторы
    Vector(int n) : data(n, 0.0) {}  //нулевая размерность вектора

    Vector(std::initializer_list<double> init) : data(init) {}

    // геттер размерности
    int size() const { return data.size(); }

    //доступ к элементам по индексу
    double& operator[](int i) {
        if (i < 0 || i >= size()) throw std::out_of_range("Index out of range");
        return data[i];
    }

    const double& operator[](int i) const {
        if (i < 0 || i >= size()) throw std::out_of_range("Index out of range");
        return data[i];
    }

    //сложение и вычитание
    Vector operator+(const Vector& other) const {
        if (size() != other.size()) throw std::invalid_argument("Size mismatch");
        Vector result(size());
        for (int i = 0; i < size(); ++i)
            result[i] = data[i] + other[i];
        return result;
    }

    Vector operator-(const Vector& other) const {
        if (size() != other.size()) throw std::invalid_argument("Size mismatch");
        Vector result(size());
        for (int i = 0; i < size(); ++i)
            result[i] = data[i] - other[i];
        return result;
    }

    //умножение на скаляр
    Vector operator*(double scalar) const {
        Vector result(size());
        for (int i = 0; i < size(); ++i)
            result[i] = data[i] * scalar;
        return result;
    }

    //скалярное произведение
    double dot(const Vector& other) const {
        if (size() != other.size()) throw std::invalid_argument("Size mismatch");
        double result = 0.0;
        for (int i = 0; i < size(); ++i)
            result += data[i] * other[i];
        return result;
    }

    double norm() const {
        double sum = 0.0;
        for (double x : data)
            sum += x * x;
        return std::sqrt(sum);
    }

    //вывод вектора
    friend std::ostream& operator<<(std::ostream& os, const Vector& v) {
        os << "(";
        for (int i = 0; i < v.size(); ++i) {
            os << v[i];
            if (i < v.size() - 1) os << ", ";
        }
        os << ")";
        return os;
    }
};

int main() {
    Vector v1 = {1.0, 2.0, 3.0};
    Vector v2 = {4.0, 5.0, 6.0};

    std::cout << "v1 = " << v1 << std::endl;
    std::cout << "v2 = " << v2 << std::endl;

    Vector v3 = v1 + v2;
    std::cout << "v1 + v2 = " << v3 << std::endl;

    Vector v4 = v1 - v2;
    std::cout << "v1 - v2 = " << v4 << std::endl;

    Vector v5 = v1 * 2.5;
    std::cout << "v1 * 2.5 = " << v5 << std::endl;

    double dot_product = v1.dot(v2);
    std::cout << "v1 . v2 = " << dot_product << std::endl;

    std::cout << "||v1|| = " << v1.norm() << std::endl;

    return 0;
}
