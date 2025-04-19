using System;
using System.IO;

class Program {
    static int[,] ReadMatrixFromFile(string filename, int size) {
        int[,] matrix = new int[size, size];
        string[] lines = File.ReadAllLines(filename);

        if(lines.Length != size)
            throw new Exception("Ошибка: количество строк в файле не равно размеру матрицы.");

        for(int i = 0; i < size; i++) {
            string[] parts = lines[i].Split(new[] { ' ', '\t' }, StringSplitOptions.RemoveEmptyEntries);
            if(parts.Length != size)
                throw new Exception($"Ошибка: в строке {i} не {size} элементов.");

            for(int j = 0; j < size; j++)
                matrix[i, j] = int.Parse(parts[j]);
        }

        return matrix;
    }

    static void PrintMatrix(int[,] matrix) {
        int size = matrix.GetLength(0);
        Console.WriteLine("\nМатрица:");
        for(int i = 0; i < size; i++) {
            for(int j = 0; j < size; j++)
                Console.Write($"{matrix[i, j],4}");
            Console.WriteLine();
        }
    }

    static void CheckMatchingRowsAndColumns(int[,] matrix) {
        int size = matrix.GetLength(0);
        Console.WriteLine("\nИндексы k, где k-я строка совпадает с k-м столбцом:");
        bool found = false;

        for(int k = 0; k < size; k++) {
            bool match = true;
            for(int i = 0; i < size; i++) {
                if(matrix[k, i] != matrix[i, k]) {
                    match = false;
                    break;
                }
            }

            if(match) {
                Console.Write(k+1 + " ");
                found = true;
            }
        }

        if(!found)
            Console.Write("Таких строк и столбцов нет.");
        Console.WriteLine();
    }

    static void SumRowsWithNegatives(int[,] matrix) {
        int size = matrix.GetLength(0);
        int totalSum = 0;
        Console.WriteLine("\nСуммы строк, содержащих хотя бы один отрицательный элемент:");

        for(int i = 0; i < size; i++) {
            bool hasNegative = false;
            int rowSum = 0;

            for(int j = 0; j < size; j++) {
                rowSum += matrix[i, j];
                if(matrix[i, j] < 0)
                    hasNegative = true;
            }

            if(hasNegative) {
                Console.WriteLine($"Строка {i + 1}: сумма = {rowSum}");
                totalSum += rowSum;
            }
        }

        Console.WriteLine($"\nОбщая сумма таких строк: {totalSum}");
    }

    static void Main() {
        const int size = 8;
        string filename = "array.txt";

        try {
            Console.WriteLine("Программа считывает матрицу из файла array.txt.");

            int[,] matrix = ReadMatrixFromFile(filename, size);
            PrintMatrix(matrix);

            CheckMatchingRowsAndColumns(matrix);
            SumRowsWithNegatives(matrix);

        } catch(Exception ex) {
            Console.WriteLine("Ошибка: " + ex.Message);
        }
    }
}