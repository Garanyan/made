//
// Created by ovanes on 10.12.2019.
//

#include <fstream>
#include <vector>
#include <algorithm>
#include <iostream>
#include <map>

class Graph
{
public:
    void AddEdge(size_t from, size_t to, size_t id);

    bool BridgesCount(std::string output_file);

private:
    void dfs(size_t i, std::vector<bool>& visited, std::vector<int>& disc, std::vector<int>& low,
             std::vector<std::pair<size_t, size_t>>& bridges, int p = -1);

    //Вершина на список смежных вершин
    std::vector<std::vector<size_t>> vertex_to_neighbour;

    //ребро на список его индексов во входном файле
    std::map<std::pair<size_t, size_t>, std::vector<size_t>> edges;
    size_t vertices_num = 0;
};

void Graph::AddEdge(size_t from, size_t to, size_t id)
{

    if (from >= vertices_num)
    {
        vertices_num = from + 1;
        vertex_to_neighbour.resize(vertices_num);
    }

    if (to >= vertices_num)
    {
        vertices_num = to + 1;
        vertex_to_neighbour.resize(vertices_num);
    }

    vertex_to_neighbour[from].push_back(to);
    vertex_to_neighbour[to].push_back(from);
    auto edge = std::make_pair(from, to);
    edges[edge].emplace_back(id);
}

bool Graph::BridgesCount(std::string output_file)
{

    std::vector<bool> visited(vertices_num, false);
    std::vector<int> disc(vertices_num, -1);
    std::vector<int> low(vertices_num, -1);

    std::vector<std::pair<size_t, size_t>> bridges;
    for (size_t i = 0; i < vertices_num; i++)
        if (!visited[i])
            dfs(i, visited, disc, low, bridges);

    std::ofstream output;
    output.open(output_file, std::ios::out);

    std::vector<size_t> result;
    result.reserve(bridges.size());
    for (const auto& bridge: bridges)
    {
        auto it = edges.find(bridge);
        if (it != edges.end() && it->second.size() == 1)
            result.insert(result.end(), edges[bridge].begin(), edges[bridge].end());
        else if (edges[std::make_pair(bridge.second, bridge.first)].size() == 1)
            result.insert(result.end(), edges[std::make_pair(bridge.second, bridge.first)].begin(),
                edges[std::make_pair(bridge.second, bridge.first)].end());
    }
    output << result.size() << std::endl;
    std::sort(result.begin(), result.end());
    for (const auto r : result)
    {
        output << r << std::endl;
    }
    output.close();
    return true;

}

void Graph::dfs(size_t id, std::vector<bool>& visited, std::vector<int>& disc,
                std::vector<int>& low, std::vector<std::pair<size_t, size_t>>& bridges, int p)
{
    static int time = 0;

    visited[id] = true;
    disc[id] = low[id] = time++;

    for (const auto neighbour: vertex_to_neighbour[id])
    {
        if (!visited[neighbour])
        {
            dfs(neighbour, visited, disc, low, bridges, id);
            if (low[id] > low[neighbour])
                low[id] = low[neighbour];
            if (low[neighbour] == disc[neighbour])
                bridges.emplace_back(id, neighbour);

        }
        else if (neighbour != p)
        {
            if (low[id] > disc[neighbour])
                low[id] = disc[neighbour];

        }
    }
}

int main()
{
    Graph graph;
    std::ifstream file;
    file.open("bridges.in", std::ios::in);
    if (file.is_open())
    {
        size_t v, e;
        file >> v >> e;
        for (size_t i = 0; i < e; ++i)
        {
            size_t from, to;
            file >> from >> to;
            graph.AddEdge(from - 1, to - 1, i + 1);
        }
    }
    file.close();
    graph.BridgesCount("bridges.out");
    return 0;
}