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
    
    int lengthOfLongestSubstring(std::string s)  {
        int n = s.length();
        std::set<char> set;
        int ans = 0, i = 0, j = 0;
        while (i < n && j < n) {
            // try to extend the range [i, j]
            if (set.find(s[j])==set.end()){
                set.insert(s[j++]);
                ans = std::max(ans, j - i);
            }
            else {
                set.erase(s[i++]);
            }
        }
        return ans;
    }

    
};

int main(int argc, char **argv){

    return 0;
}