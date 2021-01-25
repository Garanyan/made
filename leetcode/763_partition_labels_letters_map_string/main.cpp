#include <string>
#include <iostream>
#include <cstddef>
#include <cassert>

class Solution {
public:
    vector<int> partitionLabels(string S) {
        std::map<char, int> last_pos;
        for(size_t i = 0; i < S.size(); i++)
        {
            last_pos[S[i]] = i;
        }
        vector<int> result;
        int start = 0;
        int end = last_pos[S[0]];
        for(size_t i = 0; i < S.size(); i++)
        {
            end = std::max(end, last_pos[S[i]]);
            if(end == i)
            {
                result.push_back(end - start + 1);
                start = end + 1;
            }
        }
        return result;
                                         
    }
};

int main(int argc, char **argv){

    return 0;
}