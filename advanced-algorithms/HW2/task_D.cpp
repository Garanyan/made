//
// Created by ovanes on 06/03/2020.
//

// Продавец аквариумов (коммивояжер)

#include <iostream>
#include <vector>

size_t getBit(size_t mask, size_t i) {
    return (mask >> i) & 1;
}

int main(int argc, char **argv) {
    int N = 0;
    int total_distnce = 0;

    std::cin >> N;
    std::vector<std::vector<int>> distance(N, std::vector<int>(N, 0));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            std::cin >> distance[i][j];
            total_distnce += distance[i][j];
        }
    }
    total_distnce *= 10;
    // dp[v][mask] кратчайший путь в вершину v через маску mask 1 раз в вершине

    int min_dist = total_distnce;
    std::vector<int> costs;
    std::vector<std::vector<int>> paths;
    for (size_t city = 0; city < N; ++city) {

        std::vector<std::vector<int>> dp(N, std::vector<int>((1 << N), total_distnce));
        dp[city][1 << city] = 0;
        for (size_t mask = 1; mask < 1 << N; ++mask) {
            for (size_t i = 0; i < N; ++i) {
                if (getBit(mask, i) == 1) {
                    for (size_t j = 0; j < N; ++j) {
                        if (getBit(mask, j) == 1) {
                            dp[i][mask] = std::min(dp[i][mask], dp[j][mask - (1 << i)] + distance[j][i]);
                        }
                    }
                }
            }
        }

        int last_city = 0;
        for (size_t u = 0; u < N; ++u) {
            if (min_dist > dp[u].back()) {
                min_dist = dp[u].back();
                last_city = u;
            }
        }
//        std::cout << min_dist << std::endl;
        costs.push_back(min_dist);
        int last_mask = (1 << N) - 1;
        std::vector<int> path;
        while (last_mask && last_city != city) {
//            std::cout << last_city + 1 << " ";
            path.push_back(last_city + 1);
            for (size_t i = 0; i < N; ++i) {

                if (getBit(last_mask - (1 << last_city), i) == 1) {
                    if (dp[last_city][last_mask] - distance[last_city][i] == dp[i][last_mask - (1 << last_city)]) {
                        last_mask = last_mask - (1 << last_city);
                        last_city = i;
                        break;

                    }
                }
            }
        }
        path.push_back(last_city + 1);
        paths.push_back(path);
    }

    int min_cost_id = 0;
    int min_cost = costs.front();
    for (size_t i = 1; i < costs.size(); ++i) {
        if (min_cost > costs[i]) {
            min_cost = costs[i];
            min_cost_id = i;
        }
    }

    std::cout << min_cost << std::endl;
    for (auto i : paths[min_cost_id]) {
        std::cout << i << " ";
    }
    return 0;
}
