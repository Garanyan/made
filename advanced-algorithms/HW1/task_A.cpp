//
// Created by Ovanes on 26.02.2020.
//

//A. Кузнечик собирает монеты
//    ограничение по времени на тест
//2 секунды
//    ограничение по памяти на тест
//256 мегабайт
//    ввод
// input.txt
//    вывод
// output.txt
//
//    Кузнечик прыгает по столбикам, расположенным на одной линии на равных расстояниях друг от друга. Столбики имеют порядковые номера от 1 до N.
//    В начале Кузнечик сидит на столбике с номером 1 и хочет добраться до столбика с номером N. Он может прыгнуть вперед на расстояние
//    от 1 до K столбиков, считая от текущего.
//
// На каждом столбике Кузнечик может получить или потерять несколько золотых монет (для каждого столбика это число известно). Определите,
// как нужно прыгать Кузнечику, чтобы собрать наибольшее количество золотых монет. Учитывайте, что Кузнечик не может прыгать назад.
// Входные данные
//
// В первой строке вводятся два натуральных числа: N и K (2 ≤ N, K ≤ 10000), разделённые пробелом. Во второй строке записаны
// через пробел N - 2 целых числа – количество монет, которое Кузнечик получает на каждом столбике, от 2-го до N - 1-го.
// Если это число отрицательное, Кузнечик теряет монеты. Гарантируется, что все числа по модулю не превосходят 10 000.
// Выходные данные
//
// В первой строке программа должна вывести наибольшее количество монет, которое может собрать Кузнечик.
// Во второй строке выводится число прыжков Кузнечика, а в третьей строке – номера всех столбиков,
// которые посетил Кузнечик (через пробел в порядке возрастания).
//
// Если правильных ответов несколько, выведите любой из них.

//In
//5 3
//2 -3 5
//Out
//7
//3
//1 2 4 5

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

int main(int argc, char** argv)
{
    std::ifstream inFile;
    inFile.open("input.txt");
    int N = 0, K = 0;
    inFile >> N;
    inFile >> K;

    std::vector<int> cost(N, 0);
    for (size_t i = 1; i < N - 1; ++i)
    {
        inFile >> cost[i];
    }


    std::vector<int> money(N, 0);
    // step
    for (int i = 1; i < money.size(); ++i)
    {
        size_t last_stone = std::max(0, i - K);
        auto it = max_element(money.begin() + last_stone, money.begin() + i);
        money[i] = cost[i] + *it;
    }

    std::vector<bool> path(N, false);
    path.back() = true;
    int steps = 0;
    for (int i = N - 1; i > 0;)
    {
        size_t last_stone = std::max(0, i - K);
        auto it = max_element(money.begin() + last_stone, money.begin() + i);
        i = it - money.begin();
        path[i] = true;
        ++steps;
    }
    std::ofstream outFile;
    outFile.open("output.txt");
    outFile << money.back() << std::endl;
    outFile << steps << std::endl;

    for (size_t i = 0; i < path.size(); ++i)
    {
        if (path[i])
            outFile << i + 1 << " ";
    }
    return 0;
}