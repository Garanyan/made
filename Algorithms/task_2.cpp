//
// Created by Ovanes on 13.10.2019.
//

//Задача № 2 (5 баллов)
//Решение всех задач данного раздела предполагает использование кучи.
//2_4. Скользящий максимум.
//Дан массив целых чисел A[0..n), n не превосходит 10^8. Также задан размер некоторого окна (последовательно расположенных элементов массива) в этом массиве k, k<=n. Требуется для каждого положения окна (от 0 и до n-k) вывести значение максимума в окне. Скорость работы O(n log n), память O(n).
//Формат входных данных. Вначале вводится n - количество элементов массива. Затем вводится n строк со значением каждого элемента. Затем вводится k  - размер окна.
//Формат выходных данных. Разделенные пробелом значения максимумов для каждого положения окна.
// set на дереве
// куча , можно не извлекать мининмум, но смотреть на координату при извлечении (извлекать только при проверке?)
// очередь
// массив максимум на префиксах , массив  максимум на суффиксах
#include <cstdio>
#include <iostream>

int main(int argc, char** argv)
{
    size_t result = 0;

    std::cout<<result;
    return 0;
}