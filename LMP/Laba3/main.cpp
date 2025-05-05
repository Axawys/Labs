#include <iostream>
#include <string>
#include "Stack.hpp"

int main() {
    try {
        Stack<int> s1;
        s1.push(1);
        s1.push(2);
        std::cout << "Top int: " << s1.top() << std::endl;
        s1.pop();
        s1.pop();
        s1.pop();  // вызовет исключение
    } catch (const StackEmptyException& e) {
        std::cerr << "Исключение: " << e.what() << std::endl;
    }

    try {
        Stack<double> s2;
        s2.push(3.14);
        std::cout << "Top double: " << s2.top() << std::endl;
    } catch (const StackEmptyException& e) {
        std::cerr << "Исключение: " << e.what() << std::endl;
    }

    try {
        Stack<std::string> s3;
        s3.push("hello");
        s3.push("world");
        std::cout << "Top string: " << s3.top() << std::endl;
        s3.pop();
        std::cout << "Top после pop: " << s3.top() << std::endl;
    } catch (const StackEmptyException& e) {
        std::cerr << "Исключение: " << e.what() << std::endl;
    }

    return 0;
}
