//
// Created by ovanes on 08.12.2019.
//
//11_2. Количество различных путей
//        Дан невзвешенный неориентированный граф. В графе может быть несколько кратчайших путей между какими-то вершинами.
//        Найдите количество различных кратчайших путей между заданными вершинами. Требуемая сложность O(V+E).
//Ввод: v: кол-во вершин (макс. 50000), n: кол-во ребер (макс. 200000), n пар реберных вершин, пара вершин v, w для запроса.
//Вывод: количество кратчайших путей от v к w.
#include <iostream>
#include <vector>
#include <climits>
#include <queue>


class Graph
{
public:
    void AddEdge(size_t from, size_t to);

    size_t NumberOfShortestPaths(size_t s, size_t dest);

private:
    std::vector<std::vector<size_t>> vertex_chain;
    size_t vertices_num = 0;
    size_t edges_num = 0;
};

void Graph::AddEdge(size_t from, size_t to)
{

    if (from >= vertices_num)
    {
        vertices_num = from + 1;
        vertex_chain.resize(vertices_num);
    }

    if (to >= vertices_num)
    {
        vertices_num = to + 1;
        vertex_chain.resize(vertices_num);
    }

    vertex_chain[from].push_back(to);
    vertex_chain[to].push_back(from);
    ++edges_num;
}

size_t Graph::NumberOfShortestPaths(size_t s, size_t dest)
{
    if (dest >= vertices_num || s >= vertices_num)
        return 0;

    std::vector<size_t> dist(vertices_num, INT_MAX), paths(vertices_num, 0);
    std::vector<bool> visited(vertices_num, false);
    dist[s] = 0;
    paths[s] = 1;

    std::queue<size_t> queue;
    queue.push(s);
    visited[s] = true;

    while (!queue.empty())
    {
        size_t current = queue.front();
        queue.pop();

        for (auto neighbour : vertex_chain[current])
        {

            if (!visited[neighbour])
            {
                queue.push(neighbour);
                visited[neighbour] = true;
            }

            if (dist[neighbour] > dist[current] + 1)
            {
                dist[neighbour] = dist[current] + 1;
                paths[neighbour] = paths[current];
            } else if (dist[neighbour] == dist[current] + 1)
            {
                paths[neighbour] += paths[current];
            }

        }

    }
    return paths[dest];
}

//4
//5
//0 1
//0 2
//1 2
//1 3
//2 3
//
//0 3

//2
int main()
{
    size_t v, n;
    std::cin >> v >> n;
    Graph graph;
    size_t a, b;
    for (size_t i = 0; i < n; ++i)
    {
        std::cin >> a >> b;
        graph.AddEdge(a, b);
    }
    std::cin >> a >> b;
    std::cout << graph.NumberOfShortestPaths(a, b) << std::endl;
    return 0;
}
