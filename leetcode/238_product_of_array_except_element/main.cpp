#include <string>
#include <iostream>
#include <cstddef>
#include <cassert>

class Solution {
public:
    vector<int> productExceptSelf(vector<int>& nums) {
        std::vector<int> pre_prod(nums.size(), 1);
        for(int i = 0; i < nums.size(); ++i)
        {
            if(i==0)
            {
                pre_prod[i] = nums[i];
            }
            else{
                pre_prod[i] = pre_prod[i-1]*nums[i];
            }
        }
        

        int acc = 1;
        for(int i = nums.size() - 1; i >=0; --i)
        {
            if(i!=0){
                    
                pre_prod[i] = acc * pre_prod[i-1];
                acc *= nums[i];
            }
            else{
                pre_prod[i] = acc;
            }
        }
        
        return pre_prod;
    }
};

int main(int argc, char **argv){

    return 0;
}