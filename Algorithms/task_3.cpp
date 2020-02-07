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

void merge(int arr[], const int left_begin, const int left_end, const int right_end, const int stop = -1){
    int left_it, right_it, result_it;
    int left_size = left_end - left_begin + 1;
    int right_size = right_end - left_end;
    int left_array[left_size], right_array[right_size];

    for(left_it = 0; left_it < left_size; left_it++)
        left_array[left_it] = arr[left_begin + left_it];
    for(right_it = 0; right_it < right_size; right_it++)
        right_array[right_it] = arr[left_end + 1 + right_it];

    right_it = 0;
    left_it = 0;
    result_it = left_begin;
    while(left_it < left_size && right_it < right_size){
        if(left_array[left_it] <= right_array[right_it]){
            arr[result_it] = left_array[left_it];
            left_it++;
        }
        else{
            arr[result_it] = right_array[right_it];
            right_it++;
        }
        result_it++;
        if(result_it == stop){
            return;
        }
    }

    while(left_it < left_size){
        arr[result_it] = left_array[left_it];
        left_it++;
        result_it++;
        if(result_it == stop)
            return;
    }

    while(right_it < right_size){
        arr[result_it] = right_array[right_it];
        right_it++;
        result_it++;
        if(result_it == stop)
            return;
    }
}

void mergeSort(int arr[], int l, int r){
    if(l < r){
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l + (r - l) / 2;

        // Sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        merge(arr, l, m, r);
    }

}

void print(const std::vector<int> &vector){
    for(size_t i = 0; i < vector.size(); i++){
        std::cout << vector[i] << " ";
    }
    std::cout << std::endl;
}

void task_3(size_t n, size_t k){
    std::vector<int> result;
    result.reserve(2 * k);
    for(size_t i = 0; i < k; ++i){
        int value = 0;
        std::cin >> value;
        result.push_back(value);
    }

    mergeSort(result.data(), 0, result.size() - 1);
    for(size_t i = 0; i < k; ++i){
        result.push_back(0);
    }
    for(size_t i = k; i < n;){
        std::vector<int> tmp;
        for(size_t j = 0; j < k && i < n; ++i, ++j){
            int value = 0;
            std::cin >> value;
            tmp.push_back(value);
        }
        mergeSort(tmp.data(), 0, tmp.size() - 1);
        std::copy(tmp.begin(), tmp.end(), result.begin() + k);
        merge(result.data(), 0, k - 1, k + tmp.size() - 1, k);
        //print(result);
    }
    for(size_t i = 0; i < k; i++){
        std::cout << result[i] << " ";
    }
    std::cout << std::endl;
}

int main(int argc, char **argv){
    size_t n, k;
    std::cin >> n >> k;
    task_3(n, k);

    return 0;
}
