
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

void merge(std::vector<int>& arr, int val)
{
    for(size_t k = 0; k < arr.size(); ++k)
    {
        if(arr[k]>val)
        {
            if(k + 1 < arr.size())
            {
                std::cout<<k<<" "<< arr.size() - 2<<" "<< k + 1<< std::endl;
            }
            arr[k]=val;
            break;
        }
    }
}

void task_3(size_t n, size_t k)
{
    for (size_t i = 0; i < n; ++i)
    {

    }

    std::vector<int> arr = {1,3,5,6,8,9};
    merge(arr, 4);
    for(auto& i: arr)
    {
        std::cout<<i<<" ";
    }

}

int main(int argc, char** argv)
{

    size_t n, k;
    std::cin >> n >> k;
    task_3(n, k);
    return 0;
}