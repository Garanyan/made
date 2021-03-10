//
// Created by Ovanes on 28.02.2020.
//

#include <string>
#include <iostream>
#include <vector>

int main(int argc, char** argv)
{

    int N = 0, M = 0;
    std::cin >> N >> M;

    std::vector<std::vector<int>> field_with_money(N, std::vector<int>(M, 0));
    for (auto& row: field_with_money)
    {
        for (auto& cell: row)
        {
            std::cin >> cell;
        }
    }

    std::vector<std::vector<int>> dp(N, std::vector<int>(M, 0));
    dp[0][0] = field_with_money[0][0];
    for (int row = 0; row < N; ++row)
    {
        for (int col = 0; col < M; ++col)
        {
            if (row == 0 && col != 0)
            {
                dp[row][col] = dp[row][col - 1] + field_with_money[row][col];
            }
            else if (col == 0 && row != 0)
            {
                dp[row][col] = dp[row - 1][col] + field_with_money[row][col];
            }
            else if (col != 0 && row != 0)
            {
                dp[row][col] = std::max(dp[row][col - 1], dp[row - 1][col]) + field_with_money[row][col];
            }
        }
    }


    std::string path(M - 1 + N - 1, ' ');
    int row = N - 1, col = M - 1;
    for (int current_move_id = path.size() - 1; current_move_id >= 0; --current_move_id)
    {

        if ((row == 0 && col != 0) || (row != 0 && col != 0 && dp[row][col - 1] > dp[row - 1][col]))
        {
            path[current_move_id] = 'R';
            --col;
        }
        else
        {
            path[current_move_id] = 'D';
            --row;
        }

    }
    std::cout << dp.back().back() << std::endl;
    std::cout << path << std::endl;

    return 0;
}
