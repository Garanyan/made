//
// Created by Ovanes on 19.01.2021.
//
//

#include <string>
#include <iostream>
#include <cstddef>
#include <cassert>



class FenwickTree {
public:
    explicit FenwickTree(int s);


    long long sum(long long i, long long j);

    void add(long long i, long long x);

private:
    long long get(long long i);


    std::vector<long long> T;

};

long long F(long long i) {
    return i & (i + 1);
}


FenwickTree::FenwickTree(int s) : T(s, 0) {

}

long long FenwickTree::sum(long long i, long long j) {
    if (i == 0) {
        return get(j);
    }

    return get(j) - get(i - 1);
}

long long FenwickTree::get(long long i) {
    long long result = 0;
    while (i >= 0) {
        result += T[i];
        i = F(i) - 1;
    }
    return result;
}



void FenwickTree::add(long long i, long long x) {
    auto j = i;
    while (j < T.size()) {
        T[j] += x;
        j |= (j + 1);
    }
}


class Solution {
public:
    int createSortedArray(vector<int>& instructions) {
        int N = 100002;
        FenwickTree tree(N);
        long long ans = 0;
        for(int i = 0; i < instructions.size(); ++i )
        {
            int l = tree.sum(0, instructions[i]-1);
            int r = tree.sum(instructions[i]+1, N - 1);
            ans += std::min(l,r);
            ans = ans % 1000000007;
            tree.add(instructions[i],1);
        }
        return ans;
    }
};

int main(int argc, char **argv){

    return 0;
}