//
// Created by Ovanes on 06/03/2020.
//

// Удаление скобок 2.0 (подотрезки)
//ограничение по времени на тест
//2 секунды
//    ограничение по памяти на тест
//256 мегабайт
//    ввод
//стандартный ввод
//вывод
//    стандартный вывод
//
//    Дана строка, составленная из круглых, квадратных и фигурных скобок. Определите,
//    какое наименьшее количество символов необходимо удалить из этой строки,
//    чтобы оставшиеся символы образовывали правильную скобочную последовательность.

//Входные данные
//
//Во входном файле записана строка из круглых, квадратных и фигурных скобок.
// Длина строки не превосходит {100} символов.

//Выходные данные
//
//Выведите строку максимальной длины, являющейся правильной скобочной последовательностью,
// которую можно получить из исходной строки удалением некоторых символов.

#include <iostream>
#include <vector>
#include <queue>

int main(int argc, char **argv) {
    std::string braket_seq;
    std::cin >> braket_seq;

    std::vector<std::vector<int>> dp(braket_seq.size(), std::vector<int>(braket_seq.size(), 1));
    for (int i = braket_seq.size() - 1; i >= 0; --i) {
        for (size_t j = 0; j < braket_seq.size(); ++j) {
            if (i != j) {
                int ans = braket_seq.size();
                if ((braket_seq[i] == '(' && braket_seq[j] == ')' ||
                     braket_seq[i] == '[' && braket_seq[j] == ']' ||
                     braket_seq[i] == '{' && braket_seq[j] == '}')
                        ) {
                    if (i + 1 != j && i + 1 < braket_seq.size()) {
                        ans = dp[i + 1][j - 1];
                    } else if (i + 1 == j) {
                        ans = 0;
                    }
                }

                for (size_t k = i + 1; k < j; ++k) {
                    ans = std::min(ans, dp[i][k] + dp[k + 1][j]);
                }

                if (i + 1 < braket_seq.size()) {
                    ans = std::min(ans, dp[i + 1][j] + 1);
                }

                if (1 < j) {
                    ans = std::min(ans, dp[i][j - 1] + 1);
                }

                dp[i][j] = ans;
            }
        }
    }
    //std::cout << dp[0].back() << std::endl;
    std::vector<bool> to_del(braket_seq.size(), false);
    std::queue<std::pair<int, int>> sub_seq;
    sub_seq.push(std::make_pair(0, braket_seq.size() - 1));
    while (!sub_seq.empty()) {
        auto elem = sub_seq.front();
        sub_seq.pop();
        int i = elem.first;
        int j = elem.second;


        if ((braket_seq[i] == '(' && braket_seq[j] == ')' ||
             braket_seq[i] == '[' && braket_seq[j] == ']' ||
             braket_seq[i] == '{' && braket_seq[j] == '}')
                ) {

            if (i + 1 != j && i + 1 < braket_seq.size() && dp[i][j] == dp[i + 1][j - 1]) {
                sub_seq.push(std::make_pair(i + 1, j - 1));
                continue;
            }
            if (i + 1 == j) {
                continue;
            }

        }

        if (i == j) {
            to_del[i] = true;
            continue;
        }

        if (i + 1 < braket_seq.size() && dp[i][j] == dp[i + 1][j] + 1) {
            to_del[i] = true;
            sub_seq.push(std::make_pair(i + 1, j));
            continue;
        }

        if (1 < j && dp[i][j] == dp[i][j - 1] + 1) {
            to_del[j] = true;
            sub_seq.push(std::make_pair(i, j - 1));
            continue;
        }

        bool flag = false;
        std::pair<int, int> sub_seq_1(i, j);
        std::pair<int, int> sub_seq_2(i, j);
        for (size_t k = i + 1; k < j; ++k) {

            if (dp[i][j] == dp[i][k] + dp[k + 1][j]
//                dp[sub_seq_1.first][sub_seq_1.second] + dp[sub_seq_2.first][sub_seq_2.second] >
//                dp[i][k] + dp[k + 1][j]
                    ) {

                sub_seq_1.first = i;
                sub_seq_1.second = k;

                sub_seq_2.first = k + 1;
                sub_seq_2.second = j;

                flag = true;

            }
        }

        sub_seq.push(sub_seq_1);
        sub_seq.push(sub_seq_2);

    }

    for (size_t i = 0; i < braket_seq.size(); ++i) {
        if (!to_del[i]) {
            std::cout << braket_seq[i];
        }

    }

    return 0;
}
// [][][][][](](](](](])])