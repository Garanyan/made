//
// Created by Ovanes on 13.10.2019.
//
//

#include <string>
#include <iostream>
#include <cstddef>
#include <cassert>

class Solution {
public:
    
    vector<int> twoSum(vector<int>& nums, int target) {
        vector<int> v;
        for(int i=0; i<nums.size(); i++)
        {
            for(int j=i+1; j<nums.size(); j++)
            {
                if(nums[i]+nums[j]==target)
                {
                    v.push_back(i);
                    v.push_back(j);
                    break;
                }
            }
        }
        return v;
    }
    
};

int main(int argc, char **argv){

    return 0;
}