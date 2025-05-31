#include <iostream>
#include <fstream>
#include <vector>
#include <climits>

using namespace std;

int main() {
    ifstream input("matrix.txt");
    if (!input.is_open()) {
        cerr << "Ошибка: файл не найден!" << endl;
        return 1;
    }

    int rows, cols;
    input >> rows >> cols;
    cout << "Размерность матрицы " << rows << " x " << cols << endl;

    vector<vector<int>> matrix(rows, vector<int>(cols));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            input >> matrix[i][j];
        }
    }
    input.close();

    vector<int> min_elements;
    for (int i = 0; i < rows; ++i) {
        int min_in_row = INT_MAX;
        for (int j = 0; j < cols; ++j) {
            if (matrix[i][j] < min_in_row) {
                min_in_row = matrix[i][j];
            }
        }
        min_elements.push_back(min_in_row);
    }

    int max_of_mins = INT_MIN;
    for (int num : min_elements) {
        if (num > max_of_mins) {
            max_of_mins = num;
        }
    }

    cout << "Минимальные элементы в строках:\n";
    for (int num : min_elements) {
        cout << num << " ";
    }
    cout << "\nМаксимальный среди них: " << max_of_mins << endl;

    return 0;
}