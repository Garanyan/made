/*
 *
    B. Разрез
    ограничение по времени на тест 2 секунды
    ограничение по памяти на тест 1024 мегабайта
    ввод стандартный ввод
    вывод стандартный вывод

    Найдите минимальный разрез между вершинами 1 и n в заданном неориентированном графе.
    Входные данные
    На первой строке входного файла содержится n (2≤n≤100) — число вершин в графе
    и m (0≤m≤400) — количество ребер. На следующих m строках входного файла содержится описание ребер.
    Ребро описывается номерами вершин, которые оно соединяет, и его пропускной способностью
    (положительное целое число, не превосходящее 10000000), при этом никакие две вершины н
    е соединяются более чем одним ребром.

    Выходные данные
    На первой строке выходного файла должны содержаться количество ребер в минимальном разрезе
    и их суммарная пропускная способность. На следующей строке выведите возрастающую последовательность
    номеров ребер (ребра нумеруются в том порядке, в каком они были заданы во входном файле).
 *
 * */

#include <iostream>
#include <vector>
#include <climits>
#include <queue>

struct edge {
    int a, b, capacity, flow;
};

class Dinic {

public:
    Dinic(int kVertexNum, int kEndesNum);

    void AddEdge(int a, int b, int cap);

    void SetSource(int e);

    void SetSink(int e);

    int GetMaxFlow();

    std::vector<int> GetMinCut();

private:

    bool bfs();

    int dfs(int v, int flow, int person);

    void dfs(int u, std::vector<bool> &visited);

    std::vector<edge> edges;
    std::vector<std::vector<int>> graph;
    std::vector<int> d;
    std::vector<int> ptr;
    std::vector<int> q;
    int source, sink;
};

Dinic::Dinic(const int kVertexNum, const int kEndesNum) {
    edges.reserve(2 * kEndesNum);
    graph.resize(kVertexNum);
    ptr.reserve(kEndesNum);
    q.reserve(kEndesNum);
    d.reserve(kEndesNum);
    source = 0;
    sink = kVertexNum - 1;
}

void Dinic::AddEdge(const int a, const int b, const int cap) {
    edge e1 = {a, b, cap, 0};
    edge e2 = {b, a, cap, 0};
    graph[a].push_back(edges.size());
    edges.push_back(e1);
    graph[b].push_back(edges.size());
    edges.push_back(e2);
}

void Dinic::SetSource(const int e) {
    source = e;
}

void Dinic::SetSink(const int e) {
    sink = e;
}

int Dinic::dfs(const int v, int flow, int person) {
    if (v == sink || flow == 0) {
        return flow;
    }
    for (; ptr[v] < graph[v].size(); ++ptr[v]) {
        int id = graph[v][ptr[v]];
        int to = edges[id].b;
        if (d[to] == d[v] + 1) {
            int pushed = dfs(to, std::min(flow, edges[id].capacity - edges[id].flow), person);
            if (pushed) {
                edges[id].flow += pushed;
                edges[id ^ 1].flow -= pushed;

                return pushed;
            }
        }
    }
    return 0;
}

bool Dinic::bfs() {
    d = std::vector<int>(graph.size(), -1);
    d[source] = 0;
    std::queue<int> qu;
    qu.push(source);

    while (!qu.empty()) {
        auto u = qu.front();
        qu.pop();
        for (auto edge_id: graph[u]) {
            auto edge = edges[edge_id];
            if (edge.flow < edge.capacity && d[edge.b] == -1) {
                d[edge.b] = d[u] + 1;
                qu.push(edge.b);
            }
        }
    }

    return d[sink] != -1;//sink reachable
}

int Dinic::GetMaxFlow() {

    int flow = 0;
    int i = 0;
    while (!edges.empty() && bfs()) {

        ptr = std::vector<int>(graph.size(), 0);
        int subflow = dfs(source, INT_MAX, flow);
        while (subflow) {
            flow += subflow;
            subflow = dfs(source, INT_MAX, flow);
        }

        i++;
    }
    return flow;

}

void Dinic::dfs(int u, std::vector<bool> &visited) {
    visited[u] = true;

    for (auto edge_id : graph[u]) {
        auto edge = edges[edge_id];
        if (!visited[edge.b] && edge.flow < edge.capacity) {
            dfs(edge.b, visited);
        }
    }
}

std::vector<int> Dinic::GetMinCut() {
    std::vector<int> minCut;
    std::vector<bool> visited(graph.size(), false);
    dfs(source, visited);
    for (int i = 0; i < edges.size(); i += 2) {
        auto edge = edges[i];
        if ((visited[edge.a] && !visited[edge.b]) || (!visited[edge.a] && visited[edge.b])) {
            minCut.push_back(i / 2);
        }
    }

    return minCut;
}

int main() {

    int v, e;
    std::cin >> v >> e;
    Dinic dinic(v, e);
    int g, f, capacity;

    for (int i = 0; i < e; ++i) {
        std::cin >> g >> f >> capacity;
        dinic.AddEdge(g - 1, f - 1, capacity);
    }

    dinic.SetSource(0);
    dinic.SetSink(v - 1);

    auto flow = dinic.GetMaxFlow();

    auto cut = dinic.GetMinCut();

    std::cout << cut.size() << " " << flow << std::endl;
    for (auto i :cut) {
        std::cout << i + 1 << " ";
    }


    return 0;
}
