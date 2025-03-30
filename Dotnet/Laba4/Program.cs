using System;

class Rectangle
{
    private int x, y, width, height; // координаты левого верхнего угла и размеры

    public Rectangle(int x, int y, int width, int height)
    {
        if (width <= 0 || height <= 0)
            throw new ArgumentException("Ширина и высота должны быть положительными");
        this.x = x;
        this.y = y;
        this.width = width;
        this.height = height;
    }

    public int X { get => x; set => x = value; }
    public int Y { get => y; set => y = value; }
    public int Width
    {
        get => width;
        set
        {
            if (value <= 0) throw new ArgumentException("Некорректная ширина");
            width = value;
        }
    }
    public int Height
    {
        get => height;
        set
        {
            if (value <= 0) throw new ArgumentException("Некорректная высота");
            height = value;
        }
    }

    public void Move(int dx, int dy) // перемещение
    {
        x += dx;
        y += dy;
    }

    public void Resize(int newWidth, int newHeight) // изменение размера
    {
        Width = newWidth;
        Height = newHeight;
    }

    public static Rectangle BoundingBox(Rectangle r1, Rectangle r2) // наименьший прямоугольник
    {
        int minX = Math.Min(r1.x, r2.x);
        int minY = Math.Min(r1.y, r2.y);
        int maxX = Math.Max(r1.x + r1.width, r2.x + r2.width);
        int maxY = Math.Max(r1.y + r1.height, r2.y + r2.height);
        return new Rectangle(minX, minY, maxX - minX, maxY - minY);
    }

    public static Rectangle? Intersection(Rectangle r1, Rectangle r2) // пересечение
    {
        int x1 = Math.Max(r1.x, r2.x);
        int y1 = Math.Max(r1.y, r2.y);
        int x2 = Math.Min(r1.x + r1.width, r2.x + r2.width);
        int y2 = Math.Min(r1.y + r1.height, r2.y + r2.height);

        if (x1 < x2 && y1 < y2)
            return new Rectangle(x1, y1, x2 - x1, y2 - y1);
        return null;
    }

    public override string ToString() => $"({x}, {y}), {width}x{height}";
}

class Program
{
    static void Main()
    {
        Rectangle r1 = new Rectangle(2, 3, 5, 4);
        Rectangle r2 = new Rectangle(4, 5, 6, 3);
        
        Console.WriteLine($"Прямоугольник 1: {r1}");
        Console.WriteLine($"Прямоугольник 2: {r2}");
        
        r1.Move(2, -1);
        Console.WriteLine("После перемещения r1: " + r1);
        
        r2.Resize(4, 6);
        Console.WriteLine("После изменения размера r2: " + r2);
        
        Rectangle bounding = Rectangle.BoundingBox(r1, r2);
        Console.WriteLine("Наименьший ограничивающий прямоугольник: " + bounding);
        
        Rectangle? intersect = Rectangle.Intersection(r1, r2);
        Console.WriteLine(intersect != null ? "Пересечение: " + intersect : "Нет пересечения");
        
        r1.Move(-3, 2);
        Console.WriteLine("После второго перемещения r1: " + r1);
        
        r1.Resize(7, 5);
        Console.WriteLine("После второго изменения размера r1: " + r1);
        
        Rectangle bounding2 = Rectangle.BoundingBox(r1, r2);
        Console.WriteLine("Наименьший охватывающий прямоугольник (после изменений): " + bounding2);
        
        Rectangle? intersect2 = Rectangle.Intersection(r1, r2);
        Console.WriteLine(intersect2 != null ? "Пересечение после изменений: " + intersect2 : "Нет пересечения после изменений");
    }
}
