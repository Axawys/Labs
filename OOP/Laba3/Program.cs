using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

public class Note{
    public string LastName { get; set; }
    public DateTime BirthDate { get; set; }
    public string PhoneNumber { get; set; }

    public Note(string lastName, DateTime birthDate, string phoneNumber){
        LastName = lastName ?? throw new ArgumentNullException(nameof(lastName));
        BirthDate = birthDate;
        PhoneNumber = phoneNumber ?? throw new ArgumentNullException(nameof(phoneNumber));
    }

    public override string ToString(){
        return $"Фамилия: {LastName}, Дата рождения: {BirthDate.ToShortDateString()}, Телефон: {PhoneNumber}";
    }
}

public class Notebook{
    private List<Note> notes = new List<Note>();

    public int Count => notes.Count;

    public Note this[int index]{
        get{
            if (index < 0 || index >= notes.Count)
                throw new IndexOutOfRangeException("Недопустимый индекс записи");
            return notes[index];
        }
    }

    // метод загрузки данных
    public void LoadFromFile(string filePath){
        if (!File.Exists(filePath))
            throw new FileNotFoundException("Файл с данными не найден");

        var lines = File.ReadAllLines(filePath);
        foreach (var line in lines){
            if (string.IsNullOrWhiteSpace(line)) continue;

            var parts = line.Split(',');
            if (parts.Length != 3)
                throw new FormatException("Некорректный формат данных в файле");

            try{
                var lastName = parts[0].Trim();
                var birthDate = DateTime.ParseExact(parts[1].Trim(), "dd.MM.yyyy", null);
                var phoneNumber = parts[2].Trim();
                
                notes.Add(new Note(lastName, birthDate, phoneNumber));
            }
            catch (Exception ex){
                throw new FormatException($"Ошибка при обработке строки: {line}", ex);
            }
        }
    }

    public void AddNote(Note note){
        if (note == null)
            throw new ArgumentNullException(nameof(note));
        
        notes.Add(note);
    }

    public void RemoveNote(int index){
        if (index < 0 || index >= notes.Count)
            throw new IndexOutOfRangeException("Недопустимый индекс записи");
        
        notes.RemoveAt(index);
    }

    public IEnumerable<Note> FindByLastName(string lastName){
        if (string.IsNullOrWhiteSpace(lastName))
            throw new ArgumentException("Фамилия не может быть пустой");
        
        return notes.Where(n => n.LastName.Equals(lastName, StringComparison.OrdinalIgnoreCase));
    }

    public IEnumerable<Note> FindByBirthDate(DateTime birthDate){
        return notes.Where(n => n.BirthDate.Date == birthDate.Date);
    }

    public IEnumerable<Note> FindByPhoneNumber(string phoneNumber){
        if (string.IsNullOrWhiteSpace(phoneNumber))
            throw new ArgumentException("Номер телефона не может быть пустым");
        
        return notes.Where(n => n.PhoneNumber.Contains(phoneNumber));
    }

    public void SortByLastName(){
        notes = notes.OrderBy(n => n.LastName).ToList();
    }
}

class Program{
    static void Main(string[] args){
        var notebook = new Notebook();
        
        try{
            // загрузка данных из файла
            notebook.LoadFromFile("data.txt");
            
            Console.WriteLine($"Загружено записей: {notebook.Count}\n");

            Console.WriteLine("Все записи:");
            for (int i = 0; i < notebook.Count; i++){
                Console.WriteLine($"{i + 1}. {notebook[i]}");
            }

            // поиск
            Console.WriteLine("\nПоиск по фамилии 'Иванов':");
            foreach (var note in notebook.FindByLastName("Иванов")){
                Console.WriteLine(note);
            }

            Console.WriteLine("\nПоиск по дате рождения 08.03.1995:");
            foreach (var note in notebook.FindByBirthDate(new DateTime(1995, 3, 8))){
                Console.WriteLine(note);
            }

            Console.WriteLine("\nПоиск по номеру телефона '555':");
            foreach (var note in notebook.FindByPhoneNumber("555")){
                Console.WriteLine(note);
            }

            // сортировка
            notebook.SortByLastName();
            Console.WriteLine("\nЗаписи после сортировки по фамилии:");
            for (int i = 0; i < notebook.Count; i++){
                Console.WriteLine($"{i + 1}. {notebook[i]}");
            }

            // удаление
            notebook.RemoveNote(1);
            Console.WriteLine("\nПосле удаления второй записи:");
            for (int i = 0; i < notebook.Count; i++){
                Console.WriteLine($"{i + 1}. {notebook[i]}");
            }
        }
        catch (Exception ex){
            Console.WriteLine($"Ошибка: {ex.Message}");
        }
    }
}