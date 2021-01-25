#include <string>
#include <iostream>
#include <cstddef>
#include <cassert>

class Solution {
public:
    int findDuplicate(vector<int>& nums) {
        int rabb = nums[0];
        int t = nums[0];
        
        rabb = nums[rabb];
        t = nums[nums[t]];
        
        while(rabb != t)
        {
             rabb = nums[rabb];
            t = nums[nums[t]];
        }
        
        t = nums[0];
        while(t!=rabb)
        {
            rabb = nums[rabb];
            t = nums[t];
        }
        
        return rabb;
    }
};

int main(int argc, char **argv){

    return 0;
}