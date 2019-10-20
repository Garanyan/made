//
// Created by Ovanes on 19.10.2019.
//smatsk@yandex.ru

#include <algorithm>
#include <iostream>

struct Point {
    int x;
    int y;

    bool operator<(const Point &other) const {
        return x < other.x;
    }
};

bool CompareByDistToZero(const Point &first, const Point &second) {
    return (first.x * first.x + first.y * first.y) < (second.x * second.x + second.y * second.y);
}

template<typename TCompare>
void BoubbleSort(Point *arr, int size, const TCompare &f) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; ++j) {
            if (f(arr[j + 1], arr[j])) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void BoubbleSort(Point *arr, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; ++j) {
            if (arr[j + 1] < arr[j]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Функтор,сравнивающий точки по расстоянию до center.
class ComparerByDistToCenter {
public:
    explicit ComparerByDistToCenter(const Point &center) : center_(center) {};

    bool operator()(const Point &first, const Point &second) const;

private:
    Point center_;
};

bool ComparerByDistToCenter::operator()(const Point &first, const Point &second) const {
    Point vect{first.x - center_.x, first.y - center_.y};
    Point vectToSec{second.x - center_.x, second.y - center_.y};
    return (vect.x * vect.x + vect.y * vect.y) < (vectToSec.x * vectToSec.x + vectToSec.y * vectToSec.y);;
}
//универсальная ссылка


int main() {
    Point arr[]{Point{3, 5}, Point{0, 0}, Point{2, 8}, Point{1, 2}, Point{4, 2}};
    BoubbleSort(arr, 5, ComparerByDistToCenter({3, 3}));

    for (int i = 0; i < 5; i++) {
        std::cout << arr[i].x << " " << arr[i].y << std::endl;
    }

    return 0;
}