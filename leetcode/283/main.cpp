//
// Created by Ovanes on 11.02.2020.
//
//

#include <string>
#include <iostream>
#include <cstddef>
#include <cassert>

class Solution {
public:
    void moveZeroes(vector<int>& nums) {
        size_t zeroes_num = 0;
        for(size_t i = 0; i < nums.size(); ++i)
        {
            if(nums[i]==0)
                ++zeroes_num;
            else if (zeroes_num > 0 && i-zeroes_num < nums.size())
            {
                nums[i-zeroes_num] = nums[i];
                nums[i]=0;
            }
        }
    }
};

int main(int argc, char **argv){

    return 0;
}