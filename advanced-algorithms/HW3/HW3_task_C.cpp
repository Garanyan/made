//
// Created by Ovanes Garanyan on 6/20/20.
//

/*
 * Взлом RSA
 *
ограничение по времени на тест 2 секунды
ограничение по памяти на тест 64 мегабайта
ввод стандартный ввод
вывод стандартный вывод

В 1977 году Ronald Linn Rivest, Adi Shamir и Leonard Adleman предложили новую криптографическую схему RSA,
используемую до сих пор. RSA является криптосистемой с открытым ключом: зашифровать сообщение может кто угодно,
знающий общеизвестный открытый ключ, а расшифровать сообщение — только тот, кто знает специальный секретный ключ.

Желающий использовать систему RSA для получения сообщений должен сгенерировать два простых числа p и q,
вычислить n = pq и сгенерировать два числа e и d такие, что {ed ≡ 1 ± od{(p - 1)(q - 1)}}
(заметим, что {(p - 1)(q - 1) = φ(n)}). Числа n и e составляют открытый ключ и являются общеизвестными.
Число d является секретным ключом, также необходимо хранить в тайне и разложение числа n на простые множители,
так как это позволяет вычислить секретный ключ d.

Сообщениями в системе RSA являются числа из . Пусть M — исходное сообщение.
 Для его шифрования вычисляется значение (для этого необходимо только знание открытого ключа).
 Полученное зашифрованное сообщение C передается по каналу связи. Для его расшифровки необходимо вычислить значение,
 а для этого необходимо знание секретного ключа.

Вы перехватили зашифрованное сообщение C и знаете только открытый ключ: числа n и e.
"Взломайте" RSA — расшифруйте сообщение на основе только этих данных.

Входные данные
Программа получает на вход три натуральных числа: n, e, C, n ≤ 10^9, e ≤ 10^9, C < n.
Числа n и e являются частью какой-то реальной схемы RSA, т.е. n является произведением двух простых и e
взаимно просто с φ(n). Число C является результатом шифрования некоторого сообщения M.

Выходные данные
Выведите одно число M (0 ≤ M < n), которое было зашифровано такой криптосхемой.

 */

#include <iostream>
#include <cmath>

void extend_GCD(long long int a, long long int b, long long int &x, long long int &y) {
    if (b == 0){
        x = 1;
        y = 0;
        return;
    }
    else if (a == 0){
        x = 0;
        y = 1;
        return;
    }
    extend_GCD(b, a % b, x, y);
    long long int temp = x;
    x = y;
    y = temp - (a / b) * y;
}

// e*d = 1 mod (phi)
long long int search_d(const int e, const int phi) {
    long long int x, y;
    extend_GCD(e, phi, x, y);
    x = (x % phi + phi) % phi;
    return x;
}

std::pair<int, int> terms(int n) {
    if (n % 2 == 0){
        return std::make_pair(2, n / 2);
    }
    const auto bound = std::sqrt(n);
    for (int i = 3; i <= bound; i += 2){
        if (n % i == 0){
            return std::make_pair(i, n / i);
        }
    }
    return {n, 1};
}

unsigned long long modulo_multiply(unsigned long long lhs, unsigned long long rhs, unsigned long long modulo) {
    unsigned long long res = 0;
    lhs %= modulo;

    while (rhs){
        // If rhs is odd, add lhs with result
        if (rhs & 1){
            res = (res + lhs) % modulo;
        }

        // Here we assume that doing 2 * lhs doesn't cause overflow
        lhs = (2 * lhs) % modulo;

        rhs >>= 1; // rhs = rhs / 2
    }

    return res;
}

unsigned long long int power(unsigned long long int num, unsigned long long int degree, unsigned long long int mod) {
    if (degree == 0){
        return 1;
    }

    if (degree == 1){
        return num % mod;
    }

    if (degree % 2 == 0){
        auto r = power(num, degree / 2, mod);

        return modulo_multiply(r, r, mod);
    }

    auto r = power(num, degree / 2, mod);
    auto in = modulo_multiply(num, r, mod);

    return modulo_multiply(in, r, mod);
}

unsigned long long decrypt_RCA(int n, int e, int encrypted) {
    std::pair<int, int> pq = terms(n);
    int euler_phi = (pq.first - 1) * (pq.second - 1);
    auto d = search_d(e, euler_phi);
    return power(encrypted, d, n);
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(nullptr), std::cout.tie(nullptr);

    int n, e, c;
    std::cin >> n >> e >> c;
    std::cout << decrypt_RCA(n, e, c) << std::endl;

    return 0;
}
