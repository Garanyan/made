//
// Created by ovanes on 06/03/2020.
//

// Выбор вершин дерева (поддеревья)

//        ограничение по времени на тест 2 секунды
//        ограничение по памяти на тест 256 мегабайт
//        ввод стандартный ввод
//        вывод стандартный вывод
//
//        Дан граф, являющийся деревом. Необходимо найти размер наибольшего по мощности независимого множества.
//        Множество вершин графа называется независимым, если никакие две вершины этого множества не соединены ребром.

// Входные данные
// Граф в этой задаче задан в виде корневого дерева. В графе выделена вершина — корень дерева.
// Для каждой вершины i, не являющейся корнем, задан номер вершины-предка pi в корневом дереве.
// Дерево, заданное таким образом, состоит из рёбер i—pi для всех вершин i, кроме корня.
// В первой строке входного файла записано целое число n — количество вершин в графе (1 ≤ n ≤ 100).
// В следующих n строках задан граф. В i-й из этих строк записано целое число pi — номер вершины-предка i-й вершины.
// Для корня дерева pi = 0; для всех остальных вершин 1 ≤ pi ≤ n.
// Гарантируется, что заданный во входном файле граф является деревом.

// Выходные данные
// В первой строке выходного файла выведите одно число — размер наибольшего по мощности независимого множества..

#include <iostream>
#include <vector>


void dfs(int v, std::vector<std::vector<int>> &graph, std::vector<int> &used, std::vector<std::vector<int>> &dp) {
    used[v] = 1;
    int total = 0;
    for (auto adj: graph[v]) {
        if (used[adj] == 0) {
            dfs(adj, graph, used, dp);
            total += std::max(dp[adj][0], dp[adj][1]);
        }
    }

    dp[v][0] = total;

//    dp[v][1] = 1;
    total = 0;
    for (auto adj: graph[v]) {
        if (used[adj] == 2) {
            total += dp[adj][0];
//                    std::max(dp[v][1],
//                             total - std::max(dp[adj][0], dp[adj][1])
//                             + dp[adj][0] + 1);
        }
    }
    dp[v][1] = std::max(1, total + 1);
    used[v] = 2;

}

int main(int argc, char **argv) {
    int N;
    std::cin >> N;
    std::vector<int> input(N);
    for (auto &i : input) {
        std::cin >> i;
    }

    std::vector<std::vector<int>> graph(N);
    int root_id = 0;
    for (size_t i = 0; i < N; ++i) {
        if (input[i] != 0) {
            graph[input[i] - 1].push_back(i);
        } else {
            root_id = i;
        }
    }

    std::vector<std::vector<int>> dp(N, std::vector<int>(2, 1));
    std::vector<int> used(N, 0);
    dfs(root_id, graph, used, dp);
    std::cout << std::max(dp[root_id][0], dp[root_id][1]);
    return 0;
}
