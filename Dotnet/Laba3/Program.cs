using System;

class Program
{
    static void Main()
    {
        double dx = 0.1;
        double e = 0.001;
        
        Console.WriteLine("  x      |  arctg(x) (series) |  arctg(x) (Math.Atan) |  terms");
        Console.WriteLine("---------------------------------------------------------------");
        for (double x = -1; x <= 1; x += dx)
        {
            double sum = 0;
            double term = x;
            int n = 0;
            int count = 0;
            
            while (Math.Abs(term) >= e)
            {
                sum += term;
                n++;
                term = -term * x * x * (2 * n - 1) / (2 * n + 1);
                count++;
            }
            
            double arctg_series = sum;
            double arctg_builtin = Math.Atan(x);
            
            Console.WriteLine($"{x,7:F2} | {arctg_series,17:F10} | {arctg_builtin,17:F10} | {count,6}");
        }
    }
}
