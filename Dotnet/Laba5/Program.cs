using System;
using System.IO;
using System.Linq;

class Program{
    static void Main(){
        string filePath = "array.txt";

        if (!File.Exists(filePath)){
            Console.WriteLine($"Файл '{filePath}' не найден.");
            return;
        }

        try {
            // Считываем массив из файла
            string fileContent = File.ReadAllText(filePath);
            int[] array = fileContent
                .Split(new[] { ' ', '\t', '\r', '\n', ',' }, StringSplitOptions.RemoveEmptyEntries)
                .Select(int.Parse)
                .ToArray();
            int n = array.Length;

            // Определение индекса максимального элемента
            int maxIndex = 0;
            for (int i = 1; i < n; i++){
                if (array[i] > array[maxIndex]){
                    maxIndex = i;
                }
            }
            Console.WriteLine($"Индекс максимального элемента: {maxIndex}");

            // Произведение элементов между первыми двумя нулями
            int firstZero = -1, secondZero = -1;
            for (int i = 0; i < n; i++){
                if (array[i] == 0){
                    if (firstZero == -1)
                        firstZero = i;
                    else{
                        secondZero = i;
                        break;
                    }
                }
            }

            if (firstZero != -1 && secondZero != -1 && secondZero - firstZero > 1){
                int product = 1;
                for (int i = firstZero + 1; i < secondZero; i++){
                    product *= array[i];
                }
                Console.WriteLine($"Произведение элементов между первым и вторым нулями: {product}");
            }
            else{
                Console.WriteLine("Произведение невозможно вычислить (меньше двух нулей или между ними нет элементов).");
            }

            // Переставить элементы: сначала с чётных позиций, затем с нечётных
            int[] transformed = new int[n];
            int index = 0;

            for (int i = 0; i < n; i += 2){
                transformed[index++] = array[i];
            }
            for (int i = 1; i < n; i += 2){
                transformed[index++] = array[i];
            }

            Console.WriteLine("Преобразованный массив:");
            Console.WriteLine(string.Join(", ", transformed));
        }
        catch (Exception ex){
            Console.WriteLine($"Ошибка при обработке файла: {ex.Message}");
        }
    }
}
