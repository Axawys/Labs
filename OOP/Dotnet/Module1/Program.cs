using System;
using System.Collections.Generic;

public class DijkstraAlgorithm
{
    // ребро графа
    public class Edge
    {
        public int Destination { get; }
        public int Weight { get; }
        
        public Edge(int dest, int weight) 
            => (Destination, Weight) = (dest, weight);
    }

    // информация о вершине
    public class VertexData
    {
        public int Distance { get; set; }
        public int Previous { get; set; }
        public bool Visited { get; set; }
        
        public VertexData() 
            => (Distance, Previous, Visited) 
                = (int.MaxValue, -1, false);
    }

    // основной алгоритм
    public static void FindShortestPaths(List<List<Edge>> graph, int start)
    {
        var vertices = new VertexData[graph.Count];
        for (int i = 0; i < graph.Count; i++)
            vertices[i] = new VertexData();
        
        vertices[start].Distance = 0;

        for (int i = 0; i < graph.Count - 1; i++)
        {
            int current = -1;
            int minDist = int.MaxValue;
            
            // поиск непосещенной вершины с минимальным расстоянием
            for (int j = 0; j < graph.Count; j++)
            {
                if (!vertices[j].Visited && vertices[j].Distance < minDist)
                {
                    minDist = vertices[j].Distance;
                    current = j;
                }
            }
            
            if (current == -1) break;

            vertices[current].Visited = true;

            // обновление расстояний до соседей
            foreach (var edge in graph[current])
            {
                int newDist = vertices[current].Distance + edge.Weight;
                if (newDist < vertices[edge.Destination].Distance)
                {
                    vertices[edge.Destination].Distance = newDist;
                    vertices[edge.Destination].Previous = current;
                }
            }
        }

        // вывод результатов
        PrintResults(vertices, start);
    }

    private static void PrintResults(VertexData[] vertices, int start)
    {
        Console.WriteLine($"Start vertex: {start}\n");
        Console.WriteLine("Vertex\tDistance\tPath");
        
        for (int i = 0; i < vertices.Length; i++)
        {
            Console.Write($"{i}\t{(vertices[i].Distance == int.MaxValue ? "INF" : vertices[i].Distance)}\t\t");
            
            if (vertices[i].Distance == int.MaxValue)
            {
                Console.WriteLine("no path");
                continue;
            }
            
            var path = new Stack<int>();
            for (int v = i; v != -1; v = vertices[v].Previous)
                path.Push(v);
            
            Console.WriteLine(string.Join(" → ", path));
        }
    }

    public static void Main()
    {
        // пример графа
        var graph = new List<List<Edge>> {
            new List<Edge> { new Edge(1, 4), new Edge(2, 1) },    // 0
            new List<Edge> { new Edge(3, 1) },                     // 1
            new List<Edge> { new Edge(1, 2), new Edge(3, 5) },    // 2
            new List<Edge>()                                       // 3
        };

        FindShortestPaths(graph, 0);
    }
}