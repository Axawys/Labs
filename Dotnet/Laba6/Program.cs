using System;

class Program {
    static int[,] GenerateMatrix(int size) {
        Random rand = new Random();
        int[,] matrix = new int[size, size];

        int[] firstRow = { 1, 2, 3, 4, 5, 6, 7, 8 };
        for(int j = 0; j < size; j++)
            matrix[0, j] = firstRow[j];

        for(int i = 1; i < size; i++)
            for(int j = 0; j < size; j++)
                matrix[i, j] = rand.Next(-10, 11);

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
                Console.Write(k + " ");
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

        Console.WriteLine("Программа работает с матрицей 8x8, где первая строка задаётся вручную, а остальные — случайные.");

        int[,] matrix = GenerateMatrix(size);
        PrintMatrix(matrix);

        CheckMatchingRowsAndColumns(matrix);
        SumRowsWithNegatives(matrix);

        Console.WriteLine("\nПрограмма завершила работу!");
    }
}
