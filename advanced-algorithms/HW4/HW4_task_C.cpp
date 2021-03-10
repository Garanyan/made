//Корень по модулю
//        ограничение по времени на тест 4 секунды
//        ограничение по памяти на тест 256 мегабайт
//        ввод стандартный ввод
//        вывод стандартный вывод
//
// Даны числа a, b и m, (1≤a,b<m≤10^9), m — простое. Требуется найти корень степени b из a по модулю m.
// Входные данные
// Входной файл состоит из одного или нескольких тестов. Количество тестов T ≤ 300 задано в первой строке.
// В T последующих строках заданы по три числа в каждой — a, b и m.
//
// Выходные данные
// Для каждого из тестов выведите какой-либо из корней степени b из a по модулю m,
// либо -1, если искомого корня не существует.
// Выводимое число должно быть в пределах от -1 до m − 1.

#include <iostream>
#include <vector>
#include <cmath>
#include <map>


unsigned long long int power(unsigned long long int num, unsigned long long int degree, unsigned long long int mod) {
    if (degree == 0) {
        return 1;
    }

    if (degree == 1) {
        return num % mod;
    }

    if (degree % 2 == 0) {
        auto r = power(num, degree / 2, mod);

        return (r * r) % mod;
    }

    auto r = power(num, degree / 2, mod);
    auto in = (num * r) % mod;

    return (in * r) % mod;
}

unsigned long long getGenerator(unsigned long long p) {

    unsigned long long generator = 1;
    unsigned long long int phi = p - 1;
    std::vector<long long int> prime_devisors;
    auto n = phi;
    for (long long int i = 2; i * i <= phi; ++i) {
        if (n % i == 0) {
            prime_devisors.emplace_back(i);
            while (!(n % i)) {
                n /= i;
            }
        }
    }
    if (n > 1) {
        prime_devisors.push_back(n);
    }

    for (long long int i = 2; i <= p; ++i) {
        bool found_generator = true;
        for (auto prime: prime_devisors) {
            if (power(i, phi / prime, p) == 1) {
                found_generator = false;
                break;
            }
        }

        if (found_generator) {
            generator = i;
            break;
        }
    }

    return generator;
}

bool discreteLogarithm(unsigned long long a, unsigned long long b, unsigned long long p, unsigned long long &s) {
    unsigned long long H = std::sqrt(p) + 1;

    const auto c = power(a, H, p);
    std::map<uint64_t, uint64_t> c_table;
    uint64_t celement = c;

    for (uint64_t i = 1; i <= H; ++i) {
        c_table[celement] = i;

        celement = (c * celement) % p;

    }

    std::map<uint64_t, uint64_t> b_table;
    unsigned long long belement = b;

    for (uint64_t i = 0; i <= H; ++i) {
        if (c_table.find(belement) != c_table.end()) {

            s = (((((H % (p - 1)) * (c_table[belement] % (p - 1))) % (p - 1)) + (p - 1)) - i % (p - 1)) %
                (p - 1);
            return true;
        }


        belement = (a * belement) % p;

    }
    return false;
}


long long int GCD(long long int a, long long int b) {
    if (a == 0) {
        return b;
    }
    if (b == 0) {
        return a;
    }
    return GCD(b, a % b);
}

void extendGCD(long long int a, long long int b, long long int &x, long long int &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return;
    } else if (a == 0) {
        x = 0;
        y = 1;
        return;
    }
    extendGCD(b, a % b, x, y);
    long long int temp = x;
    x = y;
    y = temp - (a / b) * y;
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(nullptr), std::cout.tie(nullptr);

    unsigned long long T = 0;
    std::cin >> T;

    std::map<unsigned long long, unsigned long long> prime_to_generator;
    for (unsigned long long i = 0; i < T; ++i) {
        unsigned long long a, b, m;
        std::cin >> a >> b >> m;

        unsigned long long g;
        auto it = prime_to_generator.find(m);
        if (it != prime_to_generator.end()) {
            g = it->second;
        } else {
            g = getGenerator(m);
        }

        unsigned long long s;

        if (!discreteLogarithm(power(g, b, m), a, m, s)) {
            std::cout << -1 << "\n";
            continue;
        }
        std::cout << power(g, s, m) << "\n";

    }

    return 0;
}
