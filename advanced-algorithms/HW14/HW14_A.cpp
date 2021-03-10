//
// Created by Ovanes Garanyan on 6/13/20.
//

#include <complex>
#include <iostream>
#include <vector>
#include <cmath>

# define PI 3.141592653589793238462643383279502884197169399375105820974
typedef std::complex<double> complex_num;

void fft(std::vector<complex_num> &a) {
    int n = static_cast<int> (a.size());
    if (n == 1){
        return;
    }

    std::vector<complex_num> a_even(n / 2), a_odd(n / 2);
    for (int i = 0, j = 0; i < n; i += 2, ++j){
        a_even[j] = a[i];
        a_odd[j] = a[i + 1];
    }
    fft(a_even);
    fft(a_odd);

    double ang = 2 * PI / n;
    complex_num w(1), wn(cos(ang), sin(ang));
    for (int i = 0; i < n / 2; ++i){
        a[i] = a_even[i] + w * a_odd[i];
        a[i + n / 2] = a_even[i] - w * a_odd[i];
        w *= wn;
    }
}

std::vector<complex_num> ifft(std::vector<complex_num> &a) {
    fft(a);
    double n = static_cast<int> (a.size());
    std::vector<complex_num> inv_a;
    inv_a.reserve(a.size());
    inv_a.push_back(a.front() / n);
    for (auto it = a.rbegin(); it + 1 < a.rend(); it++){
        inv_a.push_back((*it) / n);
    }
    return inv_a;
}

void multiply(const std::vector<int> &a, const std::vector<int> &b, std::vector<int> &res, const int base) {
    std::vector<complex_num> fa(a.begin(), a.end()), fb(b.begin(), b.end());
    size_t n = 1;
    while (n < a.size() + b.size() - 1){
        n <<= 1;
    }
    //n <<= 1;
    fa.resize(n, 0), fb.resize(n, 0);

    fft(fa);
    fft(fb);
    for (size_t i = 0; i < n; ++i){
        fa[i] *= fb[i];
    }
    auto c = ifft(fa);

    res.resize(n);
    for (size_t i = 0; i < n; ++i){
        res[i] = lround(c[i].real());
    }

    int carryover = 0;
    for (size_t i = 0; i < n; ++i){
        res[i] += carryover;
        carryover = res[i] / base;
        res[i] %= base;
    }
}

int main() {
    const int base = 10;
    long long int x, y;
    std::cin >> x >> y;
    if (x == 0 || y == 0){
        std::cout << 0 << std::endl;
        return 0;
    }

    int x_negative = x < 0 ? -1 : 1;
    int y_negative = y < 0 ? -1 : 1;
    x = x < 0 ? -x : x;
    y = y < 0 ? -y : y;
    std::vector<int> a;
    std::vector<int> b;
    while (x){
        a.push_back(x % base);
        x /= base;
    }

    while (y){
        b.push_back(y % base);
        y /= base;
    }

    std::vector<int> res;
    multiply(a, b, res, base);
    if (x_negative * y_negative < 0){
        std::cout << '-';
    }
    int mul = 1;
    long long int ans = 0;
    for (auto it = res.begin(); it < res.end(); it++){
        ans += mul * (*it);
        mul *= base;
    }
    std::cout << ans << std::endl;
    return 0;
}
