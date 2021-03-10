//
// Created by Ovanes on 3/29/20.
//

// RSQ
//        ограничение по времени на тест 2 секунды
//        ограничение по памяти на тест 256 мегабайт
//        ввод стандартный ввод
//        вывод стандартный вывод
//
//        Входные данные
//        В первой строке находится число n — размер массива. (1 ≤ n ≤ 500 000)
//        Во второй строке находится n чисел ai — элементы массива. Далее содержится описание операций,
//        их количество не превышает 1 000 000. В каждой строке находится одна из следующих операций:
//        set i x — установить a[i] в x.
//        sum i j — вывести значение суммы элементов в массиве на отрезке с i по j, гарантируется, что (1 ≤ i ≤ j ≤ n).
//
// Все числа во входном файле и результаты выполнения всех операций не превышают по модулю 10^18.
// Выходные данные
// Выведите последовательно результат выполнения всех операций sum. Следуйте формату выходного файла из примера.

#include <iostream>
#include <vector>

class FenwickTree {
public:
    explicit FenwickTree(std::vector<long long> &values);

    void set(long long i, long long value);

    long long sum(long long i, long long j);

    void add(long long i, long long x);

private:
    long long get(long long i);

    std::vector<long long> a;
    std::vector<long long> T;

};

long long F(long long i) {
    return i & (i + 1);
}


FenwickTree::FenwickTree(std::vector<long long> &values) : a(std::move(values)), T(a.size(), 0) {
    for (size_t i = 0; i < a.size(); ++i) {
        long long F_i = F(i);
        for (size_t j = F_i; j <= i; ++j) {
            T[i] += a[j];
        }
    }

}

long long FenwickTree::sum(long long i, long long j) {
    if (i == 0) {
        return get(j);
    }

    return get(j) - get(i - 1);
}

long long FenwickTree::get(long long i) {
    long long result = 0;
    while (i >= 0) {
        result += T[i];
        i = F(i) - 1;
    }
    return result;
}

void FenwickTree::set(long long i, long long value) {
    auto d = value - a[i];
    a[i] = value;
    add(i, d);

}

void FenwickTree::add(long long i, long long x) {
    auto j = i;
    while (j < T.size()) {
        T[j] += x;
        j |= (j + 1);
    }
}

int main() {
    int n;
    std::cin >> n;
    std::vector<long long> a(n, 0);
    for (int i = 0; i < n; ++i) {
        std::cin >> a[i];
    }

    FenwickTree fenwickTree(a);
    std::string command;
    long long position, value;
    while (std::cin >> command >> position >> value) {
        if (command == "set") {
            fenwickTree.set(position - 1, value);
        }
        if (command == "sum") {
            std::cout << fenwickTree.sum(position - 1, value - 1) << std::endl;
        }
    }
    return 0;
}
