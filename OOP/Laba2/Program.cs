using System;

public class RectangleClass
{
    public int Xpos { get; private set; }
    public int Ypos { get; private set; }
    public int Wid { get; private set; }
    public int Hei { get; private set; }

    public RectangleClass(int x, int y, int w, int h)
    {
        if (w <= 0 || h <= 0)
            throw new ArgumentException("Размер должен быть положительным");
        
        Xpos = x;
        Ypos = y;
        Wid = w;
        Hei = h;
    }

    ~RectangleClass()
    {
        Console.WriteLine($"Деструктор: Прямоугольник {ToString()} уничтожен");
    }

    public void MoveRect(int dx, int dy)
    {
        Xpos += dx;
        Ypos += dy;
    }

    public void ChangeSize(int newW, int newH)
    {
        if (newW <= 0 || newH <= 0)
            throw new ArgumentException("Неправильный размер для изменения!");
        
        Wid = newW;
        Hei = newH;
    }

    public void ChangeSize(int delta)
    {
        int newW = Wid + delta;
        int newH = Hei + delta;
        
        if (newW <= 0 || newH <= 0)
            throw new ArgumentException("Неправильный дельта-размер!");
        
        Wid = newW;
        Hei = newH;
    }

    public static RectangleClass GetBigRectangle(RectangleClass r1, RectangleClass r2)
    {
        int left = Math.Min(r1.Xpos, r2.Xpos);
        int top = Math.Min(r1.Ypos, r2.Ypos);
        int right = Math.Max(r1.Xpos + r1.Wid, r2.Xpos + r2.Wid);
        int bottom = Math.Max(r1.Ypos + r1.Hei, r2.Ypos + r2.Hei);
        
        return new RectangleClass(left, top, right - left, bottom - top);
    }

    public static RectangleClass FindOverlap(RectangleClass a, RectangleClass b)
    {
        int startX = Math.Max(a.Xpos, b.Xpos);
        int endX = Math.Min(a.Xpos + a.Wid, b.Xpos + b.Wid);
        int startY = Math.Max(a.Ypos, b.Ypos);
        int endY = Math.Min(a.Ypos + a.Hei, b.Ypos + b.Hei);

        if (endX > startX && endY > startY)
            return new RectangleClass(startX, startY, endX - startX, endY - startY);
        
        return null;
    }

    public override string ToString()
    {
        return $"[X: {Xpos}, Y: {Ypos}, W: {Wid}, H: {Hei}]";
    }
}

class Program
{
    static void Main()
    {
        // Оригинальный сценарий
        RectangleClass rect1 = new RectangleClass(2, 3, 5, 4);
        RectangleClass rect2 = new RectangleClass(4, 5, 6, 3);
        
        Console.WriteLine("Исходные прямоугольники:");
        Console.WriteLine("Прямоугольник 1: " + rect1);
        Console.WriteLine("Прямоугольник 2: " + rect2);
        Console.WriteLine();

        rect1.MoveRect(2, -1);
        Console.WriteLine("После перемещения первого:");
        Console.WriteLine(rect1);
        Console.WriteLine();

        rect2.ChangeSize(4, 6);
        Console.WriteLine("После изменения размера второго:");
        Console.WriteLine(rect2);
        Console.WriteLine();

        RectangleClass big = RectangleClass.GetBigRectangle(rect1, rect2);
        Console.WriteLine("Наименьший ограничивающий прямоугольник:");
        Console.WriteLine(big);
        Console.WriteLine();

        RectangleClass overlap = RectangleClass.FindOverlap(rect1, rect2);
        Console.WriteLine("Результат пересечения:");
        Console.WriteLine(overlap == null ? "Нет пересечения" : overlap.ToString());
        Console.WriteLine();

        rect1.MoveRect(-3, 2);
        rect1.ChangeSize(7, 5);
        Console.WriteLine("После дополнительных изменений первого:");
        Console.WriteLine(rect1);
        Console.WriteLine();

        RectangleClass big2 = RectangleClass.GetBigRectangle(rect1, rect2);
        Console.WriteLine("Новый ограничивающий прямоугольник:");
        Console.WriteLine(big2);
        Console.WriteLine();

        RectangleClass overlap2 = RectangleClass.FindOverlap(rect1, rect2);
        Console.WriteLine("Проверка пересечения снова:");
        Console.WriteLine(overlap2 == null ? "Нет пересечения" : overlap2.ToString());

        GC.Collect();
        GC.WaitForPendingFinalizers();
    }
}