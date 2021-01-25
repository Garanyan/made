#include <string>
#include <iostream>
#include <cstddef>
#include <cassert>


class Solution {
public:
    vector<vector<int>> reconstructQueue(vector<vector<int>>& people) {
        std::sort(people.begin(), people.end(), 
                 [](vector<int>& a, vector<int>& b){
                     return (a[0] > b[0] || (a[0]==b[0] && a[1] < b[1]));
                 });
            
        for(int i = 0; i < people.size(); ++i)
        {
            
            if( people[i][1] != i)
            {
                auto human = people[i];
                people.erase(people.begin() + i);
                people.insert(people.begin() + human[1], human);
            }
        }
        
        return people;
    }
};

int main(int argc, char **argv){

    return 0;
}