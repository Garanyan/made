#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm>

// Факторизация, Ро-алгоритм Полларда
//        ограничение по времени на тест 3 секунды
//        ограничение по памяти на тест 256 мегабайт
//        ввод стандартный ввод
//        вывод стандартный вывод
//
//        Дано натуральное число. Факторизуйте его, то есть представьте в виде произведения набора простых чисел.
//        Число p называется простым, если имеет ровно два различных натуральных делителя: 1 и p.
//
// Входные данные
// В единственной строке записано единственное натуральное число N. 2 ≤ N ≤ 9·1018.
//
// Выходные данные
// Выведите в неубывающем порядке одно или несколько простых чисел, произведение которых равно N.

unsigned long long moduloMultiply(unsigned long long lhs, unsigned long long rhs, unsigned long long modulo) {
    unsigned long long res = 0; // Initialize result

    // Update lhs if it is more than or equal to modulo
    lhs %= modulo;

    while (rhs) {
        // If rhs is odd, add lhs with result
        if (rhs & 1)
            res = (res + lhs) % modulo;

        // Here we assume that doing 2 * lhs doesn't cause overflow
        lhs = (2 * lhs) % modulo;

        rhs >>= 1; // rhs = rhs / 2
    }

    return res;
}

unsigned long long int power(unsigned long long int num, unsigned long long int degree, unsigned long long int mod) {
    if (degree == 0) {
        return 1;
    }

    if (degree == 1) {
        return num % mod;
    }

    if (degree % 2 == 0) {
        auto r = power(num, degree / 2, mod);

        return moduloMultiply(r, r, mod);
    }

    auto r = power(num, degree / 2, mod);
    auto in = moduloMultiply(num, r, mod);

    return moduloMultiply(in, r, mod);
}


bool isPrime(unsigned long long candidate) {

    int accuracy_iterations = 12;
    if (candidate == 2 || candidate == 3 || candidate == 5 || candidate == 7) {
        return true;
    }
    if (candidate == 1 || candidate == 4 || candidate % 2 == 0 || candidate % 3 == 0 || candidate % 5 == 0 ||
        candidate % 7 == 0) {
        return false;
    }

    auto num = candidate - 1;
    unsigned long long degree = 0;
    while (num % 2 == 0) {
        ++degree;
        num /= 2;
    }

    for (int i = 0; i < accuracy_iterations; ++i) {
        unsigned long long a = 2 + rand() % (candidate - 4);

        a = power(a, num, candidate);
        if (a == 1 || a == candidate - 1) {
            continue;
        }
        long long step = 1;
        for (; step < degree; ++step) {
            a = moduloMultiply(a, a, candidate);
            if (a == 1) {
                return false;
            }
            if (a == candidate - 1) {
                break;
            }
        }
        if (step == degree)
            return false;
    }

    return true;
}

unsigned long long GCD(unsigned long long a, unsigned long long b) {
    if (a == 0) {
        return b;
    }
    if (b == 0) {
        return a;
    }
    return GCD(b, a % b);
}

unsigned long long f(unsigned long long x, unsigned long long c, unsigned long long mod) {
    return (moduloMultiply(x, x, mod) + c) % mod;
}

unsigned long long pollardRho(unsigned long long n) {
    if (n % 2 == 0)
        return 2;

    if (n % 3 == 0)
        return 3;

    unsigned long long x = rand() % (n - 2) + 1;

    unsigned long long c = 7;
    unsigned long long y = 1;
    unsigned long long i = 0, stage = 2;
    unsigned long long gcd = 1;

    // f(x) = x*x + c
    // (x_{i},x_{j}),
    // где j=2^{k} и  k пробегает 1, 2, 3, …
    // а i  принимает значения из интервала [2^{k} + 1; 2^{k + 1}].
    while (gcd == 1) {
        if (i == stage) {
            y = x;
            stage *= 2;
        }
        x = f(x, c, n);
        ++i;
        if (x > y) {
            gcd = GCD(x - y, n);
        } else {
            gcd = GCD(y - x, n);
        }
    }
    return gcd;
}


void factorization(unsigned long long n, std::vector<unsigned long long> &result) {

    if (n == 1) {
        return;
    }

    if (isPrime(n)) {
        result.push_back(n);
        return;
    }
    unsigned long long divisor = pollardRho(n);

    factorization(divisor, result);
    factorization(n / divisor, result);

}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(nullptr), std::cout.tie(nullptr);
    unsigned long long n = 0;
    std::cin >> n;
    std::vector<unsigned long long> factors;

    factorization(n, factors);
    std::sort(factors.begin(), factors.end());

    for (auto factor: factors) {
        std::cout << factor << " ";
    }
    std::cout << std::endl;

    return 0;
}
