#include <iostream>

using namespace std;

int main() {
    int n;
    cout << "Введите количество чисел Фибоначчи для вывода: ";
    cin >> n;

    if (n <= 0) {
        cout << "Пожалуйста, введите положительное число." << endl;
        return 1;
    }

    long long a = 0, b = 1;

    cout << "Первые " << n << " чисел Фибоначчи:" << endl;

    for (int i = 0; i < n; ++i) {
        cout << a;
        if (i < n - 1) {
            cout << ", ";
        }

        long long next = a + b;
        a = b;
        b = next;
    }

    cout << endl;
    return 0;
}