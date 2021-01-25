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
    vector<int> decompressRLElist(vector<int>& nums) {
        std::vector<int> result;
        size_t s=0;
        for(size_t i=0; i < nums.size()/2; ++i)
        {
            s+= nums[2*i];
        }
        result.reserve(s);
        for(size_t i=0; i < nums.size()/2; ++i)
        {
            for(size_t j =0; j < nums[2*i]; ++j)
            {
                result.push_back(nums[2*i+1]);
            }
        }
        return result;
    }
};

int main(int argc, char **argv){

    return 0;
}