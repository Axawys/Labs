using System;
using System.IO;
using System.Text.RegularExpressions;

class Program {
    static void Main() {
        Console.WriteLine("Программа выводит предложения без запятых из текстового файла.");
        Console.Write("Введите путь к файлу: ");
        string path = Console.ReadLine();

        if(!File.Exists(path)) {
            Console.WriteLine("Файл не найден!");
            return;
        }

        string text = File.ReadAllText(path);
        string[] sentences = Regex.Split(text, @"(?<=[.!?])\s+");

        bool found = false;
        Console.WriteLine("\nПредложения без запятых:\n");
        foreach(string sentence in sentences) {
            if(!sentence.Contains(",")) {
                Console.WriteLine(sentence.Trim());
                found = true;
            }
        }

        if(!found)
            Console.WriteLine("Все предложения содержат запятую.");
    }
}
