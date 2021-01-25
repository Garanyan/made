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
    vector<int> topKFrequent(vector<int>& nums, int k) {
        std::map<int,int> num_to_freq;
        for(auto i: nums)
        {
            num_to_freq[i]++;
        }
        
        std::map<int, std::vector<int>> freq_num;
        for(auto i: num_to_freq)
        {
            freq_num[i.second].emplace_back(i.first);
        }
        
        std::vector<int> result;
        for(auto it = freq_num.rbegin(); it != freq_num.rend(); it++)
        {
            for(auto e: it->second){
                result.emplace_back(e);
                if(result.size() == k)
                    break;
            }
            if(result.size() == k)
                    break;
        }
        
        return result;
    }
};

int main(int argc, char **argv){

    return 0;
}