/*
 *
 *
B. Задача о назначениях
ограничение по времени на тест 5.0 с
ограничение по памяти на тест 256 мегабайт
ввод стандартный ввод
вывод стандартный вывод

 Дана целочисленная матрица C размера n×n. Требуется выбрать n ячеек так,
 чтобы в каждой строке и каждом столбце была выбрана ровно одна ячейка, а сумма значений в выбранных
 ячейках была минимальна.

Входные данные
Первая строка входного файла содержит n
(2≤n≤300). Каждая из последующих n строк содержит по n чисел: Cij Все значения во входном файле неотрицательны и не превосходят 10^6.

Выходные данные
В первую строку выходного файла выведите одно число — искомая минимизируемая величина. Далее выведите n
строк по два числа в каждой — номер строки и столбца клетки, участвующей в оптимальном назначении.
Пары чисел можно выводить в произвольном порядке.


Пример
Входные данные
3
3 2 1
1 3 2
2 1 3

Выходные данные
3
2 1
3 2
1 3

 *
 *
 */



#include <iostream>
#include <vector>
#include <zconf.h>
#include <deque>

struct edge {
    int a, b, capacity;
    long long flow, weight;
    int reversed;
};

class AssignmentProblem {
public:
    explicit AssignmentProblem(int n);

    void AddEdge(const int a, const int b, const int cap, int weight);

    long long Solve(bool pre_FB);

    void PrintAssignment();

    void setSink(int i);

private:

    void dijkstra(int v);

    void LevitAlgorithm();

    std::vector<std::vector<edge>> graph;
    std::vector<long long> d;
    std::vector<long long> potential;
    std::vector<edge *> p;

    int source = 0;
    int sink;
    int n;

};

void AssignmentProblem::AddEdge(const int a, const int b, const int cap, int weight) {
    edge e1 = {a, b, cap, 0, weight, static_cast<int>(graph[b].size())};
    edge e2 = {b, a, 0, 0, -weight, static_cast<int>(graph[a].size())};

    graph[a].push_back(e1);
    graph[b].push_back(e2);
}

void AssignmentProblem::dijkstra(int s) {
    d = std::vector<long long>(graph.size(), LONG_LONG_MAX);
    std::vector<bool> used(graph.size(), false);

    d[s] = 0;
    for (int i = 0; i < graph.size(); i++) {
        auto v = -1;
        for (int j = 0; j < graph.size(); j++) {
            if (!used[j] && (v == -1 || d[j] < d[v])) {
                v = j;
            }
        }

        if (d[v] == LONG_LONG_MAX) {
            break;
        }

        used[v] = true;

        for (int id = 0; id < graph[v].size(); id++) {
            auto &e = graph[v][id];
            long long len = e.weight + potential[e.a] - potential[e.b];//вес с учетом потенциала

            // если еще можно пустить поток с учетом новых весов
            if (e.flow < e.capacity && d[v] + len < d[e.b]) {
                d[e.b] = d[v] + len;
                p[e.b] = &e;
            }
        }
    }
}

AssignmentProblem::AssignmentProblem(int N) {
    n = N;
    graph.resize(2 * n + 2);
    p.resize(2 * n + 2);
    potential.resize(2 * n + 2, 0);
    sink = 2 * n + 1;
}

void AssignmentProblem::LevitAlgorithm() {
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
                potential[e.b] = d[e.b];

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

long long AssignmentProblem::Solve(bool pre_FB) {

    // Если есть отрицательные веса
    if (!pre_FB) {
        LevitAlgorithm();
    }

    long long min_cost = 0;
    for (int k = 0; k < n; ++k) {

        // пересчитываем d[]
        dijkstra(0);

        // сток недостижим
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
            // увеличиваем поток вдоль пути
            auto e1 = p[u];
            auto e2 = &graph[e1->b][e1->reversed];

            e1->flow += adjust_flow;
            e2->flow -= adjust_flow;

            min_cost += adjust_flow * e1->weight;
        }

        // Обновляем потенциалы
        for (int i = 0; i < graph.size(); i++) {
            potential[i] = potential[i] + d[i];
        }
    }

    return min_cost;
}

void AssignmentProblem::setSink(int i) {
    sink = i;
}

void AssignmentProblem::PrintAssignment() {
    for (int i = 1; i <= n; ++i) {
        for (auto e : graph[i]) {
            if (e.flow == 1) {
                std::cout << e.a << " " << e.b - n << std::endl;
            }
        }
    }
}

int main() {

    int n = 0;
    std::cin >> n;

    AssignmentProblem assignmentProblem(n);
    int w;
    bool all_positive = true;
    for (int i = 1; i <= n; i++) {
        assignmentProblem.AddEdge(0, i, 1, 0);
        assignmentProblem.AddEdge(i + n, 2 * n + 1, 1, 0);
        for (int j = 1; j <= n; j++) {
            std::cin >> w;
            if (w < 0) {
                all_positive = false;
            }
            assignmentProblem.AddEdge(i, j + n, INT_MAX, w);
        }
    }

    assignmentProblem.setSink(2 * n + 1);

    std::cout << assignmentProblem.Solve(all_positive) << std::endl;
    assignmentProblem.PrintAssignment();

    return 0;
}
