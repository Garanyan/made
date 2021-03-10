//
// Created by Ovanes on 4/6/20.
//

// Звезды
//        ограничение по времени на тест 2 секунды
//        ограничение по памяти на тест 256 мегабайт
//        ввод стандартный ввод
//        вывод стандартный вывод
//
//        Вася любит наблюдать за звездами. Но следить за всем небом сразу ему тяжело.
//        Поэтому он наблюдает только за частью пространства, ограниченной кубом размером n × n × n.
//        Этот куб поделен на маленькие кубики размером 1 × 1 × 1.

//        Во время его наблюдений могут происходить следующие события:
//        В каком-то кубике появляются или исчезают несколько звезд.
//        К нему может заглянуть его друг Петя и поинтересоваться, сколько видно звезд в части пространства,
//        состоящей из нескольких кубиков.
//
// Входные данные
// Первая строка входного файла содержит натуральное число 1 ≤ n ≤ 128.
// Координаты кубиков — целые числа от 0 до n - 1. Далее следуют записи о происходивших событиях по одной в строке.
// В начале строки записано число m. Если m равно:
// 1, то за ним следуют 4 числа — x, y, z (0 ≤ x, y, z < N) и k (-20000 ≤ k ≤ 20000) — координаты кубика и величина,
// на которую в нем изменилось количество видимых звезд;
// 2, то за ним следуют 6 чисел — x1, y1, z1, x2, y2, z2 (0 ≤ x1 ≤ x2 < N, 0 ≤ y1 ≤ y2 < N, 0 ≤ z1 ≤ z2 < N),
// которые означают, что Петя попросил подсчитать количество звезд в кубиках (x, y, z) из
// области: x1 ≤ x ≤ x2, y1 ≤ y ≤ y2, z1 ≤ z ≤ z2;
// 3, то это означает, что Васе надоело наблюдать за звездами и отвечать на вопросы Пети.
// Эта запись встречается во входном файле только один раз и будет последней.
//
// Количество записей во входном файле не больше 100 002.
// Выходные данные
//
// Для каждого Петиного вопроса выведите искомое количество звезд.

//Пример
//Входные данные
//2
//2 1 1 1 1 1 1
//1 0 0 0 1
//1 0 1 0 3
//2 0 0 0 0 0 0
//2 0 0 0 0 1 0
//1 0 1 0 -2
//2 0 0 0 1 1 1
//3
//
//Выходные данные
//0
//1
//4
//2

#include <iostream>
#include <vector>

class FenwickTree3D {
public:
    FenwickTree3D(int x, int y, int z);

    long long sum(int x_1, int y_1, int z_1, int x_2, int y_2, int z_2);

    void add(int x, int y, int z, int diff);

private:
    long long get(int x, int y, int z);

    std::vector<std::vector<std::vector<int>>> T;
    int cube_x_size_ = 0;
    int cube_y_size_ = 0;
    int cube_z_size_ = 0;

};

long long F(long long i) {
    return i & (i + 1);
}


FenwickTree3D::FenwickTree3D(int x, int y, int z) :
        T(x, std::vector<std::vector<int>>(y, std::vector<int>(z, 0))) {
    cube_x_size_ = x;
    cube_y_size_ = y;
    cube_z_size_ = z;
}

long long FenwickTree3D::sum(int x_1, int y_1, int z_1, int x_2, int y_2, int z_2) {
    if (x_1 == 0 && y_1 == 0 && z_1 == 0) {
        return get(x_2, y_2, z_2);
    }

    if (x_1 == 0 && y_1 == 0) {
        return get(x_2, y_2, z_2) - get(x_2, y_2, z_1 - 1);
    }

    if (y_1 == 0 && z_1 == 0) {
        return get(x_2, y_2, z_2) - get(x_1 - 1, y_2, z_2);
    }

    if (x_1 == 0 && z_1 == 0) {
        return get(x_2, y_2, z_2) - get(x_2, y_1 - 1, z_2);
    }

    if (x_1 == 0) {
        return get(x_2, y_2, z_2) - get(x_2, y_2, z_1 - 1) - get(x_2, y_1 - 1, z_2) +
               get(x_2, y_1 - 1, z_1 - 1);
    }

    if (y_1 == 0) {
        return get(x_2, y_2, z_2) - get(x_2, y_2, z_1 - 1) - get(x_1 - 1, y_2, z_2) +
               +get(x_1 - 1, y_2, z_1 - 1);
    }

    if (z_1 == 0) {
        return get(x_2, y_2, z_2) - get(x_2, y_1 - 1, z_2) - get(x_1 - 1, y_2, z_2) +
               +get(x_1 - 1, y_1 - 1, z_2);
    }

    return get(x_2, y_2, z_2) - get(x_2, y_2, z_1 - 1) - get(x_2, y_1 - 1, z_2) - get(x_1 - 1, y_2, z_2) +
           get(x_2, y_1 - 1, z_1 - 1) + get(x_1 - 1, y_2, z_1 - 1) + get(x_1 - 1, y_1 - 1, z_2)
           - get(x_1 - 1, y_1 - 1, z_1 - 1);
}

long long FenwickTree3D::get(int x, int y, int z) {
    long long result = 0;

    for (int i = x; i >= 0; i = F(i) - 1) {
        for (int j = y; j >= 0; j = F(j) - 1) {
            for (int k = z; k >= 0; k = F(k) - 1) {
                result += T[i][j][k];
            }
        }
    }
    return result;
}

void FenwickTree3D::add(int x, int y, int z, int diff) {
    for (int i = x; i < cube_x_size_; i = (i | (i + 1))) {
        for (int j = y; j < cube_y_size_; j = (j | (j + 1))) {
            for (int k = z; k < cube_z_size_; k = (k | (k + 1))) {
                T[i][j][k] += diff;
            }
        }
    }
}

int main() {
    size_t cube_size = 0;
    std::cin >> cube_size;
    int command = 0;
    int x_1, x_2, y_1, y_2, z_1, z_2, stars_diff;
    FenwickTree3D fenwickTree(cube_size, cube_size, cube_size);
    while (std::cin >> command) {
        if (command == 1) {
            std::cin >> x_1 >> y_1 >> z_1 >> stars_diff;
            fenwickTree.add(x_1, y_1, z_1, stars_diff);
        }
        if (command == 2) {
            std::cin >> x_1 >> y_1 >> z_1 >> x_2 >> y_2 >> z_2;
            std::cout << fenwickTree.sum(x_1, y_1, z_1, x_2, y_2, z_2) << std::endl;
        }
        if (command == 3) {
            break;
        }
    }
    return 0;
}
