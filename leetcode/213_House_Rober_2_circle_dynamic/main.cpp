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
    int rob_street(vector<int>& nums)
    {
        std::vector<int> rob(nums.size(), 0);
        std::vector<int> no_rob(nums.size(), 0);
        for(int i =0; i < nums.size(); ++i)
        {
            if(i==0)
            {
                rob[i] = nums[i];
                no_rob[i] = 0;
            }
            else{
                rob[i] = nums[i] + no_rob[i-1];
                no_rob[i] = std::max(rob[i-1], no_rob[i-1]);
            }
        }
        return std::max(rob.back(), no_rob.back());
    }
    
    int rob(vector<int>& nums) {
        if(nums.size() == 1)
            return nums[0];
        vector<int> c(nums.begin(), nums.end()-1);
        vector<int> d(nums.begin()+1, nums.end());
        int a = rob_street(c);
        int b = rob_street(d);
        return std::max(a,b);
    }
};
int main(int argc, char **argv){

    return 0;
}