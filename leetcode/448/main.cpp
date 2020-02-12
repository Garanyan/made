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
    vector<int> findDisappearedNumbers(vector<int>& nums) {
        std::vector<bool> appeared( nums.size(), false);
        for(auto i : nums)
        {
            appeared[i-1]= true;
        }
        std::vector<int> result;
        for(size_t i = 0; i<appeared.size(); ++i)
        {
            if(!appeared[i])
                result.push_back(i+1);
        }
        return result;
    }
};

int main(int argc, char **argv){

    return 0;
}