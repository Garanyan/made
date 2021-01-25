#include <string>
#include <iostream>
#include <cstddef>
#include <vector>


class Solution {
public:
    int minPathSum(vector<vector<int>>& grid) {
        int m = grid.size();
        int n = grid.front().size();
        vector<vector<int>> min_cost(m , std::vector(n, 0));
        for(int i = 0; i < m; i++)
        {
            for(int j = 0; j < n; j++)
            {
                if(j >= 1 && i >= 1)
                    min_cost[i][j] = grid[i][j] + std::min(min_cost[i][j-1], min_cost[i-1][j]);
                else if(j>=1)
                {
                    min_cost[i][j] = grid[i][j] + min_cost[i][j-1];
                }
                else if(i>=1)
                {
                    min_cost[i][j] = grid[i][j] + min_cost[i-1][j];
                }
                else
                {
                    min_cost[i][j] = grid[i][j];
                }
            }
        }
        
        return min_cost.back().back();
    }
};

int main(int argc, char **argv){

    return 0;
}