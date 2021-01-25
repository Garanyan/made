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
    
    void generate(std::vector<string>& result, const string str, const int begin, const int end, int n)
    {
        if(str.size() == 2*n)
        {
            result.emplace_back(str);
            return;
        }
        
        if(begin < n)
        {
            generate(result, str + "(", begin+1, end, n);
        }
        
        if(end < begin)
        {
            generate(result, str + ")", begin, end+1, n);
        }
    }
    
    vector<string> generateParenthesis(int n) {
        std::vector<string> result;
        generate(result, "", 0, 0, n);
        return result;
    }
};

int main(int argc, char **argv){

    return 0;
}