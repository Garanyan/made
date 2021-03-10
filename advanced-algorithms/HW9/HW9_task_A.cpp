/*
 *

A. Максимальный поток минимальной стоимости

ограничение по времени на тест 2 секунды
ограничение по памяти на тест 256 мегабайт
ввод стандартный ввод
вывод стандартный вывод

Задан ориентированный граф, каждое ребро которого обладает пропускной способностью и стоимостью.
Найдите максимальный поток минимальной стоимости из вершины с номером 1 в вершину с номером n.

Входные данные

Первая строка входного файла содержит n и m — количество вершин и количество ребер графа (2≤n≤100, 0≤m≤1000).
Следующие m строк содержат по четыре целых числа числа: номера вершин, которые соединяет соответствующее ребро графа,
его пропускную способность и его стоимость. Пропускные способности и стоимости не превосходят 10^5.

Выходные данные
В выходной файл выведите одно число — цену максимального потока минимальной стоимости из вершины с номером 1
в вершину с номером n. Ответ не превышает 2^63−1. Гарантируется, что в графе нет циклов отрицательной стоимости.

 *
 */

#include <vector>
#include <iostream>
#include <queue>
#include <zconf.h>


struct edge {
    int a, b, capacity;
    long long flow, weight;
    int reversed;
};

class BellmanFord {
public:

    BellmanFord(int v, int e);

    void AddEdge(const int a, const int b, const int cap, int weight);

    void SetSink(int v);

    void SetSource(int v);

    long long int MinCostForMaxFlow();

private:

    void LevitAlgorithm();

    std::vector<std::vector<edge>> graph;
    std::vector<long long> d; // d[i] текущая длина кратчайшего пути от source  до вершины i
    std::vector<edge *> p;

    int source = 0;
    int sink;
};

void BellmanFord::AddEdge(const int a, const int b, const int cap, int weight) {
    edge e1 = {a, b, cap, 0, weight, static_cast<int>(graph[b].size())};
    edge e2 = {b, a, 0, 0, -weight, static_cast<int>(graph[a].size())};

    graph[a].push_back(e1);
    graph[b].push_back(e2);
}

long long BellmanFord::MinCostForMaxFlow() {
    long long max_flow = 0, min_cost = 0;

    while (true) {
        LevitAlgorithm();


        //сток недостижим
        if (d[sink] == LONG_LONG_MAX) {
            break;
        }

        long long adjust_flow = LONG_LONG_MAX;
        for (int u = sink; u != source; u = p[u]->a) {
            // идем по пути от стока к источнику, считаем на сколько можем увеличить поток
            auto e1 = p[u];

            adjust_flow = std::min(adjust_flow, static_cast<long long>(e1->capacity - e1->flow));
        }

        for (int u = sink; u != source; u = p[u]->a) {
            auto e1 = p[u];
            auto e2 = &graph[e1->b][e1->reversed];

            e1->flow += adjust_flow;
            e2->flow -= adjust_flow;

            min_cost += adjust_flow * e1->weight;
        }

        max_flow += adjust_flow;
    }

    return min_cost;
}

BellmanFord::BellmanFord(int v, int e) {
    graph.resize(v + 1);
    p.resize(v + 1);
}

void BellmanFord::SetSink(int v) {
    sink = v;
}

void BellmanFord::SetSource(int v) {
    source = v;
}

void BellmanFord::LevitAlgorithm() {
    std::deque<int> q;
    d = std::vector<long long>(graph.size(), LONG_LONG_MAX);
    std::vector<int> state(graph.size(), 2); // хранит номер множества, к которому относится вершина
    d[source] = 0;
    state[source] = 0;
    q.push_back(source);

    while (!q.empty()) {
        int u = q.front();
        q.pop_front();
        state[u] = 0;

        for (int i = 0; i < graph[u].size(); ++i) {
            auto &e = graph[u][i];
            if (e.flow < e.capacity && d[u] + e.weight < d[e.b]) {
                d[e.b] = d[u] + e.weight;
                if (state[e.b] == 2) {
                    q.push_back(e.b);
                } else if (state[e.b] == 0) {
                    q.push_front((e.b));
                }

                p[e.b] = &e; // ребро в кратчайшем пути от вершины
                state[e.b] = 1;
            }
        }
    }
}

int main() {
    int v = 0, e = 0;
    std::cin >> v >> e;
    int a = 0, b = 0, f = 0, w = 0;
    BellmanFord solver(v, e);
    for (int i = 0; i < e; ++i) {
        std::cin >> a >> b >> f >> w;
        solver.AddEdge(a - 1, b - 1, f, w);
    }

    solver.SetSink(v - 1);
    solver.SetSource(0);

    std::cout << solver.MinCostForMaxFlow() << std::endl;
    return 0;
}
