//
// Created by Ovanes on 3/9/20.
//

// Для положительного целого n определим функции:
// d(n) — минимальный делитель n, больший 1, по определению положим d(1) = 0.
// s0(n) — количество различных делителей n.
// s1(n) — сумма всех делителей n.
// φ(n) — функция Эйлера, количество целых чисел k, таких что 1 ≤ k ≤ n и GCD(n, k) = 1.

// По данному числу n вычислите sum_1^N(d(i)), sum_1^N(s0(i)), sum_1^N(s1(i)), sum_1^N(φ(i)) .

// Входные данные
// В единственной строке записано число n (1 ≤ n ≤ 10^7).
// Выходные данные
// Выведите четыре числа: sum_1^N(d(i)), sum_1^N(s0(i)), sum_1^N(s1(i)), sum_1^N(φ(i)) .

#include <vector>
#import <iostream>
#include <map>

int main() {

    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    int n;
    std::cin >> n;
    n += 1;
    std::vector<unsigned long long int> lp(n + 1, 0);
    std::vector<unsigned long long int> primes;

    for (unsigned long long int i = 2; i < lp.size(); i++) {
        if (lp[i] == 0) {
            lp[i] = i;
            primes.push_back(i);

        }
        for (unsigned long long int p : primes) {
            if (p > lp[i] || i * p >= lp.size()) {
                break;
            }

            lp[i * p] = p;

        }
    }
    lp[1] = 1;

    unsigned long long int total_d = 0, total_s_0 = 0, total_s_1 = 0, total_phi = 0;
    for (unsigned long long int i = 1; i < n; ++i) {
        unsigned long long int num = i;
        unsigned long long int d = lp[num], s_0 = 1, s_1 = 1, phi = 1;
        if (i == 1) {
            d = 0;
        }

        unsigned long long int last_den = lp[num];
        unsigned long long int degr = 1;
        unsigned long long int counts = 0;
        while (num > 1) {


            if (last_den == lp[num]) {
                degr *= lp[num];
                ++counts;
            } else {
                degr /= last_den;

                phi *= degr * (last_den - 1);
                s_0 *= counts + 1;
                s_1 *= (degr * last_den * last_den - 1) / (last_den - 1);

                degr = lp[num];
                counts = 1;
            }
            last_den = lp[num];
            num /= lp[num];

        }
        if (i != 1) {
            degr /= last_den;

            phi *= degr * (last_den - 1);
            s_0 *= counts + 1;
            s_1 *= (degr * last_den * last_den - 1) / (last_den - 1);
        }

        total_d += d;
        total_s_0 += s_0;
        total_s_1 += s_1;
        total_phi += phi;
    }

    std::cout << total_d << " " << total_s_0 << " " << total_s_1 << " " << total_phi << std::endl;

    return 0;
}
