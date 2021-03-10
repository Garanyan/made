//
// Created by Ovanes on 3/14/20.
//
//
// A. Первообразный корень по простому модулю
//        ограничение по времени на тест 2 секунды
//        ограничение по памяти на тест 256 мегабайт
//        ввод стандартный ввод
//        вывод стандартный вывод
//
//        Дано простое нечётное число p
//        Требуется найти минимальный первообразный корень по модулю p
//

// Входные данные
// Первая строка входного файла содержит простое число p (3≤p≤10^9)

//Выходные данные
//Выведите ответ на поставленную задачу.

#include <iostream>
#include <vector>

long long int power(long long int num, long long int degree, long long int mod) {
    if (degree == 0) {
        return 1;
    }

    if (degree == 1) {
        return num % mod;
    }

    if (degree % 2 == 0) {
        auto r = power(num, degree / 2, mod);
        return (r * r) % mod;
    } else {
        auto r = power(num, degree / 2, mod);
        return (((num * r) % mod) * r) % mod;
    }

}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);


    long long int p;
    while (std::cin >> p) {
        long long int phi = p - 1;
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
                std::cout << i << std::endl;
                break;
            }
        }
    }
    return 0;
}
