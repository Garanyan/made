//
// Created by Ovanes on 28.02.2020.
//


#include <iostream>
#include <vector>

int main(int argc, char **argv) {
    int N = 0;
    std::cin >> N;
    std::vector<int> sequence(N, 0);
    for (auto &n: sequence) {
        std::cin >> n;
    }

    std::vector<int> dp(N, 1);
    int max_sub_seq = 1;
    int sub_seq_end = 0;
    for (int i = 0; i < N; ++i) {
        //dp[] = 1;
        for (int j = 0; j < i; ++j) {
            if (sequence[j] < sequence[i] && dp[j] + 1 > dp[i]) {
                dp[i] = dp[j] + 1;
            }
        }
        if (max_sub_seq < dp[i]) {
            max_sub_seq = dp[i];
            sub_seq_end = i;
        }
    }

    std::cout << max_sub_seq << std::endl;
    std::vector<int> result(max_sub_seq, sequence[sub_seq_end]);
    int i = sub_seq_end;
    for (int n = max_sub_seq - 2; n >= 0; --n) {
        for (int j = i - 1; j >= 0; --j) {
            if (sequence[j] < sequence[i] && dp[j] + 1 == dp[i]) {
                i = j;
                result[n] = sequence[i];
                break;
            }
        }
    }

    for (auto it = result.begin(); it < result.end(); it++) {
        std::cout << *it << " ";
    }
    return 0;
}
