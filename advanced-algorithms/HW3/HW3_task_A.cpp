//
// Created by Ovanes on 3/7/20.
//

// Дано много чисел. Требуется разложить их все на простые множители.

// Входные данные
// В первой строке задано число n (2 ≤ n ≤ 300000).
// В следующих n строках заданы числа ai (2 ≤ ai ≤ 10^6), которые нужно разложить на множители.

// Выходные данные
// Для каждого числа выведите в отдельной строке разложение на простые множители в порядке возрастания множителей.
// Дано много чисел. Требуется разложить их все на простые множители.


#include <vector>
#include <iostream>
#include <chrono>
#include <sstream>

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    int N;
    std::cin >> N;

    std::vector<int> input(N);
    int max_num = 0;

    for (auto &num:input) {
        std::cin >> num;
        if (max_num < num) {
            max_num = num;
        }
    }

    std::vector<int> lp(max_num + 1);
    std::vector<int> primes;
    for (int i = 2; i < lp.size(); i++) {
        if (lp[i] == 0) {
            lp[i] = i;
            primes.push_back(i);
        }
        for (int p : primes) {
            if (p > lp[i] || i * p >= lp.size()) {
                break;
            }
            lp[i * p] = p;
        }
    }

    int num = 0;
    std::stringstream out;
    for (size_t i = 0; i < N; ++i) {
        num = input[i];
        while (num > 1) {
            out << lp[num] << " ";
            num /= lp[num];
        }
        out << "\n";
    }

    std::cout << out.str();
    return 0;

}
