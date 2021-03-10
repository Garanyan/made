
// C. Улиточки
//        ограничение по времени на тест 2 секунды
//        ограничение по памяти на тест 1024 мегабайта
//        ввод стандартный ввод
//        вывод стандартный вывод
//
//        Две улиточки Маша и Петя сейчас находятся в на лужайке с абрикосами и хотят добраться до своего домика.
//        Лужайки пронумерованы числами от 1 до n и соединены дорожками (может быть несколько дорожек соединяющих
//        две лужайки, могут быть дорожки, соединяющие лужайку с собой же). В виду соображений гигиены, если по
//        дорожке проползла улиточка, то вторая по той же дорожке уже ползти не может.
//        Помогите Пете и Маше добраться до домика.
//
// Входные данные
// В первой строке файла записаны четыре целых числа — n, m, s и t (количество лужаек,
// количество дорог, номер лужайки с абрикосами и номер домика).
// В следующих m строках записаны пары чисел. Пара чисел (x, y) означает,
// что есть дорожка с лужайки x до лужайки y (из-за особенностей улиток и
// местности дорожки односторонние). Ограничения: 2 <= n <= 10^5, 0<=m <=10^5, s != t.
//
// Выходные данные
// Если существует решение, то выведите YES и на двух отдельных строчках сначала последовательность
// лужаек для Машеньки (дам нужно пропускать вперед), затем путь для Пети.
// Если решения не существует, выведите NO. Если решений несколько, выведите любое.

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

    void PrintPaths();

private:

    bool bfs();

    int dfs(int v, int flow, int person);

    bool
    dfs_edge(int u, std::vector<bool> &visited, std::vector<bool> &visited_edge, std::vector<int> &vertex_path);

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
    edge e2 = {b, a, 0, 0};
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
        if (flow == 2)
            break;

        i++;
    }
    return flow;

}

bool Dinic::dfs_edge(int u, std::vector<bool> &visited, std::vector<bool> &visited_edge,
                     std::vector<int> &vertex_path) {
    visited[u] = true;
    if (u == sink) {
        return true;
    }

    for (auto edge_id : graph[u]) {
        auto edge = edges[edge_id];
        if (edge_id < edges.size() && !visited_edge[edge_id] && edge.flow == 1) {
            visited_edge[edge_id] = true;
            //std::cout<<edge.a+1<<" "<< edge.b+1<<"\t";
            if (dfs_edge(edge.b, visited, visited_edge, vertex_path)) {
                vertex_path.push_back(edge.b);
                return true;
            }

        }
    }

    return false;
}

void Dinic::PrintPaths() {
    std::vector<bool> visited_edge(edges.size(), false);
    std::vector<bool> visited(graph.size(), false);
    std::vector<int> vertex_path;
    dfs_edge(source, visited, visited_edge, vertex_path);
    std::cout << source + 1 << " ";
    for (auto i = vertex_path.rbegin(); i < vertex_path.rend(); i++) {
        std::cout << *i + 1 << " ";
    }

    std::cout << std::endl;
    vertex_path.clear();
    visited = std::vector<bool>(graph.size(), false);
    dfs_edge(source, visited, visited_edge, vertex_path);
    std::cout << source + 1 << " ";
    for (auto i = vertex_path.rbegin(); i < vertex_path.rend(); i++) {
        std::cout << *i + 1 << " ";
    }

}

int main() {

    int n, m, s, t;
    std::cin >> n >> m >> s >> t;
    Dinic dinic(n, m);
    int g, f, capacity = 1;

    for (int i = 0; i < m; ++i) {
        std::cin >> g >> f;
        if (g != f)
            dinic.AddEdge(g - 1, f - 1, capacity);
    }

    dinic.SetSource(s - 1);
    dinic.SetSink(t - 1);

    auto flow = dinic.GetMaxFlow();
    if (flow >= 2) {
        std::cout << "YES" << std::endl;
        dinic.PrintPaths();
    } else {
        std::cout << "NO" << std::endl;
    }
    return 0;
}

//8 11 4 3
//1 2
//2 3
//1 4
//4 5
//5 3
//1 6
//6 7
//7 3
//6 4
//7 5
//8 4