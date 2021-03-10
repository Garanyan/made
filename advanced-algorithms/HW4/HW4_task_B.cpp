//
// Created by Ovanes on 3/14/20.
//

// Дискретное логарифмирование
// ограничение по времени на тест 3 секунды
// ограничение по памяти на тест 256 мегабайт
// ввод стандартный ввод
// вывод стандартный вывод
//
//  Даны натуральные числа a, b, и простое число n.
//  Требуется найти дискретный логарифм b по основанию a по модулю n, то есть такое число x (0⩽x<n), что a^x≡b(mod n)
//

// Входные данные
// В первой строке заданы через пробел три целых числа a, b и n (0⩽a,b<n⩽10^12), Число n простое.

// Выходные данные
// В первой строке выведите −1, если дискретного логарифма не существует.
// Иначе следует вывести его значение.
// Если ответ неоднозначен, разрешается выводить любой.

#include <iostream>
#include <map>
#include <cmath>

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


int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(nullptr), std::cout.tie(nullptr);

    unsigned long long a, b, p, H;
    std::cin >> a >> b >> p;
    H = std::sqrt(p) + 1;

    const auto c = power(a, H, p);
    std::map<uint64_t, uint64_t> c_table;
    uint64_t celement = c;

    for (uint64_t i = 1; i <= H; ++i) {
        c_table[celement] = i;
        if (celement < c) {
            celement = moduloMultiply(celement, c, p);
        } else {
            celement = moduloMultiply(c, celement, p);
        }
    }


    std::map<uint64_t, uint64_t> b_table;
    unsigned long long belement = b;

    for (uint64_t i = 0; i <= H; ++i) {
        if (c_table.find(belement) != c_table.end()) {
            std::cout << ((moduloMultiply(H % (p - 1), c_table[belement] % (p - 1), p - 1) + (p - 1)) - i % (p - 1)) %
                         (p - 1)
                      << std::endl;
            return 0;
        }

        if (belement < a) {
            belement = moduloMultiply(belement, a, p);
        } else {
            belement = moduloMultiply(a, belement, p);
        }
    }
    std::cout << -1 << std::endl;

    return 0;
}
