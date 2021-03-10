//
// Created by Ovanes on 3/10/20.
//

// Своим уравнением Ax + By + C = 0 задана прямая на плоскости. Требуется найти любую принадлежащую этой прямой точку,
// координаты которой — целые числа от  - 5·1018 до 5·1018 включительно, или выяснить что таких точек нет.

// Входные данные
// В первой строке содержатся три целых числа A, B и C ( - 2·10^9 ≤ A, B, C ≤ 2·10^9) —
// соответствующие коэффициенты уравнения прямой. Гарантируется, что A^2 + B^2 > 0.
// Выходные данные
// Если искомая точка существует, выведите ее координаты, иначе выведите единственное число -1.

#include<iostream>

bool isCorrect(long long int x) {
    return static_cast<long long int>(-5e+18) <= x && x <= static_cast<long long int>(5e+18);
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
    long long int a, b, c;
    std::cin >> a >> b >> c;
    c = -c;

    long long int local_gcd = GCD(a, b);

    if (c % local_gcd) {
        std::cout << "-1" << std::endl;
        return 0;
    }
    a /= local_gcd;
    b /= local_gcd;
    c /= local_gcd;
    long long int x, y;
    extendGCD(a, b, x, y);
    x *= c;
    y *= c;
    if (isCorrect(x) && isCorrect(y)) {
        std::cout << x << " " << y << std::endl;
    } else {
        std::cout << "-1" << std::endl;
    }

    return 0;
}
