
//
// Created by ovanes on 28/10/2019.
//

//3_1. Первые k элементов длинной последовательности.
//Дана очень длинная последовательность целых чисел длины n. Требуется вывести в отсортированном виде её первые k элементов.
//Последовательность может не помещаться в память. Время работы O(n * log(k)). Память O(k). Использовать слияние.
//
//In
//9 4
//3 7 4 5 6 1 15 4 2
//Out
//1 2 3 4

#include <iostream>
#include <vector>
#include <cstring>

class SortedVector{
public:
    explicit SortedVector(size_t k);

    ~SortedVector();

    void push(int value);

    void print() const;

private:

    size_t buffer_size;
    size_t size;
    int *data;
};

SortedVector::~SortedVector(){
    delete[] data;
}

SortedVector::SortedVector(size_t k) : size(0), buffer_size(k){
    data = new int[buffer_size];
}

int BinarySearchPosition(int *array, int left, int right, int value){
    if(value <= array[left])
        return left;
    if(value > array[right])
        return -1;

    int middle = (left + right) / 2;
    if(array[middle] == value){
        return middle;
    }

    else if(array[middle] < value){
        if(middle + 1 <= right && value <= array[middle + 1]){
            return middle + 1;
        }
        else{
            return BinarySearchPosition(array, middle + 1, right, value);
        }
    }
    else{
        if(left <= middle - 1 && array[middle - 1] < value){
            return middle;
        }
        else{
            return BinarySearchPosition(array, left, middle - 1, value);
        }
    }
}

void SortedVector::push(int value){
    if(size == 0){
        data[size] = value;
        size++;
    }
    else if(size < buffer_size){
        int id = BinarySearchPosition(data, 0, size - 1, value);
        if(id != -1 && id + 1 < buffer_size){
            std::memmove(data + id + 1, data + id, (size - id) * sizeof(int));
            data[id] = value;
        }
        else if(id == -1){
            data[size] = value;
        }
        size++;
    }
    else{
        int id = BinarySearchPosition(data, 0, size - 1, value);
        if(id != -1 && id + 1 < size){
            std::memmove(data + id + 1, data + id, (size - id - 1) * sizeof(int));
            data[id] = value;
        }
        else if(id + 1 == size){
            data[id] = value;
        }
    }
}

void SortedVector::print() const{
    for(size_t i = 0; i < size; i++){
        std::cout << data[i] << " ";
    }
    std::cout << std::endl;
}

void task_3(size_t n, size_t k){
    SortedVector sorted_vector(k);
    for(size_t i = 0; i < n; ++i){
        int value = 0;
        std::cin >> value;
        sorted_vector.push(value);
    }
    sorted_vector.print();
}

int main(int argc, char **argv){
    size_t n, k;
    std::cin >> n >> k;
    task_3(n, k);

    return 0;
}