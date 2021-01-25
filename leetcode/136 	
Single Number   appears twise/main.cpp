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
    int singleNumber(vector<int>& nums) {
        int accumulator = 0;
        for(auto num : nums)
        {
            accumulator = accumulator ^ num;
        }
        return accumulator;
    }
};

int main(int argc, char **argv){

    return 0;
}