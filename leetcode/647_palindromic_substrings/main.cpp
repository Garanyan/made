#include <string>
#include <iostream>
#include <cstddef>
#include <cassert>

class Solution {
public:
    int countSubstrings(string s) {
        int ans = 0;
        std::vector<std::vector<bool>> dp(s.size(), std::vector(s.size(), false));
        for(int i = 0; i < s.size(); ++i)
        {
            dp[i][i] = true;
        
            if(i + 1 < s.size())
            {
                dp[i][i+1] = s[i] == s[i+1];
                ans+=dp[i][i+1];
            }
            ans += dp[i][i];
        }
        
        for(int len = 3; len <= s.size(); ++len)
        {
            for(int i = 0, j = i + len - 1; j < s.size(); ++i, ++j)
            {
                dp[i][j] = dp[i+1][j-1] && (s[i]==s[j]);
                ans += dp[i][j];
            }
        }
        
        return ans;
    }
};

int main(int argc, char **argv){

    return 0;
}