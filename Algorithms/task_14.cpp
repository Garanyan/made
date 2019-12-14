//
// Created by Ovanes Garanyan (Ovanes-X) on 14.12.2019.
//

//Дан неориентированный связный граф. Требуется найти вес минимального остовного дерева в этом графе.
//Вариант 2. С помощью алгоритма Крускала.
//Первая строка содержит два натуральных числа n и m — количество вершин
// и ребер графа соответственно (1 ≤ n ≤ 20000, 0 ≤ m ≤ 100000).
//Следующие m строк содержат описание ребер по одному на строке.
//Ребро номер i описывается тремя натуральными числами bi, ei и wi — номера концов ребра и его вес соответственно
// (1 ≤ bi, ei ≤ n, 0 ≤ wi ≤ 100000).
// Выведите единственное целое число - вес минимального остовного дерева.

#include <vector>
#include <iostream>
#include <map>
#include <cassert>

class Graph{
public:
    Graph (size_t v, size_t e);

    void AddEdge(size_t from, size_t to, size_t weight);

    size_t Kruskal();

    ~Graph() = default;

private:
    // weight -> (vertex, vertex)
    std::map<size_t ,std::vector<std::pair<size_t ,size_t>>> vertex_chain;
    size_t vertices_num = 0;
};

Graph::Graph(size_t v, size_t e){
    vertices_num = v;
}

void Graph::AddEdge(size_t from, size_t to, size_t weight){
    vertex_chain[weight].emplace_back(from, to);
}

class DSU {
public:
    explicit DSU(size_t size);

    int find(int element);
    void merge(int left, int right);

private:
    std::vector<int> parent;
    std::vector<int> rank;
};

DSU::DSU(size_t size) :
        parent(size, -1),
        rank(size, 1) {}

int DSU::find(int element) {
    if (parent[element] == -1) {
        return element;
    }
    return parent[element] = find(parent[element]);
}

void DSU::merge(int left, int right) {
    const int deputat_left = find(left);
    const int deputat_right = find(right);
    assert(deputat_left != deputat_right);
    if (rank[deputat_left] == rank[deputat_right]) {
        parent[deputat_right] = deputat_left;
        ++rank[deputat_left];
    } else if(rank[deputat_left] > rank[deputat_right]) {
        parent[deputat_right] = deputat_left;
    } else {
        parent[deputat_left] = deputat_right;
    }
}

size_t Graph::Kruskal(){
    size_t tree_weight = 0;
    DSU ds(vertices_num);
    for(const auto& weight_to_edges: vertex_chain)
    {
        for(const auto& edge: weight_to_edges.second)
        {
            int deputat_u = ds.find(edge.first);
            int deputat_v = ds.find(edge.second);

            if (deputat_u != deputat_v)
            {
                tree_weight += weight_to_edges.first;
                ds.merge(deputat_u, deputat_v);
            }
        }
    }
    return tree_weight;
}

//4 4
//1 2 1
//2 3 2
//3 4 5
//4 1 4
int main(int argc, char **argv){
    size_t n, m;
    std::cin >> n >> m;
    Graph graph(n,m);
    for(size_t i = 0; i < m; ++i){
        size_t u, v, w;
        std::cin >> u >> v >> w;
        graph.AddEdge(u-1, v-1, w);
    }

    std::cout<<graph.Kruskal()<<std::endl;

    return 0;
}
