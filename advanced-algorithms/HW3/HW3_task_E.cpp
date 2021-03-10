//
// Created by Ovanes on 3/13/20.
//

// Решите в целых числах систему уравнений
//
//        x≡a(mod n)
//        x≡b(mod m),        где n и m взаимно просты.
//        Среди решений следует выбрать наименьшее неотрицательное число.

//        Входные данные
//        Первая строка входных данных содержит число N, 1 ≤ N ≤ 10^4, — количество тестов, для которых нужно решить задачу.

//        Следующие N
//        строк содержат по четыре целых числа ai, bi, ni и mi (1 ≤ ni,mi ≤ 10^9, 0 ≤ ai < ni, 0 ≤ bi < mi).

//        Выходные данные
//        Для каждого из тестов выведите искомое наименьшее неотрицательное число xi.

#include <vector>
#include <cmath>
#include <iostream>

long long int extendGCD(long long int a, long long int b, long long int &x, long long int &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    } else if (a == 0) {
        x = 0;
        y = 1;
        return b;
    }
    auto ans = extendGCD(b, a % b, x, y);
    long long int temp = x;
    x = y;
    y = temp - (a / b) * y;
    return ans;
}

int main(int argc, char **argv) {
    long long int test_num = 0;
    std::cin >> test_num;
    long long int a, b, n, m;
    while (test_num--) {
        std::cin >> a >> b >> n >> m;
        long long int x = 0;
        long long int y = 0;
        auto gcd = extendGCD(n, m, x, y);

        auto mod = static_cast<long long int>(std::fabs((m / gcd) * n));
        long long int ans = (a + ((x * ((b - a) / gcd) % (m / gcd)) * n)) % mod;
        while (ans < 0) {
            ans += mod;
        }
        std::cout << ans << std::endl;
    }
    return 0;
}
