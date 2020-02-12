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
    int maxProfit(vector<int>& prices) {
        int minprice = INT_MAX;
        int result = 0;
        for(int i = 0; i < prices.size(); i++)
        {
            if(prices[i] < minprice)
                minprice = prices[i];
            else if (prices[i] - minprice > result)
                result = prices[i] - minprice;
        }
        return result;
    }
};

int main(int argc, char **argv){

    return 0;
}