//
// Created by Ovanes on 3/28/20.
//

// Сумма простая
//        ограничение по времени на тест 1 секунда
//        ограничение по памяти на тест 512 мегабайт
//        ввод стандартный ввод
//        вывод стандартный вывод
//
// Вам нужно научиться отвечать на запрос «сумма чисел на отрезке».
// Массив не меняется. Запросов много. Отвечать на каждый запрос следует за O(1).
//
// Входные данные
// Размер массива — n и числа x, y, a0, порождающие массив a: a_i = (x*a_{i-1} + y) mod 2^16.
// Далее следует количество запросов m и числа z, t, b0, порождающие массив b: b_i = (z*b_{i-1} + t) mod 2^30
// Массив c строится следующим образом: c_i = b_i mod n
// Запросы: i-й из них — найти сумму на отрезке от min(c_2i, c_{2i + 1}) до max(c2i, c_{2i + 1}) в массиве a.
//
// Ограничения: 1 ≤ n ≤ 10^7, 0 ≤ m ≤ 10^7. Все числа целые от 0 до 2^16. t может быть равно  - 1.
//
// Выходные данные
// Выведите сумму всех сумм.

#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(nullptr), std::cout.tie(nullptr);
    const long long int seq_mod = 1 << 16;
    const long long int request_mod = 1 << 30;

    long long n, x, y, a_0;
    std::cin >> n >> x >> y >> a_0;
    std::vector<long long> A(n, a_0);
    std::vector<long long> sum_(A.size(), A[0]);

    long long m, z, t, b_0;
    std::cin >> m >> z >> t >> b_0;
    std::vector<long long> B(2 * m, b_0);

    for (long long i = 1; i < A.size(); ++i) {
        A[i] = (x * A[i - 1] + y) & (seq_mod - 1);
        sum_[i] = sum_[i - 1] + A[i];
    }

    for (long long i = 1; i < B.size(); ++i) {
        B[i] = ((z * B[i - 1]) + t) % request_mod;
        if (B[i] < 0) {
            B[i] += request_mod;
        }
    }

    long long result = 0;

    for (long long i = 0; i < m; ++i) {
        long long l = B[2 * i] % n, r = B[2 * i + 1] % n;
        if (l > r) {
            auto tmp = r;
            r = l;
            l = tmp;
        }

        long long rsq;
        if (l == 0) {
            rsq = sum_[r];
        } else {
            rsq = sum_[r] - sum_[l - 1];
        }
        result += rsq;
        //std::cout << "[" << l << "," << r << "]" << " = " << rsq << std::endl;

    }
    std::cout << result << std::endl;
    return 0;
}
