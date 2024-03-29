//
// Created by Ovanes on 3/31/20.
//

// Разреженные таблицы
//        ограничение по времени на тест 2 секунды
//        ограничение по памяти на тест 256 мегабайт
//        ввод стандартный ввод
//        вывод стандартный вывод
//
//        Дан массив из n чисел.
//        Требуется написать программу, которая будет отвечать на запросы следующего вида:
//        найти минимум на отрезке между u и v включительно.
//
// Входные данные
// В первой строке зданы три натуральных числа n , m (1 ⩽ n ⩽ 10^5, 1 ⩽ m ⩽ 10^7) и a1 (0 ⩽ a1 < 16 714 589) —
// количество элементов в массиве, количество запросов и первый элемент массива соответственно.
// Вторая строка содержит два натуральных числа u_1 и v_1 (1⩽u_1,v_1⩽n) — первый запрос.
// Для того, размер ввода был небольшой, массив и запросы генерируются.
// Элементы a_2, a_3, …, a_n задаются следующей формулой: a_{i+1}=(23 * a_i + 21 563) mod 16 714 589.
//
// Например, при n = 10 , a_1 = 12345 получается следующий массив:
// a = (12345, 305498, 7048017, 11694653, 1565158, 2591019, 9471233, 570265, 13137658, 1325095).
//
//        Запросы генерируются следующим образом:
//        u_{i+1} = ((17 * u_i + 751 + r_i + 2i) mod n) + 1,
//        v_{i+1} = ((13 * v_i + 593 + r_i + 5i) mod n) + 1,
//        где r_i — ответ на запрос номер i.
//
//        Обратите внимание, что u_i может быть больше, чем v_i.
//
// Выходные данные
// В выходной файл выведите u_m, v_m и r_m (последний запрос и ответ на него).

//
// Примеры
// Входные данные
// 10 8 12345
// 3 9
//
// Выходные данные
// 5 3 1565158

#include <iostream>
#include <vector>

size_t F(size_t x, size_t y) {
    if (x < y)
        return x;
    return y;
}

class SparseTable {
public:
    explicit SparseTable(std::vector<int> &A) : a_(std::move(A)), min_(a_.size()) {
        auto K = 25;
        for (size_t i = 0; i < a_.size(); ++i) {
            min_[i].push_back(a_[i]);
        }

        for (size_t k = 1; k < K; ++k) {
            for (size_t i = 0; i + ((1 << k)) <= a_.size(); ++i) {

//                if (i + (1 << k) < a_.size())
                min_[i].push_back(std::min(min_[i][k - 1], min_[i + (1 << (k - 1))][k - 1]));
//                else
//                    min_[i].push_back(min_[i][k - 1]);

            }
        }
    };

    int min(size_t l, size_t r) {
        size_t k = 0;
        for (size_t i = 0; (1 << i) <= r + 1 - l; ++i) {
            k = i;
        }
        return std::min(min_[l][k], min_[r + 1 - (1 << k)][k]);
    }

private:
    std::vector<int> a_;
    std::vector<std::vector<int>> min_;
};

int main() {
    int n, m, a_1;
    std::cin >> n >> m >> a_1;
    const int modulo = 16714589;
    std::vector<int> A(n, a_1);

    for (size_t i = 1; i < n; ++i) {
        auto val = (23 * A[i - 1] + 21563);
        A[i] = val > modulo ? val % modulo : val;
    }

    size_t u, v;
    std::cin >> u >> v;
    SparseTable sparseTable(A);
    size_t r = 12345;
    for (int i = 0; i < m; ++i) {
        if (u > v) {
            r = sparseTable.min(v - 1, u - 1);
        } else {
            r = sparseTable.min(u - 1, v - 1);
        }

        //std::cout << "[" << u << "," << v << "]" << " = " << r << std::endl;
        if (i + 1 < m) {
            u = ((17 * u + 751 + r + 2 * (i + 1)) % n) + 1;
            v = ((13 * v + 593 + r + 5 * (i + 1)) % n) + 1;
        }

    }

    std::cout << u << " " << v << " " << r << std::endl;

    return 0;
}
