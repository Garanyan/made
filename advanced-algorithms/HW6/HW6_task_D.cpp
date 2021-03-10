//
// Created by Ovanes on 4/6/20.
//

// LCA
//        ограничение по времени на тест 5 секунд
//        ограничение по памяти на тест 256 мегабайт
//        ввод стандартный ввод
//        вывод стандартный вывод
//
//        Дано подвешенное дерево с корнем в первой вершине. Вам нужно ответить на m
//        запросов вида "найти LCA двух вершин". LCA вершин u и v в подвешенном дереве — это наиболее
//        удалённая от корня дерева вершина, лежащая на обоих путях от u и v до корня.
// Входные данные
// В первой строке задано целое число n — число вершин в дереве (1≤n≤2⋅10^5).
//
// В следующих n−1 строках записано одно целое число x.
// Число x на строке i означает, что x — предок вершины i(x<i).
// Затем дано число m.
//
// Далее заданы m
// (0 ≤ m ≤ 5⋅10^5) запросов вида (u,v) — найти LCA двух вершин u и v (1 ≤ u,v ≤ n; u ≠ v).
//
// Выходные данные
// Для каждого запроса выведите LCA двух вершин на отдельной строке.


// Примеры
// Входные данные
// 5
// 1
// 1
// 2
// 3
// 2
// 2 3
// 4 5
//
// Выходные данные
// 1
// 1

#include <iostream>
#include <vector>
#include <cmath>


void dfs(int u, int depth, std::vector<std::vector<int>> &graph, std::vector<int> &depths, std::vector<bool> &visited) {
    visited[u] = true;
    depths[u] = ++depth;
    for (auto adj : graph[u]) {
        if (!visited[adj]) {
            dfs(adj, depth, graph, depths, visited);
        }
    }
}

void dfs(std::vector<std::vector<int>> &graph, std::vector<int> &depth) {
    std::vector<bool> visited(graph.size(), false);
    for (size_t i = 0; i < graph.size(); ++i) {
        if (!visited[i]) {
            dfs(i, 0, graph, depth, visited);
        }
    }
}

int lca(const std::vector<int> &depths, const std::vector<std::vector<int>> &jmp, int u, int v) {
    if (depths[u] < depths[v]) {
        std::swap(u, v);
    }
    auto delta = depths[u] - depths[v];
    for (int i = int(std::log2(depths.size())) + 1; i >= 0; i--) {
        if (delta >= (1 << i)) {
            u = jmp[u][i];
            delta -= (1 << i);
        }
    }

    if (u == v) {
        return v;
    }

    for (int i = int(std::log2(depths.size())) + 1; i >= 0; i--) {
        int g = jmp[u][i];
        int f = jmp[v][i];
        if (g != f) {
            u = g;
            v = f;
        }
    }

    return jmp[u][0];
}

int main() {
    int n = 0;
    std::cin >> n;

    std::vector<std::vector<int>> graph(n);
    std::vector<int> parents(n);
    for (size_t i = 2; i <= n; ++i) {
        int v;
        std::cin >> v;
        graph[v - 1].push_back(i - 1);
        parents[i - 1] = v - 1;
    }
    std::vector<int> depths(n, 0);
    dfs(graph, depths);
    std::vector<std::vector<int>> jmp(n);
    for (int v = 0; v < n; ++v) {
        for (int i = 0; i < int(std::log2(n)) + 2; ++i) {
            if (i == 0) {
                jmp[v].push_back(parents[v]);
            } else {
                jmp[v].push_back(jmp[jmp[v][i - 1]][i - 1]);
            }
        }
    }
    int m = 0;
    std::cin >> m;
    for (size_t i = 0; i < m; ++i) {
        int u, v;
        std::cin >> u >> v;
        std::cout << lca(depths, jmp, u - 1, v - 1) + 1 << std::endl;
    }

    return 0;
}
