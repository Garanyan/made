//
// Created by Ovanes.
//


#include <string>
#include <iostream>
#include <vector>
#include <climits>

int main(int argc, char** argv) {

    int days = 0;
    std::cin >> days;
    std::vector<int> costs(days, 0);
    int all_costs = 0;
    for (auto &c: costs){
        std::cin >> c;
        all_costs += c;
    }
    std::vector<std::vector<int>> dp(days + 1, std::vector<int>(days + 2, all_costs * 100));
    //dp[0] = std::vector<int>(days, );
    dp[0][0] = 0;
    for (int i = 1; i < days + 1; ++i){
        for (int j = 0; j < days + 1; ++j){


            if (costs[i - 1] > 100){
                dp[i][j] = std::min(dp[i - 1][j + 1], j == 0 ? all_costs * 100 : dp[i - 1][j - 1] + costs[i - 1]);

            }
            else{
                dp[i][j] = std::min(dp[i - 1][j + 1], dp[i - 1][j] + costs[i - 1]);

            }
        }

    }

    int k1 = 0;
    int min_sum = 100 * all_costs;
    for (size_t i = 0; i < dp.back().size(); ++i){
        if (dp.back()[i] <= min_sum){
            min_sum = dp.back()[i];
            k1 = i;
        }
    }
    int k2 = 0;

    int coupons = k1;
    std::vector<int> result;
    int next_sum = min_sum;
    for (int i = days - 1; i > 0; --i){
        if (dp[i][coupons + 1] == next_sum){
            result.push_back(i + 1);
            ++coupons;
            ++k2;
        }
        else{
            auto p = costs[i];
            if (p <= 100 && next_sum == dp[i][coupons] + p){

            }
            else{
                --coupons;
            }

            next_sum -= p;
        }
    }
    std::cout << min_sum << std::endl;
    std::cout << k1 << " " << k2 << std::endl;
    for (auto it = result.rbegin(); it < result.rend(); it++){
        std::cout << *it << std::endl;
    }
    return 0;
}
