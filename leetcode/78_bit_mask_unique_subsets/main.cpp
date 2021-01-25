#include <string>
#include <iostream>
#include <cstddef>
#include <cassert>

class Solution {
public:
    size_t getBit(size_t mask, size_t i) {
        return (mask >> i) & 1;
    }
    
    vector<vector<int>> subsets(vector<int>& nums) {
        vector<vector<int>> result;
        
        for (size_t mask = 0; mask < 1 << nums.size(); ++mask) {
            vector<int> subset;
            for (size_t i = 0; i < nums.size(); ++i) {
                if (getBit(mask, i) == 1) {
                    subset.emplace_back(nums[i]);
                }
            }
            result.push_back(subset);
        }
        
        return result;
    }
};

int main(int argc, char **argv){

    return 0;
}