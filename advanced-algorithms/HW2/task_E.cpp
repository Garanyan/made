/*
 * Замощение доминошками
 *
ограничение по времени на тест 1 секунда
ограничение по памяти на тест 256 мегабайт
ввод стандартный ввод
вывод стандартный вывод

Вам дана прямоугольная сетка из квадратных ячеек. Символ 'X' представляет собой уже покрытую ячейку,
 символ '.' - это ячейка, которую еще нужно покрыть.

Вы хотите покрыть все ячейки '.', непересекающимися доминошками 2×1. Найдите количество способов сделать это.
Два способа считаются различными, если две ячейки покрыты одним и
тем же домино в одном замощении и двумя разными домино в другом замощении.

Входные данные
Первая строка содержит числа n
и m (1≤n,m≤12). Следующие n строк содержат по m символов — описание доски.

Выходные данные
Выведите число замощений доминошками.
 */

#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

long long DominoTiling(const std::vector<std::vector<bool>> &spaces, const size_t rows, const size_t cols) {

    const size_t mask_size = 1 << rows;
    std::vector<std::vector<long long>> dp(rows * cols + 1, std::vector<long long>(mask_size, 0));

    dp[0][0] = 1;
    for (size_t j = 0; j < cols; ++j){
        for (size_t i = 0; i < rows; ++i){
            const size_t space_loc = j * rows + i;
            const size_t next_row_space_loc = j * rows + i + 1;

            for (size_t mask = 0; mask < mask_size; ++mask){
                if ((mask & 1) || !spaces[i][j]){
                    dp[next_row_space_loc][(mask >> 1)] += dp[space_loc][mask];
                }
                else{
                    if (i != rows - 1 && (mask & 2) == 0 &&
                        spaces[i + 1][j]){
                        dp[next_row_space_loc][(mask >> 1) + 1] += dp[space_loc][mask];
                    }
                    if (j != cols - 1){
                        if (spaces[i][j + 1]){
                            dp[next_row_space_loc][(mask >> 1) + (1 << (rows - 1))] += dp[space_loc][mask];
                        }
                    }
                }
            }
        }
    }

    return dp[rows * cols][0];
}


int main() {
    std::size_t rows, cols;
    std::cin >> rows >> cols;

    std::vector<std::vector<bool>> table(rows, std::vector<bool>(cols, false));
    char cell;
    for (std::size_t i = 0; i < rows; ++i){
        for (std::size_t j = 0; j < cols; ++j){
            std::cin >> cell;
            table[i][j] = (cell != 'X');
        }
    }

    std::cout << DominoTiling(table, rows, cols) << std::endl;

    return 0;
}
