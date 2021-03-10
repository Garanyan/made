//
// Created by Ovanes on 06/03/2020.
//

// Разбиение на пары (подмножества, рассмотрена на лекции)

//    ограничение по времени на тест: 2 секунды
//    ограничение по памяти на тест: 64 мегабайта
//    ввод: стандартный ввод
//    вывод: стандартный вывод
//
//    Однажды n ребят решили поучаствовать в VK Cup.
//    Для этого им нужно разбиться на команды по два человека. Однако выяснилось, что не все ребята хорошо ладят друг с другом.
//    Вы выяснили для каждой пары ребят, могут ли они участвовать в одной команде. Помогите им сформировать максимальное число команд.

// Входные данные
// Первая строка содержит число n (1 ≤ n ≤ 18), далее n строк по n символов описывают отношения ребят.
// Символ j строки i равен «Y», если ребята i и j могут участвовать в команде.

// Выходные данные
// Выведите одно число — максимальное число участников в командах, которые можно сформировать.

#include <iostream>
#include <vector>


size_t getBit(size_t mask, size_t i) {
    return (mask >> i) & 1;
}

int main(int argc, char **argv) {
    int N = 0;
    std::cin >> N;
    std::vector<std::vector<bool>> adjacent_matrix(N, std::vector<bool>(N, false));
    for (int i = 0; i < N; ++i) {
        char edge;
        for (int j = 0; j < N; ++j) {
            std::cin >> edge;
            if (edge == 'Y') {
                adjacent_matrix[i][j] = true;
            }
        }
    }

    std::vector<int> dp(1 << N, 0);
    for (size_t mask = 1; mask < dp.size(); ++mask) {
        for (size_t i = 0; i < N; ++i) {
            if (getBit(mask, i) == 1) {
                dp[mask] = dp[mask - (1 << i)];
                for (size_t j = 0; j < N; ++j) {
                    if (getBit(mask, j) == 1 && adjacent_matrix[i][j]) {
                        dp[mask] = std::max(dp[mask], dp[mask - (1 << i) - (1 << j)] + 1);
                    }
                }
            }
        }
    }


    std::cout << 2 * dp.back() << std::endl;
    return 0;
}
