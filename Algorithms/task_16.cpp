
//
// Created by Garanyan Ovanes (Ovanes-x) on 17/12/2019.
//

//Задача 16. Поиск подстроки (5 баллов)
//Найдите все вхождения шаблона в строку.
//Длина шаблона – p, длина строки ­– n. Время O(n + p), доп. память – O(p).
//Вариант 1. С помощью префикс-функции (алгоритм Кнута-Морриса-Пратта).

#include <iostream>
#include <vector>

std::vector<size_t> piFucntion(const std::string& text)
{
    std::vector<size_t> pi_values(text.size(), 0);
    for (size_t i = 1; i < text.size(); ++i)
    {
        size_t k = pi_values[i - 1];
        while (k > 0 && text.at(k) != text.at(i))
            k = pi_values[k - 1];

        if (text.at(k) == text.at(i))
            ++k;

        pi_values[i] = k;

    }
    return pi_values;
}

void printPatternPositions(const std::string& pattern, const std::string& text)
{
    std::vector<size_t> A;
    A.reserve(text.size());
    std::vector<size_t> pi = piFucntion(text);

    for (size_t i = 0, k = 0; i < text.size(); ++i)
    {
        while (k > 0 && (k >= pattern.size() ||text.at(i) != pattern[k]))
            k = pi.at(k-1);

        if (text.at(i) == pattern.at(k))
            ++k;

        if (k == pattern.size())
            A.push_back(i + 1 - pattern.size());

    }

    for (auto pos: A)
    {
        std::cout << pos << " ";
    }
}

int main(int argc, char** argv)
{
    std::string pattern, text;
    std::cin >> pattern >> text;

    printPatternPositions(pattern, text);
    return 0;
}
