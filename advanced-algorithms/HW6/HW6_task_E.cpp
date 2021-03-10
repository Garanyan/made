//
// Created by Ovanes on 4/7/20.
//


#include <iostream>
#include <vector>
#include <cmath>
#include <climits>
#include <fstream>


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

int lca_cost(const std::vector<int> &depths, const std::vector<std::vector<int>> &jmp,
        std::vector<std::vector<int>>& sum, int u, int v) {
    if (depths[u] < depths[v]) {
        std::swap(u, v);
    }
    int cost = INT_MAX;
    auto delta = depths[u] - depths[v];
    for (int i = int(std::log2(depths.size())) + 1; i >= 0; i--) {
        if (delta >= (1 << i)) {
            cost = std::min(sum[u][i], cost);
            u = jmp[u][i];
            delta -= (1 << i);
        }
    }

    if (u == v) {
        return cost;
    }

    for (int i = int(std::log2(depths.size())) + 1; i >= 0; i--) {
        int g = jmp[u][i];
        int f = jmp[v][i];
        if (g != f) {
            cost = std::min(sum[u][i], cost);
            cost = std::min(sum[v][i], cost);
            u = g;
            v = f;
        }
    }

    cost = std::min(sum[u][0], cost);
    cost = std::min(sum[v][0], cost);
    return cost;
}

int main() {
    int n = 0;
    std::ifstream minonpath ("minonpath.in");
    minonpath >> n;

    std::vector<std::vector<int>> graph(n);
    std::vector<std::pair<int, int>> parents(n,std::make_pair(0, 0) );// v -> (parent, cost)
    parents[0] = std::make_pair(0, INT_MAX);
    for (size_t i = 2; i <= n; ++i) {
        int v, cost;
        minonpath >> v >> cost;
        graph[v - 1].push_back(i - 1);
        parents[i - 1] = std::make_pair(v - 1, cost);
    }
    std::vector<int> depths(n, 0);
    dfs(graph, depths);
    std::vector<std::vector<int>> jmp(n);
    std::vector<std::vector<int>> sum(n);
    for (int v = 0; v < n; ++v) {
        for (int i = 0; i < int(std::log2(n)) + 2; ++i) {
            if (i == 0) {
                auto p = parents[v];
                jmp[v].push_back(p.first);
                sum[v].push_back(p.second);
            } else {
                auto step = jmp[v][i - 1];
                jmp[v].push_back(jmp[step][i - 1]);
                sum[v].push_back(std::min(sum[v][i - 1], sum[step][i - 1]));
            }
        }
    }
    int m = 0;
    minonpath >> m;
    std::ofstream out("minonpath.out");
    for (size_t i = 0; i < m; ++i) {
        int u, v;
        minonpath >> u >> v;
        out << lca_cost(depths, jmp, sum, u - 1, v - 1) << std::endl;
    }

    return 0;
}
