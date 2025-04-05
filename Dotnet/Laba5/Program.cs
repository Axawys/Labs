using System;

class Program{
    static void Main(){
        // Пример исходного массива
        int[] array = { 3, 0, 5, 7, 0, 2, 9, 4, 1 };
        int n = array.Length;

        // определение индекса максимального элемента
        int maxIndex = 0;
        for (int i = 1; i < n; i++){
            if (array[i] > array[maxIndex]){
                maxIndex = i;
            }
        }
        Console.WriteLine($"Индекс максимального элемента: {maxIndex}");

        // произведение элементов между первыми двумя нулями
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

        // Переставить элементы: сначала с нечётных позиций, затем с чётных
        int[] transformed = new int[n];
        int index = 0;

        // Чётные позиции 
        for (int i = 0; i < n; i += 2){
            transformed[index++] = array[i];
        }
        for (int i = 1; i < n; i += 2){
            transformed[index++] = array[i];
        }

        Console.WriteLine("Преобразованный массив:");
        Console.WriteLine(string.Join(", ", transformed));
    }
}
