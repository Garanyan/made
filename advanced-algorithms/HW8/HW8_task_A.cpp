//
// A. Просто поток
//        ограничение по времени на тест 5 секунд
//        ограничение по памяти на тест 1024 мегабайта
//        ввод стандартный ввод
//        вывод стандартный вывод
//
//        Дана система из узлов и труб, по которым может течь вода.
//        Для каждой трубы известна наибольшая скорость, с которой вода может протекать через нее.
//        Известно, что вода течет по трубам таким образом, что за единицу времени в каждый узел
//        (за исключением двух — источника и стока) втекает ровно столько воды, сколько из него вытекает.
//
//        Ваша задача — найти наибольшее количество воды, которое за единицу времени может протекать
//        между источником и стоком, а также скорость течения воды по каждой из труб.
//
//        Трубы являются двусторонними, то есть вода в них может течь в любом направлении.
//        Между любой парой узлов может быть более одной трубы.
//
// Входные данные
// В первой строке записано натуральное число N — количество узлов в системе (2≤N≤100).
// Известно, что источник имеет номер 1, а сток номер N. Во второй строке записано
// натуральное M (1≤M≤5000) — количество труб в системе. Далее в M строках идет описание труб.
// Каждая труба задается тройкой целых чисел
// Ai, Bi, Ci, где Ai, Bi — номера узлов, которые соединяет данная труба (Ai≠Bi),
// а Ci (0 ≤ Ci ≤ 10^4) — наибольшая допустимая скорость течения воды через данную трубу.
//
// Выходные данные
// В первой строке выведите наибольшее количество воды, которое протекает между источником и
// стоком за единицу времени. Далее выведите M строк, в каждой из которых выведите скорость
// течения воды по соответствующей трубе. Если направление не совпадает с порядком узлов,
// заданным во входных данных, то выводите скорость со знаком минус. Числа выводите с точностью 10^{−3}.


#include <iostream>
#include <vector>
#include <cstring>
#include <climits>

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

    void PrintEdgesFlow();

private:

    bool bfs();

    int dfs(int v, int flow);

    std::vector<edge> edges;
    std::vector<std::vector<int>> graph;
    std::vector<int> d;
    std::vector<int> ptr;
    std::vector<int> q;
    int source, sink;
};

Dinic::Dinic(const int kVertexNum, const int kEndesNum) {
    edges.reserve(kEndesNum);
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


int Dinic::dfs(const int v, int flow) {
    if (!flow) {
        return 0;
    }
    if (v == sink) {
        return flow;
    }
    for (; ptr[v] < graph[v].size(); ++ptr[v]) {
        int id = graph[v][ptr[v]];
        int to = edges[id].b;
        if (d[to] != d[v] + 1) {
            continue;
        }
        int pushed = dfs(to, std::min(flow, edges[id].capacity - edges[id].flow));
        if (pushed) {
            edges[id].flow += pushed;
            if (id % 2 == 0)
                edges[id + 1].flow -= pushed;
            else
                edges[id - 1].flow -= pushed;

            return pushed;
        }
    }
    return 0;
}

bool Dinic::bfs() {
    int qh = 0, qt = 0;
    q[qt++] = source;
    d = std::vector<int>(graph.size(), -1);
    d[source] = 0;

    while (qh < qt && d[sink] == -1) {
        int v = q[qh++];
        for (size_t i = 0; i < graph[v].size(); ++i) {
            int edge_id = graph[v][i];
            int to = edges[edge_id].b;
            if (d[to] == -1 && edges[edge_id].flow < edges[edge_id].capacity) {
                q[qt++] = to;
                d[to] = d[v] + 1;
            }
        }
    }
    return d[sink] != -1;//sink reachable
}

int Dinic::GetMaxFlow() {

    int flow = 0;
    while (bfs()) {
        ptr = std::vector<int>(graph.size(), 0);
        while (int subflow = dfs(source, INT_MAX)) {
            flow += subflow;
        }
    }
    return flow;

}

void Dinic::PrintEdgesFlow() {
    for (int i = 0; i < edges.size(); i += 2) {
        if (edges[i].flow == 0) {
            std::cout << -edges[i + 1].flow << std::endl;
        } else {
            std::cout << edges[i].flow << std::endl;
        }
    }
}

//2
//2
//1 2 1
//2 1 3

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

    std::cout << dinic.GetMaxFlow() << std::endl;
    dinic.PrintEdgesFlow();
    return 0;
}
