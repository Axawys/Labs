using System;

class Rectangle
{
    private double x1, y1; // левый нижний угол
    private double x2, y2; // правый верхний угол

    // конструктор без параметров (единичный квадрат)
    public Rectangle()
    {
        x1 = 0; y1 = 0;
        x2 = 1; y2 = 1;
    }

    // конструктор с параметрами
    public Rectangle(double x1, double y1, double x2, double y2)
    {
        if (x1 >= x2 || y1 >= y2)
            throw new ArgumentException("неправильные координаты прямоугольника");
            
        this.x1 = x1;
        this.y1 = y1;
        this.x2 = x2;
        this.y2 = y2;
    }
    
    public void Move(double dx, double dy)
    {
        x1 += dx; x2 += dx;
        y1 += dy; y2 += dy;
    }

    // изменение размеров (сохраняя левый верхний угол)
    public void Resize(double dw, double dh)
    {
        if (Width + dw <= 0 || Height + dh <= 0)
            throw new ArgumentException("размеры должны быть положительными");
            
        // Изменяем правую границу по X (x2) и нижнюю границу по Y (y1)
        x2 += dw;
        y1 -= dh; // уменьшаем y1, так как ось Y направлена вверх
    }

    // минимальный прямоугольник, содержащий два других
    public static Rectangle GetBoundingRectangle(Rectangle r1, Rectangle r2)
    {
        return new Rectangle(
            Math.Min(r1.x1, r2.x1),
            Math.Min(r1.y1, r2.y1),
            Math.Max(r1.x2, r2.x2),
            Math.Max(r1.y2, r2.y2)
        );
    }

    // пересечение двух прямоугольников
    public static Rectangle GetIntersection(Rectangle r1, Rectangle r2)
    {
        double newX1 = Math.Max(r1.x1, r2.x1);
        double newY1 = Math.Max(r1.y1, r2.y1);
        double newX2 = Math.Min(r1.x2, r2.x2);
        double newY2 = Math.Min(r1.y2, r2.y2);

        if (newX1 >= newX2 || newY1 >= newY2)
            throw new InvalidOperationException("прямоугольники не пересекаются");
            
        return new Rectangle(newX1, newY1, newX2, newY2);
    }

    // свойства для доступа к параметрам
    public double Left => x1;
    public double Bottom => y1;
    public double Right => x2;
    public double Top => y2;
    public double Width => x2 - x1;
    public double Height => y2 - y1;

    // строковое представление
    public override string ToString()
    {
        return $"Rectangle[({x1},{y1})-({x2},{y2})]";
    }
}

class Program
{
    static void Main()
    {
        try
        {
            // создание прямоугольников
            Rectangle r1 = new Rectangle(0, 0, 2, 3);
            Rectangle r2 = new Rectangle(1, 1, 4, 5);
            
            Console.WriteLine($"r1: {r1}");
            Console.WriteLine($"r2: {r2}");
            
            // перемещение
            r1.Move(1, 1);
            Console.WriteLine($"r1 после перемещения: {r1}");
            
            // изменение размеров (сохраняя левый верхний угол)
            Console.WriteLine("Изменяем размер r2 (ширина -1, высота +1)");
            r2.Resize(-1, 1);
            Console.WriteLine($"r2 после изменения размеров: {r2}");
            Console.WriteLine($"Левый верхний угол r2: ({r2.Left}, {r2.Top})");
            
            // ограничивающий прямоугольник
            Rectangle bounding = Rectangle.GetBoundingRectangle(r1, r2);
            Console.WriteLine($"ограничивающий: {bounding}");
            
            // пересечение
            Rectangle intersection = Rectangle.GetIntersection(r1, r2);
            Console.WriteLine($"пересечение: {intersection}");
        }
        catch (Exception e)
        {
            Console.WriteLine($"ошибка: {e.Message}");
        }
    }
}