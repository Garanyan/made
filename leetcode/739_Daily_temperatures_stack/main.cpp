#include <string>
#include <iostream>
#include <cstddef>
#include <cassert>
#include <string>
#include <vector>
#include <stack>

class Solution {
public:
    vector<int> dailyTemperatures(vector<int>& T) {
        std::vector<int> result(T.size());
        std::stack<std::pair<int,int>> pos;
        
        for(int i = 0; i < T.size(); ++i)
        {
            while(!pos.empty() && pos.top().second < T[i])
            {
                auto prev = pos.top();
                pos.pop();
                
                result[prev.first] = i - prev.first;
            }
            
            pos.emplace(std::make_pair(i, T[i]));
        }
        
        return result;
    }
};

int main(int argc, char **argv){

    return 0;
}