//
// Created by Ovanes on 12.02.2020.
//
//

#include <string>
#include <iostream>
#include <cstddef>
#include <cassert>


class Solution {
public:
    vector<vector<int>> permute(vector<int>& nums) {
        if(nums.size() == 1)
        {
            return {nums};
        }
        
        if(nums.size() == 2)
        {
            return {{nums[0], nums[1]}, {nums[1], nums[0]}};
        }
        
        std::vector<std::vector<int>> result;
        for(int i = 0; i  < nums.size(); i++)
        {
            auto tmp = nums;
            tmp.erase(tmp.begin() + i);
            auto permuts = permute(tmp);
            
            for(auto& u: permuts)
            {
                u.insert(u.begin(), nums[i]);
                result.emplace_back(u);
            }
        }
        
        return result;
    }
};

int main(int argc, char **argv){

    return 0;
}