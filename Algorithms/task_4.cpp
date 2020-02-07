
//
// Created by ovanes on 31/10/2019.
//

//Даны неотрицательные целые числа n,k и массив целых чисел из [0..10^9] размера n.
// Требуется найти k-ю порядковую статистику. т.е. напечатать число, которое бы стояло на позиции с индексом k (0..n-1) в отсортированном массиве.
// Напишите нерекурсивный алгоритм.
//Требования к дополнительной памяти: O(n). Требуемое среднее время работы: O(n).

//Функцию Partition следует реализовывать методом прохода двумя итераторами в одном направлении.
// Описание для случая прохода от начала массива к концу:
//Выбирается опорный элемент. Опорный элемент меняется с последним элементом массива.
//Во время работы Partition в начале массива содержатся элементы, не бОльшие опорного.
// Затем располагаются элементы, строго бОльшие опорного. В конце массива лежат нерассмотренные элементы.
// Последним элементом лежит опорный.
//Итератор (индекс) i указывает на начало группы элементов, строго бОльших опорного.
//Итератор j больше i, итератор j указывает на первый нерассмотренный элемент.
//Шаг алгоритма. Рассматривается элемент, на который указывает j. Если он больше опорного, то сдвигаем j.
//Если он не больше опорного, то меняем a[i] и a[j] местами, сдвигаем i и сдвигаем j.
//В конце работы алгоритма меняем опорный и элемент, на который указывает итератор i.

//4_3. Реализуйте стратегию выбора опорного элемента “случайный элемент”.
//Функцию Partition реализуйте методом прохода двумя итераторами от начала массива к концу.


//In
//10 4
//1 2 3 4 5 6 7 8 9 10
//Out
//5
//
//In
//10 0
//3 6 5 7 2 9 8 10 4 1
//Out
//1
//
//In
//10 9
//0 0 0 0 0 0 0 0 0 1
//Out
//1

#include <iostream>
#include <random>

void print(std::vector<int> numbers){
    for(size_t i = 0; i < numbers.size(); ++i){
        std::cout << numbers[i] << " ";
    }
    std::cout << std::endl;
}

size_t partition(int *array, size_t i, size_t pivot_id){
    int pivot = array[pivot_id];

    for(size_t j = i; j <= pivot_id; ++j){
        if(array[j] <= pivot){
            std::swap(array[i], array[j]);
            ++i;
        }
    }
    return i - 1;
}

void Sort(std::vector<int> &data, size_t k){
    std::random_device random_device;
    std::mt19937 random_engine(random_device());

    size_t l = 0, h = data.size() - 1;

    while(1 < h - l){
        std::uniform_int_distribution<int> distribution(l, h);
        size_t pivot_id = distribution(random_engine);
        std::swap(data[pivot_id], data[h]);
        size_t new_pivot_id = partition(data.data(), l, h);
        if(k < new_pivot_id){
            h = new_pivot_id - 1;
        }
        else{
            l = new_pivot_id;
        }
    }
}

void task_4(std::vector<int> &numbers, const size_t k){
    Sort(numbers, k);
    std::cout << numbers[k] << std::endl;
}

int main(int argc, char **argv){

    size_t n, k;
    std::cin >> n >> k;
    std::vector<int> data;
    data.reserve(n);
    for(size_t i = 0; i < n; i++){
        int value = 0;
        std::cin >> value;
        data.push_back(value);
    }
    task_4(data, k);

    return 0;
}